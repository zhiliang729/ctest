#include <comlib.h>

char buf2[]=			/* ��װHTTPЭ��GET������ */
"GET /toupiaoceshi.asp?name=������ HTTP/1.0\r\n"
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
	fprintf(stderr, "------------ͶƱ��ʼ---------------\n");
	ASSERT(ConnectSock(&nSock, 80, "122.51.64.66") == 0);		/* ����TCP���� */
	WriteFile(nSock, buf2, strlen(buf2));				/* ����GET���� */
	fprintf(stderr, buf2);
	memset(buf, 0, sizeof(buf));
	recv(nSock, buf, sizeof(buf), 0);				/* ����GET���� */
	fprintf(stderr, buf);
	close(nSock);							/* �ͷ�TCP���� */
	fprintf(stderr, "------------ͶƱ����---------------\n");
	
}

