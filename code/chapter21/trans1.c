#include <comlib.h>
typedef struct  stuMONITORDESK
{
	char szIp[20];				/* ��ʾ��IP��ַ */
	int  nPort;					/* ��ʾ�˷���˿ں� */
	char szUser[10];			/* Ϊ��ʾ�˷�����û����� */
	char szPass[20];			/* ��ʾ���û������� */
	int  nActive;				/* ǩ����־��0��δǩ����1����ǩ�� */
	int  nRegister[10];			/* ע���־�� */
}MONITORDESK;
typedef MONITORDESK * PMONITORDESK;
#define MAXDESK 20						
int *pTopDesk;
PMONITORDESK pDesk;

char *pAddr;
int GetShm()
{
	int nShmkey, nShmid;
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "SHM", &nShmkey, STRINT) == 0);													/* ��ȡ�����ڴ�ؼ��� */
													/* �򿪹����ڴ� */
	ASSERT((nShmid = shmget(nShmkey, 8096, 0666|IPC_CREAT)) >= 0); 
	ASSERT ((pAddr = shmat(nShmid, NULL, 0)) != NULL);	/* ӳ�乲���ڴ� */
	pTopDesk = (int *)pAddr;							/* ���û�������ַ */
	pDesk = (PMONITORDESK) (pAddr + sizeof(int));		/* �û���Ϣ��ַ */
	return 0;
}

int GetConfig()
{
	int nNumber, i;
	char buf[20];
	ASSERT(GetConfigValue("./config.ini", "USER", "NUMBER", &nNumber, STRINT) == 0);												/* �û����� */
	for (i=0; i<nNumber; i++)
	{
		memset(pDesk+i, 0, sizeof(MONITORDESK));									sprintf(buf, "USER%d", i+1);					/* �û���Ϣ�� */ 
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", pDesk[i].szUser, STRSTR) == 0);												/* �û����� */
		ASSERT(GetConfigValue("./config.ini", buf, "PASS", pDesk[i].szPass, STRSTR) == 0);												/* �û����� */
		pDesk[i].nActive = 0;							/* δǩ�� */
	}
	*pTopDesk = i;										/* �û����� */
	return 0;
}
	

int main(int argc, char *argv[])
{
	int nSock, nSize, i, nPort;
	char szBuf[8096];
	ASSERT(GetShm() == 0);						/* �򿪻򴴽������ڴ� */
	ASSERT(GetConfig() == 0);					/* ����������Ϣ�������ڴ��� */
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "UDPPORT", &nPort, STRINT) == 0);										/* ��ȡUDP����˿ں� */
	ASSERT(CreateUdpSock(&nSock, nPort) == 0);	/* ����UDP�׽��� */
	ASSERT(InitServer() == 0);					/* ����ת��̨���� */
	while (1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		nSize = sizeof(szBuf);
		RecvMsgByUdp(nSock, szBuf, &nSize);		/* �����������̼����Ϣ */
		PrintLog(stderr, szBuf);
		for (i=0; i<*pTopDesk; i++)
		{
			if (pDesk[i].nActive == 1) 
			{
				SendMsgByUdp(szBuf, nSize, pDesk[i].szIp, pDesk[i].nPort);
			}
		}
	}
}

