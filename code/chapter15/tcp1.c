#include <comlib.h>
/* ��ʾ�� */
#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.\n", b);  \
		return 0;\
	}\
	else  fprintf(stderr, "%s success.\n", b);  
		


int main()
{
	int nSock, nSock1;		/* ����SOCKET������ */
	char buf[2048];
	/* �����˿ں�Ϊ9001�������׽���*/
	VERIFYERR(CreateSock(&nSock, 9001, 9) != 0, "Create Listen SOCKET");	
	/* ���տͻ����������룬���������׽���nSock1 */
	VERIFYERR(AcceptSock(&nSock1, nSock) != 0, "Link");	
	memset(buf, 0, sizeof(buf));
	/* ���տͻ��˷��͵�TCP������Ϣ */
	recv(nSock1, buf, sizeof(buf), 0);
	fprintf(stderr, buf);
	close(nSock1);			/* �ر������׽��� */
	close(nSock);			/* �ر������׽��� */
}	
