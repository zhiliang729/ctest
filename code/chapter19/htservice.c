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
	ChildExec(nSock);							/* 子服务 */
	close(nSock);								/* 服务结束，关闭连接套接字 */
	return 0;
}


int ChildExec(int nFile)
{
	char szMsg[4096 + 1], szMsgSize[4 + 1], szLine[8192 + 1], *p;
	int  nSize, nMsgSize;
	FILE *pfile;
	memset(szMsg, 0, sizeof(szMsg));
	ASSERT(GetFileLen(nFile, szMsg, &nSize) == 0);   /* 读取远程调用申请报文 */
	if (strncmp(szMsg, "pipe", 4) == 0)			/* 判断报文格式是否正确 */
	{
		p = TrimString(szMsg + strlen("pipe"));	/* p指向shell命令行 */
		PrintLog(stderr, p);
		/* 以上解析客户端请求报文，现p指向客户端请求远程调用的shell命令行 */
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


