#include <comlib.h>
int CreateUdpSock(int * pnSock, int nPort)
{
	struct sockaddr_in addrin;
	struct sockaddr *paddr = (struct sockaddr *)&addrin;

	ASSERT(pnSock != NULL && nPort > 0);
	memset(&addrin, 0, sizeof(addrin));
	/* 协议地址组包 */
	addrin.sin_family = AF_INET;			/* 协议名 */
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);	/* 自动分配地址 */
	addrin.sin_port = htons(nPort);			/* 端口号 */
	/* 组装系统调用socket和bind */
	ASSERT((*pnSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);	/* 创建UDP套接字描述符 */
	if (VERIFY(bind(*pnSock, paddr, sizeof(addrin)) >= 0) )	/* 命名套接字 */
		return 0;					/* 准备成功，正确返回 */
	VERIFY(close(*pnSock) == 0);				/* 准备失败，关闭套接字描述符 */
	return 1;
}


int SendMsgByUdp(void * pMsg, int nSize, char * szAddr, int nPort)
{
	int nSock;
	struct sockaddr_in addrin;
    	ASSERT((nSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);	/* 创建UDP套接字描述符 */
	memset(&addrin, 0, sizeof(struct sockaddr));
	/* 接收方的协议地址组包 */
	addrin.sin_family = AF_INET;			/* 协议名 */
	addrin.sin_addr.s_addr = inet_addr(szAddr);	/* 接收方的地址 */
	addrin.sin_port = htons(nPort);			/* 接收方的服务端口号 */
	VERIFY(sendto(nSock, pMsg, nSize, 0,		/* 发送数据到接收方 */
	             (struct sockaddr *)&addrin,
	             sizeof(addrin)) > 0);
	close (nSock);					/* 关闭套接字 */
	return 0;
}
		
int RecvMsgByUdp(int nFile, void * pData, int * pnSize) 
{
	int nSize;
	/* 接收数据，并不关心发送方协议地址 */
	ASSERT((*pnSize=recvfrom(nFile, pData, *pnSize, 0 , NULL, NULL)) > 0);
	return 0;
}

