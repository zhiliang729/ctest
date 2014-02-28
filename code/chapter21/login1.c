#include <comlib.h>
typedef struct  stuMONITORDESK
{
	char szIp[20];				/* 显示端IP地址 */
	int  nPort;				/* 显示端服务端口号 */
	char szUser[10];			/* 为显示端分配的用户名称 */
	char szPass[20];			/* 显示端用户的密码 */
	int  nActive;				/* 签到标志，0－未签到；1－已签到 */
	int  nRegister[10];			/* 注册标志组 */
}MONITORDESK;
typedef MONITORDESK * PMONITORDESK;
#define MAXDESK 20						
int *pTopDesk;
PMONITORDESK pDesk;

char *pAddr;
int GetShm()
{
	int nShmkey, nShmid;
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "SHM", &nShmkey, STRINT) == 0);													/* 获取共享内存关键字 */
													/* 打开共享内存 */
	ASSERT((nShmid = shmget(nShmkey, 8096, 0666|IPC_CREAT)) >= 0); 
	ASSERT ((pAddr = shmat(nShmid, NULL, 0)) != NULL);	/* 映射共享内存 */
	pTopDesk = (int *)pAddr;							/* 总用户数量地址 */
	pDesk = (PMONITORDESK) (pAddr + sizeof(int));		/* 用户信息地址 */
	return 0;
}


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

int GetValue(char *buf, char *user, char *pass, int *type, int *port)
{
	char *nCeAddr[4];
	int nCeLen[4];
	STRRESVAR stuStr;
	/* 解析固定分隔字符串报文 */
	stuStr.nAmount = 4;						/* 数据域总数 */
	stuStr.nFlag = 1;						/* 固定分隔 */
	stuStr.filedlen = nCeLen;				/* 每个域的长度 */
	stuStr.nCompartlen = 1;					/* 分隔符号的长度 */
	strcpy(stuStr.szCompart, "|");			/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;				/* 指向每个域首的指针 */
	strrespre(buf, &stuStr);				/* 解析字符串报文 */
	ASSERT(strresvalue(buf, stuStr, 0, user, STRSTR) == 0);	/* 用户名 */
	ASSERT(strresvalue(buf, stuStr, 1, pass, STRSTR) == 0);	/* 密码 */
	ASSERT(strresvalue(buf, stuStr, 2, type, STRINT) == 0);	/* 操作 */
	ASSERT(strresvalue(buf, stuStr, 3, port, STRINT) == 0);	/* 端口 */
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
	ASSERT(GetFileLen(nSock, szMsg, &nSize) == 0);		/* 接收报文 */
														/* 解析报文 */
	ASSERT(GetValue(szMsg, szUser, szPass, &nType, &nPort) == 0);
	ASSERT(LocateRemoteAddr(nSock, szIp) == 0);		/* 远程IP */
	PrintLog(stderr, "%s:%s:%s:%d:%d",szUser, szPass, szIp,  nPort, nType);
	for (i=0; i<*pTopDesk; i++)							/* 查找用户信息 */
	{
		if (strcmp(pDesk[i].szUser, szUser) == 0 &&	/* 用户名称一致 */
		    strcmp(pDesk[i].szPass, szPass) == 0)		/* 用户密码一致 */
		{
			strcpy(pDesk[i].szIp, szIp);			/* 更新远程IP地址信息 */
			pDesk[i].nPort = nPort;					/* 更新远程服务端口号 */
			pDesk[i].nActive = nType;				/* 更新显示端签到标志 */
			SetFileLen(nSock, "OK", 2);				/* 返回成功应答报文 */
			return 0;								/* 结束查找 */
		}
	}
	SetFileLen(nSock, "ERROR", 5);					/* 发送应答失败报文 */
	return 0;
}


int main(int argc, char *argv[])
{
	int nLisSock, nSock, nPort;		
	pid_t nChild;	
	ASSERT(GetShm() == 0);							/* 打开或创建共享内存 */
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "TCPPORT", &nPort, STRINT) == 0);											/* 获取侦听套接字端口号 */
	ASSERT(CreateSock(&nLisSock, nPort, 10) == 0);	/* 创建侦听套接字 */
	ASSERT(InitServer() == 0);						/* 进程转后台运行 */
	while (1)
	{	
		ASSERT(AcceptSock(&nSock, nLisSock) == 0); /* 创建连接套接字 */
		VERIFY((nChild = fork()) >= 0);				/* 创建子进程 */
		if (nChild == 0) break;						/* 子进程转到子进程代码 */
		close(nSock);								/* 父进程关闭连接套接字 */
	}
	/* ---------------------------子进程------------------------------- */
	close(nLisSock);								/* 子进程关闭侦听套接字 */
	GetMsg(nSock);									/* 子进程处理函数 */
	close(nSock); 									/* 子进程关闭连接套接字 */
	return 0;
}


