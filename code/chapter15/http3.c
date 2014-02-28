#include <comlib.h>
char szHtml1[]=	"<html>\n<head>\n<title>精通Unix下C语言编程</title>\n</head>\n<body>\n<b>精通Unix下C语言编程与项目实战</b><br><b>作者：朱云翔，胡平</b><br><b>您的IP是";
char szHtml2[]="\n</b></body>\n</html>\n";
int main()
{
	int nSock, nSock1;
	pid_t nChild;	
	char buf[2048], szAddr[30];
	memset(buf, 0, sizeof(buf));
	/* 创建端口号9001的侦听套节字 */
	ASSERT(CreateSock(&nSock, 9001, 9) == 0);
	while(1)
	{
		/* 创建连接套接字 */
		ASSERT(AcceptSock(&nSock1, nSock) == 0);
		/* 创建子进程 */
		nChild = fork();	
		if (nChild == 0) 
		{
			break;
		}
		close(nSock1);	/* 父进程关闭连接套接字 */
	}
	/* ----------------子进程-------------------- */
	close(nSock);
	memset(szAddr, 0, sizeof(szAddr));
	/* 获取远程IP地址 */
	LocateRemoteAddr(nSock1, szAddr);
	/* 接收HTTP协议报文 */
	recv(nSock1, buf, sizeof(buf), 0);
	fprintf(stderr, buf);
	/* 组建应答包 */
	sprintf(buf, "HTTP/1.1 200 OK\n%s%s%s", szHtml1, szAddr, szHtml2);
	/* 发送HTTP协议应答报文 */
	send(nSock1, buf, strlen(buf), 0);
	fprintf(stderr, buf);
	/* 中断套接字连接 */
	close(nSock1);
}	
