#include <comlib.h>
char szHtml1[]=	"<html>\n<head>\n<title>��ͨUnix��C���Ա��</title>\n</head>\n<body>\n<b>��ͨUnix��C���Ա������Ŀʵս</b><br><b>���ߣ������裬��ƽ</b><br><b>����IP��";
char szHtml2[]="\n</b></body>\n</html>\n";
int main()
{
	int nSock, nSock1;
	pid_t nChild;	
	char buf[2048], szAddr[30];
	memset(buf, 0, sizeof(buf));
	/* �����˿ں�9001�������׽��� */
	ASSERT(CreateSock(&nSock, 9001, 9) == 0);
	while(1)
	{
		/* ���������׽��� */
		ASSERT(AcceptSock(&nSock1, nSock) == 0);
		/* �����ӽ��� */
		nChild = fork();	
		if (nChild == 0) 
		{
			break;
		}
		close(nSock1);	/* �����̹ر������׽��� */
	}
	/* ----------------�ӽ���-------------------- */
	close(nSock);
	memset(szAddr, 0, sizeof(szAddr));
	/* ��ȡԶ��IP��ַ */
	LocateRemoteAddr(nSock1, szAddr);
	/* ����HTTPЭ�鱨�� */
	recv(nSock1, buf, sizeof(buf), 0);
	fprintf(stderr, buf);
	/* �齨Ӧ��� */
	sprintf(buf, "HTTP/1.1 200 OK\n%s%s%s", szHtml1, szAddr, szHtml2);
	/* ����HTTPЭ��Ӧ���� */
	send(nSock1, buf, strlen(buf), 0);
	fprintf(stderr, buf);
	/* �ж��׽������� */
	close(nSock1);
}	
