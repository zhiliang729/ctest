#include <comlib.h>
typedef struct  stuMONITORDESK
{
	char szIp[20];				/* 显示端IP地址 */
	int  nPort;					/* 显示端服务端口号 */
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

int GetConfig()
{
	int nNumber, i;
	char buf[20];
	ASSERT(GetConfigValue("./config.ini", "USER", "NUMBER", &nNumber, STRINT) == 0);												/* 用户数量 */
	for (i=0; i<nNumber; i++)
	{
		memset(pDesk+i, 0, sizeof(MONITORDESK));									sprintf(buf, "USER%d", i+1);					/* 用户信息项 */ 
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", pDesk[i].szUser, STRSTR) == 0);												/* 用户名称 */
		ASSERT(GetConfigValue("./config.ini", buf, "PASS", pDesk[i].szPass, STRSTR) == 0);												/* 用户密码 */
		pDesk[i].nActive = 0;							/* 未签到 */
	}
	*pTopDesk = i;										/* 用户数量 */
	return 0;
}
	

int main(int argc, char *argv[])
{
	int nSock, nSize, i, nPort;
	char szBuf[8096];
	ASSERT(GetShm() == 0);						/* 打开或创建共享内存 */
	ASSERT(GetConfig() == 0);					/* 读入配置信息到共享内存中 */
	ASSERT(GetConfigValue("./config.ini", "LOCAL", "UDPPORT", &nPort, STRINT) == 0);										/* 获取UDP服务端口号 */
	ASSERT(CreateUdpSock(&nSock, nPort) == 0);	/* 创建UDP套接字 */
	ASSERT(InitServer() == 0);					/* 进程转后台服务 */
	while (1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		nSize = sizeof(szBuf);
		RecvMsgByUdp(nSock, szBuf, &nSize);		/* 接收主机进程监控信息 */
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

