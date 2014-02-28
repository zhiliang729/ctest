#include <comlib.h>
/* 提示宏 */
#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.\n", b);  \
		return 0;\
	}\
	else  fprintf(stderr, "%s success.\n", b);  
		


int main()
{
	int nSock, nSock1;		/* 定义SOCKET描述符 */
	char buf[2048];
	/* 创建端口号为9001的侦听套接字*/
	VERIFYERR(CreateSock(&nSock, 9001, 9) != 0, "Create Listen SOCKET");	
	/* 接收客户端连接申请，创建连接套接字nSock1 */
	VERIFYERR(AcceptSock(&nSock1, nSock) != 0, "Link");	
	memset(buf, 0, sizeof(buf));
	/* 接收客户端发送的TCP数据信息 */
	recv(nSock1, buf, sizeof(buf), 0);
	fprintf(stderr, buf);
	close(nSock1);			/* 关闭侦听套接字 */
	close(nSock);			/* 关闭连接套接字 */
}	
