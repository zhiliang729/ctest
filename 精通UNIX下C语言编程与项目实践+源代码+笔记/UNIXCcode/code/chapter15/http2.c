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
	/* ����argv[1]��WEB����������������ת������ַ */		
	if ((host = gethostbyname(argv[1])) == NULL) 	
	{
		herror("gethostbyname");	
		return 2;
	}
	fprintf(stderr, "%s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
	/* ������ַת�ַ���IP��ַ������TCP���ӣ�WEB�˵Ķ˿ں�һ����80 */		
	ASSERT(ConnectSock(&nSock, 80, inet_ntoa(*((struct in_addr *)host->h_addr))) == 0);
	/* �齨HTTPЭ��GET����� */
	sprintf(buf, buf2, argv[2], argv[1]);
	/* ����HTTPЭ��GET������ */
	WriteFile(nSock, buf, strlen(buf));	
	fprintf(stderr, buf);
	while (i > 0)
	{	/* ���������׽���nSock�ϵ���Ϣ��ֱ�������������ղ�����ϢΪֹ */
		memset(buf, 0, sizeof(buf));
		i = recv(nSock, buf, sizeof(buf), 0);
		j = j + i;
		fprintf(stderr, buf);
	}
	/* �ж��׽������� */
	close(nSock);
	PrintLog(stderr, "%d", j);
	
}

