#include <comlib.h>
int main()
{
	int nSock, nSize;
	char szBuf[256];
	CreateUdpSock(&nSock, 9999);/* �����˿ں�Ϊ9999��UDP���������� */
	while (RecvMsgByUdp(nSock, szBuf, &nSize) == 0)/* ѭ������UDP���� */
	{
		printf("Recv UDP Data:[%d][%s]\n", strlen(szBuf), szBuf);
	}
	close(nSock);/* �ر�UDP�׽��������� */
}
