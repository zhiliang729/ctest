#include <comlib.h>
int main(int argc, char *argv[])
{
    	int nLocalSock = 0, nRemoteSock = -1;
								/* 解析HTTP报文头，并与目标服务器建立连接 */
	if (HttpConnect(nLocalSock, &nRemoteSock) == 0)
		SendReciveServer(nLocalSock, nRemoteSock);			/* 通信转发 */
    	if (nRemoteSock >= 0) close(nRemoteSock);/* ⑨ 子进程关闭目标端套接字 */
}
/* --- 解析HTTP报文头，并与目标服务器建立连接 --- */
int HttpConnect(int nLocalSock, int *nRemoteSock)
{
	char szIp[30], buf[4096];
	int nread;	
	memset(buf,0,sizeof(buf));
											/* 接收网页浏览端发送的数据请求 */
	while ((nread = read(nLocalSock, buf, sizeof(buf))) < 0) 
	{	
		if (errno != EINTR)					/* 信号导致read中断，继续接收 */
		{
			perror("read");					/* 其它错，结束子进程 */
			return 1;				
		}
	}
	memset(szIp, 0, sizeof(szIp));
	/* 解析HTTP报文，简单验证报文，并获取目标服务器的IP地址 */
	if (GetObjectIp(buf, szIp) != 0) return 2;
	/* 连接目标服务器，端口80（网页浏览端口） */
	if (!VERIFY(ConnectSock(nRemoteSock, 80, szIp) == 0))
	{									
		return 3; 							/* 连接目标端失败，结束子进程 */
	}
	if (WriteFile(*nRemoteSock, buf, nread) != 0)			/* 转发数据 */
	{
//		perror("write");					/* 数据转发失败，结束子进程 */
		return 4;
	}	
	return 0;								/* 成功返回 */
}
/* --- 解析HTTP报文，简单验证报文，并获取目标服务器的IP地址 --- */
int GetObjectIp(char *buf, char *ip)
{
	char *p = NULL, *p1 = NULL, hostname[50];
	struct hostent *host; 
	if (strstr(buf, "HTTP") == NULL) return 1;	/* 验证是否为HTTP报文 */
	/* HTTP报文的“Host: ”实体头信息中记载了目标服务器的域名，以下获取域名数据 */
	if ((p = strstr(buf, "Host:")) == NULL) return 2;	/* HTTP报文错 */
	if ((p1 = strstr(p, "\n")) == NULL) return 3;		/* HTTP报文错 */
	memset(hostname, 0, sizeof(hostname));
	strncpy(hostname, p+strlen("Host: "), p1 - p - strlen("Host: "));
	/* 去除字符串末端的CRLF（回车换行） */
	if (hostname[strlen(hostname) - 1] == '\n' || hostname[strlen(hostname) - 1] == '\r')  hostname[strlen(hostname) - 1] = 0;
	if ((host = gethostbyname(hostname)) == NULL)	/* 域名转协议地址 */
	{
		herror("gethostbyname");	
		return 3;
	}
	strcpy(ip, (char *)inet_ntoa(*((struct in_addr *)host->h_addr)));
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
		/* ①创建套接字描述符集合 */
		FD_ZERO(&fdset);
		FD_SET(nLocalSock,&fdset);
		FD_SET(nRemoteSock,&fdset);

		/* ②准备超时时间 */
		wait.tv_sec=60;
		wait.tv_usec=0;
		/* ③调用select，检查返回 */
		nMaxSock = nLocalSock > nRemoteSock ? nLocalSock : nRemoteSock;
		if (select(nMaxSock+1,&fdset, NULL, NULL, &wait) == -1)
		{
			return 1;
		}
		/* ④监测状态变化的套接字 */
		
		/* 如果套接字nLocalSock可以接收，则接收数据，发送数据到套接字nRemoteSock */
		if (FD_ISSET(nLocalSock, &fdset))
		{
			ret = TransSock(nLocalSock, nRemoteSock);
		}
		/* 如果套接字nRemoteSock可以接收，则接收数据，发送到套接字nLocalSock */
		else if (FD_ISSET(nRemoteSock, &fdset))			
		{
			ret = TransSock(nRemoteSock, nLocalSock);
		}
		/* 结果判断 */
		if (ret == 1) return 1;
		else if (ret == 2) break;
	}/* end for while */
	return 0;
}

int TransSock(int nReadSock, int nWriteSock)
{
	int nread;
	char	buf[4096];
	memset(buf,0,sizeof(buf));
	if ((nread = read(nReadSock, buf, sizeof(buf))) < 0) /* 接收数据 */
	{
		if (errno != EINTR) return 1;
		else return 0;
	}
	else if (nread == 0)				/* 套接字关闭 */
	{ 
		return 2;
	}
	if (WriteFile(nWriteSock, buf, nread) != 0)	/* 转发数据 */
	{
//		perror("write");
		return 1;
	}
	return 0;
}
