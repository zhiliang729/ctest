#include <comlib.h>

int main(int argc, char *argv[])
{
    	int nListenSock = 0, nLocalSock = 0, nRemoteSock = 0;
	if (argc != 4) 		/* 命令行参数：“proxy1 远程IP地址 远程端口 本地端口” */
	{
    		PrintLog(stdout, "proxy1 IP PORT LOCALPORT\n");
    		return 1;
    	}
    	/* 建立侦听套接字 */
    	ASSERT(CreateSock(&nListenSock, atoi(argv[3]), 8) == 0);	
    	PrintLog(stdout, "listen service: service is activated.");
  	while (1)
   	{
     	/* 接收套接字连接 */
    		if (!VERIFY(AcceptSock(&nLocalSock, nListenSock) == 0)) 	
				continue;					/* 继续接收 */
	    	/* 连接目标端*/
		if (!VERIFY(ConnectSock(&nRemoteSock, atoi(argv[2]), argv[1]) == 0))
		{									/* 连接目标端失败 */
			PrintLog(stderr, "Connect Server Fail,Addr[%s],Port[%d]", argv[1], argv[2]);
			return 2;
		}
		SendReceiveServer(nLocalSock, nRemoteSock);		/* 通信转发 */
    		close(nLocalSock);					/* 关闭本地端套接字 */
    		close(nRemoteSock);					/* 关闭目标端套接字 */
	}
}


int SendReceiveServer(int nLocalSock, int nRemoteSock)
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
			PrintLog(stderr, "Select Failed.");
			return 1;
		}
		/* ④监测状态变化的套接字 */
		
		/* 如果套接字nLocalSock可以接收，则接收数据，发送数据到套接字nRemoteSock */
		if (FD_ISSET(nLocalSock, &fdset))
		{
			PrintLog(stderr, "Local To Remote");
			ret = TransSock(nLocalSock, nRemoteSock);
		}
		/* 如果套接字nRemoteSock可以接收，则接收数据，发送到套接字nLocalSock */
		else if (FD_ISSET(nRemoteSock, &fdset))			
		{
			PrintLog(stderr, "Remote To Local");
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
		PrintLog(stderr, "client is close");
		return 2;
	}
	PrintLog(stderr, "[%d]", nread);
	if (WriteFile(nWriteSock, buf, nread) != 0)	/* 转发数据 */
	{
		PrintLog(stderr, "Write message to server error[%d]", nread);
		perror("write");
		return 1;
	}
	return 0;
}
