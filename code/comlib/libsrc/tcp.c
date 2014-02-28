#include <comlib.h>


int CreateSock(int* pSock, int nPort, int nMax)
{
	struct sockaddr_in addrin;
	struct sockaddr *paddr = (struct sockaddr *)&addrin;

	ASSERT(pSock != NULL && nPort > 0 && nMax > 0);
	
	memset(&addrin, 0, sizeof(addrin));
	/* 协议地址组包 */
	addrin.sin_family = AF_INET;			/* 协议名 */
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);	/* 自动分配地址 */
	addrin.sin_port = htons(nPort);			/* 端口号 */
	ASSERT((*pSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);	/* 创建TCP套接字描述符 */
	if (VERIFY(bind(*pSock, paddr, sizeof(addrin)) >= 0) &&	/* 命名套接字 */
	    VERIFY(listen(*pSock, nMax) >= 0))		/* 套接字进入侦听状态 */
		return 0;				/* 准备成功，正确返回 */
	VERIFY(close(*pSock) == 0);			/* 准备失败，关闭套接字描述符 */
	return 1;
}

int AcceptSock(int * pSock, int nSock)
{
	struct sockaddr_in addrin;
	int lSize;
	ASSERT(pSock != NULL && nSock > 0);
	while (1)
	{
		lSize = sizeof(addrin);
		memset(&addrin, 0, sizeof(addrin));
		/* 阻塞接收客户端连接请求，并创建新的套接字描述符 */
		if ((*pSock = accept(nSock, (struct sockaddr *)&addrin, &lSize)) > 0)
			return 0;
		/* 调用accept过程中接收到到信号，调用中断 */
		else if (errno == EINTR)	continue;
		else  	ASSERT(0);
	}
}

int ConnectSock(int *pSock, int nPort, char * pAddr)
{
	struct sockaddr_in addrin;
	long lAddr;
	int nSock;
	
	ASSERT(pSock != NULL && nPort > 0 && pAddr != NULL);
	ASSERT((nSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);	/* 创建TCP套接字描述符 */
	/* 协议地址组包 */
	memset(&addrin, 0, sizeof(addrin));
	addrin.sin_family = AF_INET;			/* 协议地址类型 */
	addrin.sin_addr.s_addr = inet_addr(pAddr);	/* 字符串IP地址转换为网络字节顺序 */
	addrin.sin_port = htons(nPort);			/* 端口号转网络字节顺序 */
	if (VERIFY(connect(nSock, (struct sockaddr *)&addrin, sizeof(addrin)) >= 0))
	{	/* 连接成功，返回套接字描述符 */
		*pSock = nSock;
		return 0;
	}
	/* 连接失败，关闭套接字描述符 */
	close(nSock);
	return 1;
}


int LocateRemoteAddr(int nSock, char *pAddr)
{
	struct sockaddr_in addrin;
	int lSize;
	ASSERT(nSock > 0 && pAddr != NULL);
	memset(&addrin, 0, sizeof(addrin));
	lSize = sizeof(addrin);
	ASSERT(getpeername(nSock, (struct sockaddr*)&addrin, &lSize) >= 0);	/* 获取对方套接字协议地址信息 */
	strcpy(pAddr, (char *)inet_ntoa(addrin.sin_addr));		/* 转换套接字地址信息为以点分隔的字符串形式 */
	return 0;
}

int LocateNativeAddr(int nSock, char *pAddr)
{
	struct sockaddr_in addrin;
	int lSize;
	ASSERT(nSock > 0 && pAddr != NULL);
	memset(&addrin, 0, sizeof(addrin));
	lSize = sizeof(addrin);
	ASSERT(getsockname(nSock, (struct sockaddr*)&addrin, &lSize) >= 0);	/* 获取本地主机套接字协议地址信息 */
	strcpy(pAddr, (char *)inet_ntoa(addrin.sin_addr));		/* 转换套接字地址信息为以点分隔的字符串形式 */
	return 0;
}

