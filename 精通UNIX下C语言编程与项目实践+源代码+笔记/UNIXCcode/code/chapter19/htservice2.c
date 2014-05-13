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
	if (VERIFY(SetAuth(nSock) == 0))  ChildExec(nSock);			/* �ӷ��� */
	close(nSock);								/* ����������ر������׽��� */
	return 0;
}

int SetAuth(int nFile)
{
	char szUser[100 + 1], szPass[100 + 1], szPass1[100 + 1], szBuf[200];
	char *nCeAddr[2], szError[] = "Bad User or Password!\n";
	int  nSize, nCeLen[2];
	STRRESVAR stuStr;
	memset(szBuf, 0, sizeof(szBuf));
	memset(szUser, 0, sizeof(szUser));
	memset(szPass, 0, sizeof(szPass));
	memset(szPass1, 0, sizeof(szPass1));
	/* ��ȡ��Ȩ���ĵ��û��������� */
	ASSERT(GetFileLen(nFile, szUser, &nSize)==0);   
	stuStr.nAmount = 2;				/* ���������� */
	stuStr.nFlag = 1;				/* �̶��ָ� */
	stuStr.filedlen = nCeLen;		/* ÿ����ĳ��� */
	stuStr.nCompartlen = 1;			/* �ָ����ŵĳ��� */
	strcpy(stuStr.szCompart, "|");	/* �ָ��ַ��� */
	stuStr.filedaddr = nCeAddr;		/* ָ��ÿ�����׵�ָ�� */
	strrespre(szBuf, &stuStr);		/* �����ַ������� */
	if (strresvalue(szBuf, stuStr, 0, szUser, STRSTR) != 0 || 
	    strresvalue(szBuf, stuStr, 1, szPass, STRSTR) != 0)
	{
		ASSERT(SetFileLen(nFile, szError, strlen(szError)) == 0);
		return 3;
	}
	TrimString(szUser);								/* ����û����еĿո� */
	TrimString(szPass);								/* ��������еĿո� */
	/* �������ļ��ж�ȡ�û������� */
	if (GetConfigValue("./config.ini","USER",szUser,szPass1,STRSTR)==0 &&
	     strcmp(szPass, TrimString(szPass1)) != 0 )
	{	/* �ж��û������Ƿ���ȷ */
		ASSERT(SetFileLen(nFile, szError, strlen(szError)) == 0);
		return 3;
	}
	return 0;
}


int ChildExec(int nFile)
{
	char szMsg[4096 + 1], szMsgSize[4 + 1], szLine[8192 + 1], *p;
	char szCmd[100];
	int  nSize, nMsgSize;
	FILE *pfile;
	memset(szMsg, 0, sizeof(szMsg));
	ASSERT(GetFileLen(nFile, szMsg, &nSize) == 0);   /* ��ȡԶ�̵������뱨�� */
	if (strncmp(szMsg, "pipe", 4) == 0)			/* �жϱ��ĸ�ʽ�Ƿ���ȷ */
	{
		p = TrimString(szMsg + strlen("pipe"));	/* pָ������������� */
		/* ����ת���ͻ���������� */
		memset(szCmd, 0, sizeof(szCmd));		
		if (!VERIFY(GetConfigValue("./config.ini", "SERVER", p, szCmd, STRSTR) == 0))	/*��ȡ�����ļ���Ϣ */
		{											/* ������ʾ */
				strcpy(szLine, "[E]:ERROR COMMAND!");
				nSize = strlen(szLine);
				ASSERT(SetFileLen(nFile, szLine, nSize) == 0);
				return 1;
		}
		p = szCmd;			/* pָ�������ʵ������ */
		PrintLog(stderr, p);
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
