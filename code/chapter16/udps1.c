#include <comlib.h>
int main()
{
	int nSock, nSize;
	char szBuf[256];
	CreateUdpSock(&nSock, 9999);/* 创建端口号为9999的UDP服务描述符 */
	while (RecvMsgByUdp(nSock, szBuf, &nSize) == 0)/* 循环接收UDP报文 */
	{
		printf("Recv UDP Data:[%d][%s]\n", strlen(szBuf), szBuf);
	}
	close(nSock);/* 关闭UDP套接字描述符 */
}
