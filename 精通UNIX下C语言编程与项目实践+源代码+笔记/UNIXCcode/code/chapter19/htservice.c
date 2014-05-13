#include <comlib.h>


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

int bShutdown = 0;
int main()
{
	int nListenSock = 0, nSock = 0;
	int nPort;
	pid_t _pid_tChild;
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "PORT", &nPort, STRINT) == 0);													/* ��ȡ�׽��������˿ں� */
	ASSERT(InitServer() == 0)							/* ����ת��̨���� */
	ASSERT(CreateSock(&nListenSock, nPort, 8) == 0)	/* ���������׽��� */
	PrintLog(stdout, "htpservice: service is activated.");
	while (!bShutdown)
	{													/* ���������׽��� */
		if (!VERIFY(AcceptSock(&nSock, nListenSock) == 0)) continue;
		VERIFY((_pid_tChild = fork()) >= 0);			/* �����ӽ��� */
		if (_pid_tChild > 0)
		{
		    close(nSock);						/* �����̹ر������׽��� */
		    nSock = 0;
		    continue;
		}
		break;
	}
	close(nListenSock);							/* �ӽ��̹ر������׽��� */
	nListenSock = 0;
	ChildExec(nSock);							/* �ӷ��� */
	close(nSock);								/* ����������ر������׽��� */
	return 0;
}


int ChildExec(int nFile)
{
	char szMsg[4096 + 1], szMsgSize[4 + 1], szLine[8192 + 1], *p;
	int  nSize, nMsgSize;
	FILE *pfile;
	memset(szMsg, 0, sizeof(szMsg));
	ASSERT(GetFileLen(nFile, szMsg, &nSize) == 0);   /* ��ȡԶ�̵������뱨�� */
	if (strncmp(szMsg, "pipe", 4) == 0)			/* �жϱ��ĸ�ʽ�Ƿ���ȷ */
	{
		p = TrimString(szMsg + strlen("pipe"));	/* pָ��shell������ */
		PrintLog(stderr, p);
		/* ���Ͻ����ͻ��������ģ���pָ��ͻ�������Զ�̵��õ�shell������ */
		if ((pfile = popen(p, "r")) == NULL)		/* �Թܵ���ʽ���н��� */
		{											/* ������ʾ */
				strcpy(szLine, "[E]:pipe failed!");
				nSize = strlen(szLine);
				ASSERT(SetFileLen(nFile, szLine, nSize) == 0);
		}
		else
		{									/* ��ȡshell���̵�ÿһ����� */
			memset(szLine, 0, sizeof(szLine));		
			while (fgets(szLine, sizeof(szLine), pfile) != NULL)
			{
				nSize = strlen(szLine);
				PrintLog(stderr, "��%d��%s", nSize, szLine);
				/* ��shell���̵����ת�����ͻ��� */
				if (!VERIFY(SetFileLen(nFile, szLine, nSize) == 0)) break;
				memset(szLine, 0, sizeof(szLine));
        		}
        		pclose(pfile);					/* �رչܵ� */
		}
	}    
	return 0;
}


