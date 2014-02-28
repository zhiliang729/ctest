#include <comlib.h>


char szHtml1[]="\n<html>\n<head>\n<title>精通Unix下C语言编程</title>\n</head>\n<body>\n";
char szHtml2[]="</body>\n</html>";
int main()
{
	int nSock, nSock1;
	pid_t nChild;	
	char buf[2048], szAddr[30];
	memset(buf, 0, sizeof(buf));
	ASSERT(CreateSock(&nSock, 9001, 9) == 0);
	while(1)
	{
		ASSERT(AcceptSock(&nSock1, nSock) == 0);
		nChild = fork();
		if (nChild == 0) 
		{
			break;
		}
		close(nSock1);
	}
	close(nSock);
	memset(szAddr, 0, sizeof(szAddr));
	LocateRemoteAddr(nSock1, szAddr);
	recv(nSock1, buf, sizeof(buf), 0);
	PrintLog(stderr, buf);
	sprintf(buf, "HTTP/1.1 200 OK\n%s%s%s", szHtml1, szAddr, szHtml2);
	send(nSock1, buf, strlen(buf), 0);
	PrintLog(stderr, buf);
	close(nSock1);
}	
