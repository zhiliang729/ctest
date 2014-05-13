#include <comlib.h>
#include <dlfcn.h>
typedef int (*PFUNC)(char*);		
typedef struct stuSimTimer
{
	char szHost[20];				/* 主机名称 */
	int  nInterval;					/* 定时间隔，取值为0时表示取消定时 */
	time_t nLast;					/* 上一次定时的系统时间 */
	int  nRemain;					/* 剩余定时时间 */
	char szFile[50];				/* 文件名称 */
	char szFunc[30];				/* 监控函数名称 */
	int  nFast;					/* 快速调用标志 */
	void *pHandle;					/* 动态库句柄 */
	PFUNC pFunc;					/* 监控函数 */
} SIMTIMER;
typedef SIMTIMER * PSIMTIMER;

#define MAXTIMER 20
SIMTIMER sTimer[MAXTIMER];
int nTopTimer = 0;	

int GetConfig()
{
	int nNumber, i;
	char buf[20];
	ASSERT(GetConfigValue("./config.ini", "DLL", "NUMBER", &nNumber, STRINT) == 0);													/* 获取监控代码的个数 */
	for (i=0; i<nNumber; i++)
	{
		sprintf(buf, "DLL%d", i+1);					/* 监控代码项 */
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", sTimer[i].szHost, STRSTR) == 0);						/* 监控主机名称 */
		ASSERT(GetConfigValue("./config.ini", buf, "INTERVAL", &(sTimer[i].nInterval), STRINT) == 0);					/* 监控代码执行间隔 */
		ASSERT(GetConfigValue("./config.ini", buf, "DLL", sTimer[i].szFile, STRSTR) == 0);											/* 监控代码所在DLL名称 */
		ASSERT(GetConfigValue("./config.ini", buf, "DLLFUNC", sTimer[i].szFunc, STRSTR) == 0);						/* 监控代码函数名称 */
		ASSERT(GetConfigValue("./config.ini", buf, "FAST", &(sTimer[i].nFast), STRINT) == 0);						/* 快速调用标志 */
		sTimer[i].nLast = 0;
		sTimer[i].nRemain = sTimer[i].nInterval;
		sTimer[i].pHandle = NULL;				
		sTimer[i].pFunc = NULL;
		if (sTimer[i].nFast == 1)
		{											/* 打开动态库*/
			sTimer[i].pHandle = dlopen(sTimer[i].szFile, RTLD_NOW); 	
			if (sTimer[i].pHandle) 	
				sTimer[i].pFunc = (PFUNC) dlsym(sTimer[i].pHandle, sTimer[i].szFunc);										/* 获取监控代码地址 */
		}
	}
	nTopTimer = i;
	return 0;
}	
	

int GetPort()
{
	char szAddr[30];
	int nPort;
	int nSock;
	struct sockaddr_in addrin;
	ASSERT(GetConfigValue("./config.ini", "REMOTE", "IP", szAddr, STRSTR) == 0);
	ASSERT(GetConfigValue("./config.ini", "REMOTE", "PORT", &nPort, STRINT) == 0);
								/* 创建UDP套接字描述符 */
    	ASSERT((nSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);	
	memset(&addrin, 0, sizeof(struct sockaddr));
	addrin.sin_family = AF_INET;					/* 协议名 */
	addrin.sin_addr.s_addr = inet_addr(szAddr);	/* 接收方的地址 */
	addrin.sin_port = htons(nPort);					/* 接收方的服务端口号 */
	ASSERT(connect(nSock, (struct sockaddr *)&addrin, sizeof(addrin)) >= 0);
	dup2(nSock, 1);
	close(nSock);
	return 0;
}

int ExecDll(char *szHost, char *szFile ,char *szFunc)
{
	void *pHandle;
	int *p;
	PFUNC pFunc;
										/* 打开动态库*/
	if ((pHandle = dlopen(szFile, RTLD_NOW)) == 0)	return 2;
										/* 获取库函数地址 */
	if ((pFunc = (PFUNC) dlsym(pHandle, szFunc)) != 0)		
		pFunc(szHost); 					/* 获取库函数对象成功，执行库函数 */
	dlclose(pHandle); 					/* 关闭动态库 */
	return 0;
}


int FuncAlarm(SIMTIMER uTimer)
{
	if (uTimer.nFast == 1) 
	{
		if (uTimer.pFunc) uTimer.pFunc(uTimer.szHost);
	}
	else ExecDll(uTimer.szHost, uTimer.szFile, uTimer.szFunc);
	
	return 0;
}

int main()
{
	int i;
	ASSERT(GetConfig() == 0);			/* 获取监控代码信息 */
	ASSERT(GetPort() == 0);				/* 创建UDP套接字，输出重定向 */
	ASSERT(InitServer() == 0);			/* 主机进程转后台执行 */
	while(1)							/* 循环定时 */
	{	
		sleep(1);						/* 基本定时1秒钟  */
		/* 模拟定时器算法 */
		for (i=0; i<nTopTimer; i++)		/* 遍历模拟定时器数组 */
		{								/* 绝对时间判断模拟定时器是否定时到达 */
			if(time(NULL) - sTimer[i].nLast >= sTimer[i].nInterval)
			{						
				FuncAlarm(sTimer[i]);	/* 执行模拟定时响应函数 */
				fflush(stdout);			/* 刷新标准输出 */
				sTimer[i]. nLast = time(NULL); 		/* 重置模拟定时器 */
			}
		}
	}
}

