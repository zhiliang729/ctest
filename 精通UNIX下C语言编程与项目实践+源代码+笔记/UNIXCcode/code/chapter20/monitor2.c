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
	ASSERT(GetConfigValue("./config.ini", "EXEC", "NUMBER", &nNumber, STRINT) == 0);													/* 获取监控代码的个数 */
	for (i=0; i<nNumber; i++)
	{
		sprintf(buf, "EXEC%d", i+1);					/* 监控代码项 */
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", sTimer[i].szHost, STRSTR) == 0);						/* 监控主机名称 */
		ASSERT(GetConfigValue("./config.ini", buf, "INTERVAL", &(sTimer[i].nInterval), STRINT) == 0);					/* 监控代码执行间隔 */
		ASSERT(GetConfigValue("./config.ini", buf, "FILE", sTimer[i].szFile, STRSTR) == 0);											/* 监控代码所在DLL名称 */
		sTimer[i].nLast = 0;
		sTimer[i].nRemain = 0;
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
	return 0;
}

int FuncAlarm(SIMTIMER uTimer)
{
	pid_t nChild;	
	ASSERT((nChild = fork()) >= 0);				/* 创建子进程 */
	if (nChild > 0) return 0;					/* 父进程退出 */
	/* -------------------以下是子进程-------------------- */
												/* 调用监控程序 */
	execl(uTimer.szFile, uTimer.szFile, uTimer.szHost, 0);	 
	exit(0);
}


void timefunc(int sig)						/* 基本定时器响应函数 */
{
	int i;
	/* 模拟定时器算法 */
	for (i=0; i<nTopTimer; i++)						/* 遍历模拟定时器数组 */
	{
		sTimer[i].nRemain--;						/* 定时剩余时间减1 */
		if (sTimer[i].nRemain <= 0)					/* 模拟定时器定时到达 */
		{
			FuncAlarm(sTimer[i]);	/* 执行模拟定时响应函数 */
			fflush(stdout);			/* 刷新标准输出 */
			sTimer[i].nRemain = sTimer[i].nInterval; 	/* 重置模拟定时器 */
		}
	}
	signal(SIGALRM, timefunc);						/* 再次设置捕获定时信号 */
	alarm(1);										/* 基本定时器再次定时 */
}


int main()
{
	ASSERT(GetConfig() == 0);			/* 获取监控代码信息 */
	ASSERT(GetPort() == 0);				/* 创建UDP套接字，输出重定向 */
	ASSERT(InitServer() == 0);			/* 主机进程转后台执行 */
	signal(SIGALRM, timefunc);
	ASSERT(signal(SIGCLD, SIG_IGN) != SIG_ERR);
	alarm(1);										/* 基本定时器首次定时 */
	while(1)								/* 主进程保持运行、不退出的状态 */
	{
		sleep(100);
	}
}
