#include <comlib.h>

char buf2[]=
"GET %s HTTP/1.0\r\n"
"Accept: */*\r\n"
"Accept-Language: zh-cn\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)\r\n"
"Host: %s\r\n"
"Connection: Keep-Alive\r\n\r\n";

int main(int argc, char *argv[])
{
	char buf[1024];
	int nSock;
	int i=9999, j=0;
	struct hostent *host;
	if (argc != 3) return 0;
	/* 参数argv[1]是WEB服务器域名，域名转主机地址 */		
	if ((host = gethostbyname(argv[1])) == NULL) 	
	{
		herror("gethostbyname");	
		return 2;
	}
	fprintf(stderr, "%s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
	/* 主机地址转字符串IP地址，建立TCP连接，WEB端的端口号一般是80 */		
	ASSERT(ConnectSock(&nSock, 80, inet_ntoa(*((struct in_addr *)host->h_addr))) == 0);
	/* 组建HTTP协议GET请求包 */
	sprintf(buf, buf2, argv[2], argv[1]);
	/* 发送HTTP协议GET请求报文 */
	WriteFile(nSock, buf, strlen(buf));	
	fprintf(stderr, buf);
	while (i > 0)
	{	/* 反复接收套接字nSock上的信息，直到发生错误或接收不到信息为止 */
		memset(buf, 0, sizeof(buf));
		i = recv(nSock, buf, sizeof(buf), 0);
		j = j + i;
		fprintf(stderr, buf);
	}
	/* 中断套接字连接 */
	close(nSock);
	PrintLog(stderr, "%d", j);
	
}

