//
//  proxy2.c
//  comlib
//
//  Created by zhangliang on 14-5-5.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int HttpConnect(int nLocalSock, int *nRemoteSock);
int GetObjectIp(char *buf, char *ip);
int SendReciveServer(int nLocalSock, int nRemoteSock);
int TransSock(int nReadSock, int nWriteSock);

int main(int argc, char *argv[])
{
    int nListenSock = -1, nLocalSock = -1, nRemoteSock = -1;
	pid_t nChild;
	if (argc != 2) 							/*命令行参数：“proxy2 本地端口”*/
	{
        PrintLog(stdout, "proxy1 LOCALPORT\n");
        return 1;
    }
    /* ---------------------------父进程------------------------------- */
    /*1.创建侦听套接字*/
    ASSERT(CreateSock(&nListenSock, atoi(argv[1]), 8) == 0);
    PrintLog(stdout, "listen service: service is activated.");
    InitServer();							/*2.进程转后台运行*/
  	while (1)
   	{
        /*3.等待并创建连接套接字*/
        if (!VERIFY(AcceptSock(&nLocalSock, nListenSock) == 0))
            continue;					/*3.创建成功后，继续等待*/
		VERIFY((nChild = fork()) >= 0);		/*4.创建子进程*/
		if (nChild == 0) break;				/*子进程跳转到子进程代码*/
		close(nLocalSock);					/*5.父进程关闭连接套接字*/
	}
	/* ---------------------------子进程------------------------------- */
	close(nListenSock);						/*7.子进程关闭侦听套接字*/
    /*解析HTTP报文头，并与目标服务器建立连接*/
	if (HttpConnect(nLocalSock, &nRemoteSock) == 0)
		SendReciveServer(nLocalSock, nRemoteSock);			/*通信转发*/
    if (nLocalSock >= 0) close(nLocalSock);  /*9.子进程关闭本地端套接字*/
    if (nRemoteSock >= 0) close(nRemoteSock);/*10.子进程光比目标端套接字*/
}
/* ---解析HTTP报文头，并与目标服务器建立连接--- */
int HttpConnect(int nLocalSock, int *nRemoteSock)
{
	char szIp[30], buf[4096];
	ssize_t nread;
	memset(buf,0,sizeof(buf));
    /*接收网页浏览器端发送的数据请求*/
	while ((nread = read(nLocalSock, buf, sizeof(buf))) < 0)
	{
		if (errno != EINTR)					/*信号导致read中断，继续接收*/
		{
			perror("read");					/*其他错，结束子进程*/
			return 1;
		}
	}
	memset(szIp, 0, sizeof(szIp));
	/*解析HTTP报文，简单验证报文，并获取目标服务器的IP地址*/
	if (GetObjectIp(buf, szIp) != 0) return 2;
	/*连接目标服务器，端口80（网页浏览端口），IP为szip*/
	if (!VERIFY(ConnectSock(nRemoteSock, 80, szIp) == 0))
	{
		PrintLog(stderr, "Connect Server Fail,Addr[%s]", szIp);
		return 3; 							/*连接目标端失败，结束子进程*/
	}
	if (WriteFile(*nRemoteSock, buf, nread) != 0)			/*转发数据*/
	{
		PrintLog(stderr, "Write message to server error[%d]", nread);
		perror("write");					/*数据转发失败，结束子进程*/
		return 4;
	}
	return 0;								/*成功返回*/
}
/* ---解析HTTP报文，简单验证报文，并获取目标服务器的IP地址--- */
int GetObjectIp(char *buf, char *ip)
{
	char *p = NULL, *p1 = NULL, hostname[50];
	struct hostent *host;
	if (strstr(buf, "HTTP") == NULL) return 1;	/*验证是否为HTTP报文*/
	/*HTTP报文的“Host：”实体头信息中记载了目标服务器的域名，以下获取域名数据*/
	if ((p = strstr(buf, "Host:")) == NULL) return 2;	/* HTTP报文错*/
	if ((p1 = strstr(p, "\n")) == NULL) return 3;		/* HTTP报文错 */
    
    /*
     char *
     strstr(const char *s1, const char *s2);
     
     The strstr() function locates the first occurrence of the null-terminated
     string s2 in the null-terminated string s1.
     */
    
	memset(hostname, 0, sizeof(hostname));
	strncpy(hostname, p+strlen("Host: "), p1 - p - strlen("Host: "));
	/*去掉字符串末端的CFLF（回车换行）*/
	if (hostname[strlen(hostname) - 1] == '\n' || hostname[strlen(hostname) - 1] == '\r')  hostname[strlen(hostname) - 1] = 0;
	PrintLog(stderr, "[%s]", hostname);
	if ((host = gethostbyname(hostname)) == NULL)	/*域名转协议地址*/
	{
		herror("gethostbyname");
		return 3;
	}
	strcpy(ip, (char *)inet_ntoa(*((struct in_addr *)host->h_addr)));
	PrintLog(stderr, ip);
	return 0;
}

int SendReciveServer(int nLocalSock, int nRemoteSock)
{
	int nMaxSock;
	struct timeval	wait;
	fd_set fdset;
	int ret;
	while(1)
	{
		/*1.创建套接字描述符集合*/
		FD_ZERO(&fdset);
		FD_SET(nLocalSock,&fdset);
		FD_SET(nRemoteSock,&fdset);
        
		/*2.准备超时时间*/
		wait.tv_sec=60;
		wait.tv_usec=0;
		/*3.调用select，检查返回*/
		nMaxSock = nLocalSock > nRemoteSock ? nLocalSock : nRemoteSock;
		if (select(nMaxSock+1,&fdset, NULL, NULL, &wait) == -1)
		{
			PrintLog(stderr, "Select Failed.");
			return 1;
		}
		/*4.监测状态变化的套接字*/
		
		/*如果套接字nLocalSock可以接收，则接收数据，发送数据到套接字nRemoteSock*/
		if (FD_ISSET(nLocalSock, &fdset))
		{
			PrintLog(stderr, "Local To Remote");
			ret = TransSock(nLocalSock, nRemoteSock);
		}
		/*如果套接字nRemoteSock可以接收，则接收数据，并发送到nLocalSock*/
		else if (FD_ISSET(nRemoteSock, &fdset))
		{
			PrintLog(stderr, "Remote To Local");
			ret = TransSock(nRemoteSock, nLocalSock);
		}
		/*结果判断*/
		if (ret == 1) return 1;/*套接字系统错误，退出函数*/
		else if (ret == 2) break;/*套接字关闭，退出函数*/
	}/* end for while */
	return 0;
}

int TransSock(int nReadSock, int nWriteSock)
{
	int nread;
	char	buf[4096];
	memset(buf,0,sizeof(buf));
	if ((nread = read(nReadSock, buf, sizeof(buf))) < 0) /* Ω” ’ ˝æ› */
	{
		if (errno != EINTR) return 1;
		else return 0;
	}
	else if (nread == 0)				/* Ã◊Ω”◊÷πÿ±’ */
	{
		PrintLog(stderr, "client is close");
		return 2;
	}
	PrintLog(stderr, "[%d]", nread);
	if (WriteFile(nWriteSock, buf, nread) != 0)	/* ◊™∑¢ ˝æ› */
	{
		PrintLog(stderr, "Write message to server error[%d]", nread);
		perror("write");
		return 1;
	}
	return 0;
}
