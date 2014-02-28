#include <comlib.h>


int SetFileLen(int nFile, char *szMsg, int nSize)
{
	char szMsgSize[4 + 1];
	memset(szMsgSize, 0, sizeof(szMsgSize));
	sprintf(szMsgSize, "%04d", nSize);			/* 报文长度组包 */
	if (VERIFY(WriteFile(nFile, szMsgSize, 4) == 0) && VERIFY(WriteFile(nFile, szMsg, nSize) == 0))	/* 分两次分别输出报文长度和内容，在某些应用中也可以一次性输出 */
		return 0;
	return 1;
}

int GetFileLen(int nFile, char *szMsg, int *nSize)
{
	char szMsgSize[4 + 1];
	int nLen = 4, nLen1;
	memset(szMsgSize, 0, sizeof(szMsgSize));		/* 读取4字节报文 */
	ASSERT(ReadFile(nFile, szMsgSize, &nLen) == 0 && nLen == 4)
	nLen = nLen1 = atoi(szMsgSize);					/* 转化为后续报文长度 */
													/* 读取后续报文 */
	ASSERT(ReadFile(nFile, szMsg, &nLen) == 0 && nLen == nLen1);
	*nSize = nLen;									/* 回传后续报文长度 */
	return 0;
}

int bShutdown = 0;
int main()
{
	int nListenSock = 0, nSock = 0;
	int nPort;
	pid_t _pid_tChild;
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "PORT", &nPort, STRINT) == 0);													/* 读取套接字侦听端口号 */
	ASSERT(InitServer() == 0)							/* 进程转后台运行 */
	ASSERT(CreateSock(&nListenSock, nPort, 8) == 0)	/* 创建侦听套接字 */
	PrintLog(stdout, "htpservice: service is activated.");
	while (!bShutdown)
	{													/* 创建连接套接字 */
		if (!VERIFY(AcceptSock(&nSock, nListenSock) == 0)) continue;
		VERIFY((_pid_tChild = fork()) >= 0);			/* 创建子进程 */
		if (_pid_tChild > 0)
		{
		    close(nSock);						/* 父进程关闭连接套接字 */
		    nSock = 0;
		    continue;
		}
		break;
	}
	close(nListenSock);							/* 子进程关闭侦听套接字 */
	nListenSock = 0;
	if (VERIFY(SetAuth(nSock) == 0))  ChildExec(nSock);			/* 子服务 */
	close(nSock);								/* 服务结束，关闭连接套接字 */
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
	/* 读取授权报文的用户名和密码 */
	ASSERT(GetFileLen(nFile, szUser, &nSize)==0);   
	stuStr.nAmount = 2;				/* 数据域总数 */
	stuStr.nFlag = 1;				/* 固定分隔 */
	stuStr.filedlen = nCeLen;		/* 每个域的长度 */
	stuStr.nCompartlen = 1;			/* 分隔符号的长度 */
	strcpy(stuStr.szCompart, "|");	/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;		/* 指向每个域首的指针 */
	strrespre(szBuf, &stuStr);		/* 解析字符串报文 */
	if (strresvalue(szBuf, stuStr, 0, szUser, STRSTR) != 0 || 
	    strresvalue(szBuf, stuStr, 1, szPass, STRSTR) != 0)
	{
		ASSERT(SetFileLen(nFile, szError, strlen(szError)) == 0);
		return 3;
	}
	TrimString(szUser);								/* 清除用户名中的空格 */
	TrimString(szPass);								/* 清除密码中的空格 */
	/* 从配置文件中读取用户及密码 */
	if (GetConfigValue("./config.ini","USER",szUser,szPass1,STRSTR)==0 &&
	     strcmp(szPass, TrimString(szPass1)) != 0 )
	{	/* 判断用户密码是否正确 */
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
	ASSERT(GetFileLen(nFile, szMsg, &nSize) == 0);   /* 读取远程调用申请报文 */
	if (strncmp(szMsg, "pipe", 4) == 0)			/* 判断报文格式是否正确 */
	{
		p = TrimString(szMsg + strlen("pipe"));	/* p指向服务宣称名称 */
		/* 以下转化客户端申请服务 */
		memset(szCmd, 0, sizeof(szCmd));		
		if (!VERIFY(GetConfigValue("./config.ini", "SERVER", p, szCmd, STRSTR) == 0))	/*读取配置文件信息 */
		{											/* 错误提示 */
				strcpy(szLine, "[E]:ERROR COMMAND!");
				nSize = strlen(szLine);
				ASSERT(SetFileLen(nFile, szLine, nSize) == 0);
				return 1;
		}
		p = szCmd;			/* p指向服务真实命令行 */
		PrintLog(stderr, p);
		if ((pfile = popen(p, "r")) == NULL)		/* 以管道形式运行进程 */
		{											/* 错误提示 */
				strcpy(szLine, "[E]:pipe failed!");
				nSize = strlen(szLine);
				ASSERT(SetFileLen(nFile, szLine, nSize) == 0);
		}
		else
		{									/* 读取shell进程的每一行输出 */
			memset(szLine, 0, sizeof(szLine));		
			while (fgets(szLine, sizeof(szLine), pfile) != NULL)
			{
				nSize = strlen(szLine);
				PrintLog(stderr, "【%d】%s", nSize, szLine);
				/* 将shell进程的输出转发到客户端 */
				if (!VERIFY(SetFileLen(nFile, szLine, nSize) == 0)) break;
				memset(szLine, 0, sizeof(szLine));
        		}
        		pclose(pfile);					/* 关闭管道 */
		}
	}    
	return 0;
}
