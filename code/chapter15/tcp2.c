#include <comlib.h>
#define ASSURE(b) if(!b) \
	{\
		close(nSock);\
		return 0;\
	}
int main()
{
	int nSock, nSock1;		/* ����SOCKET������ */
	char szAddr[30];
	/* �����˿ں�Ϊ9001�������׽���*/
	if (CreateSock(&nSock, 9001, 9) != 0) return 0;	
	/* ���տͻ����������룬���������׽���nSock1 */
	ASSURE(AcceptSock(&nSock1, nSock) == 0);
	/* ��ȡ�ͻ��˵�IP��ַ��Ϣ */
	ASSURE(LocateRemoteAddr(nSock1, szAddr) == 0);
	fprintf(stderr, "ip=%s\n", szAddr);
	close(nSock1);			/* �ر������׽��� */
	close(nSock);			/* �ر������׽��� */
}	
