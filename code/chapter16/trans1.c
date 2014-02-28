#include <comlib.h>
struct desktop			/* ���ǰ̨�ṹ */
{
	char szIP[20];
	int nPort;
};
struct desktop top[] = 		/* ���ǰ̨�б� */
{
	{"127.0.0.1", 10000},
	{"127.0.0.1", 10001}
};

int main(int argc, char *argv[])
{
	int nSock, nSize, i;
	char szBuf[1024];
	if (CreateUdpSock(&nSock, 8888) != 0) return 1;	/* ��������UDP�׽��� */
	while (1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		nSize = sizeof(szBuf);
		RecvMsgByUdp(nSock, szBuf, &nSize);	/* �����������̼����Ϣ */
		fprintf(stderr, "Get[%s]\n", szBuf);
		for (i=0; i<sizeof(top)/sizeof(struct desktop); i++)
		{
			/* ���ͽ��յ�����Ϣ��ÿһ��ǰ̨���� */
			fprintf(stderr, "Send [%s][%d]\n", top[i].szIP, top[i].nPort);
			SendMsgByUdp(szBuf, strlen(szBuf), top[i].szIP, top[i].nPort);
		}
	}
}
