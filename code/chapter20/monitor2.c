#include <comlib.h>
#include <dlfcn.h>
typedef int (*PFUNC)(char*);		
typedef struct stuSimTimer
{
	char szHost[20];				/* �������� */
	int  nInterval;					/* ��ʱ�����ȡֵΪ0ʱ��ʾȡ����ʱ */
	time_t nLast;					/* ��һ�ζ�ʱ��ϵͳʱ�� */
	int  nRemain;					/* ʣ�ඨʱʱ�� */
	char szFile[50];				/* �ļ����� */
	char szFunc[30];				/* ��غ������� */
	int  nFast;					/* ���ٵ��ñ�־ */
	void *pHandle;					/* ��̬���� */
	PFUNC pFunc;					/* ��غ��� */
} SIMTIMER;
typedef SIMTIMER * PSIMTIMER;

#define MAXTIMER 20
SIMTIMER sTimer[MAXTIMER];
int nTopTimer = 0;	

int GetConfig()
{
	int nNumber, i;
	char buf[20];
	ASSERT(GetConfigValue("./config.ini", "EXEC", "NUMBER", &nNumber, STRINT) == 0);													/* ��ȡ��ش���ĸ��� */
	for (i=0; i<nNumber; i++)
	{
		sprintf(buf, "EXEC%d", i+1);					/* ��ش����� */
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", sTimer[i].szHost, STRSTR) == 0);						/* ����������� */
		ASSERT(GetConfigValue("./config.ini", buf, "INTERVAL", &(sTimer[i].nInterval), STRINT) == 0);					/* ��ش���ִ�м�� */
		ASSERT(GetConfigValue("./config.ini", buf, "FILE", sTimer[i].szFile, STRSTR) == 0);											/* ��ش�������DLL���� */
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
								/* ����UDP�׽��������� */
    	ASSERT((nSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);	
	memset(&addrin, 0, sizeof(struct sockaddr));
	addrin.sin_family = AF_INET;					/* Э���� */
	addrin.sin_addr.s_addr = inet_addr(szAddr);	/* ���շ��ĵ�ַ */
	addrin.sin_port = htons(nPort);					/* ���շ��ķ���˿ں� */
	ASSERT(connect(nSock, (struct sockaddr *)&addrin, sizeof(addrin)) >= 0);
	dup2(nSock, 1);
	return 0;
}

int FuncAlarm(SIMTIMER uTimer)
{
	pid_t nChild;	
	ASSERT((nChild = fork()) >= 0);				/* �����ӽ��� */
	if (nChild > 0) return 0;					/* �������˳� */
	/* -------------------�������ӽ���-------------------- */
												/* ���ü�س��� */
	execl(uTimer.szFile, uTimer.szFile, uTimer.szHost, 0);	 
	exit(0);
}


void timefunc(int sig)						/* ������ʱ����Ӧ���� */
{
	int i;
	/* ģ�ⶨʱ���㷨 */
	for (i=0; i<nTopTimer; i++)						/* ����ģ�ⶨʱ������ */
	{
		sTimer[i].nRemain--;						/* ��ʱʣ��ʱ���1 */
		if (sTimer[i].nRemain <= 0)					/* ģ�ⶨʱ����ʱ���� */
		{
			FuncAlarm(sTimer[i]);	/* ִ��ģ�ⶨʱ��Ӧ���� */
			fflush(stdout);			/* ˢ�±�׼��� */
			sTimer[i].nRemain = sTimer[i].nInterval; 	/* ����ģ�ⶨʱ�� */
		}
	}
	signal(SIGALRM, timefunc);						/* �ٴ����ò���ʱ�ź� */
	alarm(1);										/* ������ʱ���ٴζ�ʱ */
}


int main()
{
	ASSERT(GetConfig() == 0);			/* ��ȡ��ش�����Ϣ */
	ASSERT(GetPort() == 0);				/* ����UDP�׽��֣�����ض��� */
	ASSERT(InitServer() == 0);			/* ��������ת��ִ̨�� */
	signal(SIGALRM, timefunc);
	ASSERT(signal(SIGCLD, SIG_IGN) != SIG_ERR);
	alarm(1);										/* ������ʱ���״ζ�ʱ */
	while(1)								/* �����̱������С����˳���״̬ */
	{
		sleep(100);
	}
}
