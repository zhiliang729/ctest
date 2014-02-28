#include <comlib.h>

char buf2[]=			/* 组装HTTP协议GET请求报文 */
"GET /toupiaoceshi.asp?name=朱云翔 HTTP/1.0\r\n"
"Accept: */*\r\n"
"Accept-Language: zh-cn\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)\r\n"
"Host: www.zhiliaowang.com\r\n"
"Connection: Keep-Alive\r\n\r\n";
int main()
{
	char buf[2048];
	int nSock;
	fprintf(stderr, "------------投票开始---------------\n");
	ASSERT(ConnectSock(&nSock, 80, "122.51.64.66") == 0);		/* 建立TCP连接 */
	WriteFile(nSock, buf2, strlen(buf2));				/* 发送GET请求 */
	fprintf(stderr, buf2);
	memset(buf, 0, sizeof(buf));
	recv(nSock, buf, sizeof(buf), 0);				/* 接收GET请求 */
	fprintf(stderr, buf);
	close(nSock);							/* 释放TCP连接 */
	fprintf(stderr, "------------投票结束---------------\n");
	
}

