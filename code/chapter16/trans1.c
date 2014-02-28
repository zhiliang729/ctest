#include <comlib.h>
struct desktop			/* 监控前台结构 */
{
	char szIP[20];
	int nPort;
};
struct desktop top[] = 		/* 监控前台列表 */
{
	{"127.0.0.1", 10000},
	{"127.0.0.1", 10001}
};

int main(int argc, char *argv[])
{
	int nSock, nSize, i;
	char szBuf[1024];
	if (CreateUdpSock(&nSock, 8888) != 0) return 1;	/* 创建接收UDP套接字 */
	while (1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		nSize = sizeof(szBuf);
		RecvMsgByUdp(nSock, szBuf, &nSize);	/* 接收主机进程监控信息 */
		fprintf(stderr, "Get[%s]\n", szBuf);
		for (i=0; i<sizeof(top)/sizeof(struct desktop); i++)
		{
			/* 发送接收到的信息到每一个前台进程 */
			fprintf(stderr, "Send [%s][%d]\n", top[i].szIP, top[i].nPort);
			SendMsgByUdp(szBuf, strlen(szBuf), top[i].szIP, top[i].nPort);
		}
	}
}
