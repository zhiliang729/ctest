#include <comlib.h>
typedef struct  stuMONITORDESK
{
	char szIp[20];				/* ��ʾ��IP��ַ */
	int  nPort;				/* ��ʾ�˷���˿ں� */
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


int SetFileLen(int nFile, char *szMsg, int nSize)
{
	char szMsgSize[4 + 1];
	memset(szMsgSize, 0, sizeof(szMsgSize));
	sprintf(szMsgSize, "%04d", nSize);			/* ���ĳ������ */
	if (VERIFY(WriteFile(nFile, szMsgSize, 4) == 0) && VERIFY(WriteFile(nFile, szMsg, nSize) == 0))	/* �����ηֱ�������ĳ��Ⱥ����ݣ���ĳЩӦ����Ҳ����һ������� */
		return 0;
	return 1;
}
int GetFileLen(int nFile, char *szMsg, int *nSize)
{
	char szMsgSize[4 + 1];
	int nLen = 4, nLen1;
	memset(szMsgSize, 0, sizeof(szMsgSize));		/* ��ȡ4�ֽڱ��� */
	ASSERT(ReadFile(nFile, szMsgSize, &nLen) == 0 && nLen == 4)
	nLen = nLen1 = atoi(szMsgSize);					/* ת��Ϊ�������ĳ��� */
													/* ��ȡ�������� */
	ASSERT(ReadFile(nFile, szMsg, &nLen) == 0 && nLen == nLen1);
	*nSize = nLen;									/* �ش��������ĳ��� */
	return 0;
}

int GetValue(char *buf, char *user, char *pass, int *type, int *port)
{
	char *nCeAddr[4];
	int nCeLen[4];
	STRRESVAR stuStr;
	/* �����̶��ָ��ַ������� */
	stuStr.nAmount = 4;						/* ���������� */
	stuStr.nFlag = 1;						/* �̶��ָ� */
	stuStr.filedlen = nCeLen;				/* ÿ����ĳ��� */
	stuStr.nCompartlen = 1;					/* �ָ����ŵĳ��� */
	strcpy(stuStr.szCompart, "|");			/* �ָ��ַ��� */
	stuStr.filedaddr = nCeAddr;				/* ָ��ÿ�����׵�ָ�� */
	strrespre(buf, &stuStr);				/* �����ַ������� */
	ASSERT(strresvalue(buf, stuStr, 0, user, STRSTR) == 0);	/* �û��� */
	ASSERT(strresvalue(buf, stuStr, 1, pass, STRSTR) == 0);	/* ���� */
	ASSERT(strresvalue(buf, stuStr, 2, type, STRINT) == 0);	/* ���� */
	ASSERT(strresvalue(buf, stuStr, 3, port, STRINT) == 0);	/* �˿� */
	TrimString(user);
	TrimString(pass);
	return 0;	
}

int GetMsg(int nSock)
{
	int  nSize, nType, nPort, i;
	char szMsg[2048], szUser[20], szPass[20], szIp[30];
	nSize = sizeof(szMsg);
	memset(szMsg,  0, sizeof(szMsg));		
	memset(szUser, 0, sizeof(szUser));		
	memset(szPass, 0, sizeof(szPass));		
	memset(szIp,   0, sizeof(szIp));		
	ASSERT(GetFileLen(nSock, szMsg, &nSize) == 0);		/* ���ձ��� */
														/* �������� */
	ASSERT(GetValue(szMsg, szUser, szPass, &nType, &nPort) == 0);
	ASSERT(LocateRemoteAddr(nSock, szIp) == 0);		/* Զ��IP */
	PrintLog(stderr, "%s:%s:%s:%d:%d",szUser, szPass, szIp,  nPort, nType);
	for (i=0; i<*pTopDesk; i++)							/* �����û���Ϣ */
	{
		if (strcmp(pDesk[i].szUser, szUser) == 0 &&	/* �û�����һ�� */
		    strcmp(pDesk[i].szPass, szPass) == 0)		/* �û�����һ�� */
		{
			strcpy(pDesk[i].szIp, szIp);			/* ����Զ��IP��ַ��Ϣ */
			pDesk[i].nPort = nPort;					/* ����Զ�̷���˿ں� */
			pDesk[i].nActive = nType;				/* ������ʾ��ǩ����־ */
			SetFileLen(nSock, "OK", 2);				/* ���سɹ�Ӧ���� */
			return 0;								/* �������� */
		}
	}
	SetFileLen(nSock, "ERROR", 5);					/* ����Ӧ��ʧ�ܱ��� */
	return 0;
}


int main(int argc, char *argv[])
{
	int nLisSock, nSock, nPort;		
	pid_t nChild;	
	ASSERT(GetShm() == 0);							/* �򿪻򴴽������ڴ� */
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "TCPPORT", &nPort, STRINT) == 0);											/* ��ȡ�����׽��ֶ˿ں� */
	ASSERT(CreateSock(&nLisSock, nPort, 10) == 0);	/* ���������׽��� */
	ASSERT(InitServer() == 0);						/* ����ת��̨���� */
	while (1)
	{	
		ASSERT(AcceptSock(&nSock, nLisSock) == 0); /* ���������׽��� */
		VERIFY((nChild = fork()) >= 0);				/* �����ӽ��� */
		if (nChild == 0) break;						/* �ӽ���ת���ӽ��̴��� */
		close(nSock);								/* �����̹ر������׽��� */
	}
	/* ---------------------------�ӽ���------------------------------- */
	close(nLisSock);								/* �ӽ��̹ر������׽��� */
	GetMsg(nSock);									/* �ӽ��̴����� */
	close(nSock); 									/* �ӽ��̹ر������׽��� */
	return 0;
}


