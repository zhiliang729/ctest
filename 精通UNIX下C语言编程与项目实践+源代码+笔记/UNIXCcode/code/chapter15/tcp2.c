#include <comlib.h>
#define ASSURE(b) if(!b) \
	{\
		close(nSock);\
		return 0;\
	}
int main()
{
	int nSock, nSock1;		/* 定义SOCKET描述符 */
	char szAddr[30];
	/* 创建端口号为9001的侦听套接字*/
	if (CreateSock(&nSock, 9001, 9) != 0) return 0;	
	/* 接收客户端连接申请，创建连接套接字nSock1 */
	ASSURE(AcceptSock(&nSock1, nSock) == 0);
	/* 获取客户端的IP地址信息 */
	ASSURE(LocateRemoteAddr(nSock1, szAddr) == 0);
	fprintf(stderr, "ip=%s\n", szAddr);
	close(nSock1);			/* 关闭侦听套接字 */
	close(nSock);			/* 关闭连接套接字 */
}	
