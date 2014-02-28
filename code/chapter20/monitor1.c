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
	ASSERT(GetConfigValue("./config.ini", "DLL", "NUMBER", &nNumber, STRINT) == 0);													/* ��ȡ��ش���ĸ��� */
	for (i=0; i<nNumber; i++)
	{
		sprintf(buf, "DLL%d", i+1);					/* ��ش����� */
		ASSERT(GetConfigValue("./config.ini", buf, "NAME", sTimer[i].szHost, STRSTR) == 0);						/* ����������� */
		ASSERT(GetConfigValue("./config.ini", buf, "INTERVAL", &(sTimer[i].nInterval), STRINT) == 0);					/* ��ش���ִ�м�� */
		ASSERT(GetConfigValue("./config.ini", buf, "DLL", sTimer[i].szFile, STRSTR) == 0);											/* ��ش�������DLL���� */
		ASSERT(GetConfigValue("./config.ini", buf, "DLLFUNC", sTimer[i].szFunc, STRSTR) == 0);						/* ��ش��뺯������ */
		ASSERT(GetConfigValue("./config.ini", buf, "FAST", &(sTimer[i].nFast), STRINT) == 0);						/* ���ٵ��ñ�־ */
		sTimer[i].nLast = 0;
		sTimer[i].nRemain = sTimer[i].nInterval;
		sTimer[i].pHandle = NULL;				
		sTimer[i].pFunc = NULL;
		if (sTimer[i].nFast == 1)
		{											/* �򿪶�̬��*/
			sTimer[i].pHandle = dlopen(sTimer[i].szFile, RTLD_NOW); 	
			if (sTimer[i].pHandle) 	
				sTimer[i].pFunc = (PFUNC) dlsym(sTimer[i].pHandle, sTimer[i].szFunc);										/* ��ȡ��ش����ַ */
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
								/* ����UDP�׽��������� */
    	ASSERT((nSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);	
	memset(&addrin, 0, sizeof(struct sockaddr));
	addrin.sin_family = AF_INET;					/* Э���� */
	addrin.sin_addr.s_addr = inet_addr(szAddr);	/* ���շ��ĵ�ַ */
	addrin.sin_port = htons(nPort);					/* ���շ��ķ���˿ں� */
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
										/* �򿪶�̬��*/
	if ((pHandle = dlopen(szFile, RTLD_NOW)) == 0)	return 2;
										/* ��ȡ�⺯����ַ */
	if ((pFunc = (PFUNC) dlsym(pHandle, szFunc)) != 0)		
		pFunc(szHost); 					/* ��ȡ�⺯������ɹ���ִ�п⺯�� */
	dlclose(pHandle); 					/* �رն�̬�� */
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
	ASSERT(GetConfig() == 0);			/* ��ȡ��ش�����Ϣ */
	ASSERT(GetPort() == 0);				/* ����UDP�׽��֣�����ض��� */
	ASSERT(InitServer() == 0);			/* ��������ת��ִ̨�� */
	while(1)							/* ѭ����ʱ */
	{	
		sleep(1);						/* ������ʱ1����  */
		/* ģ�ⶨʱ���㷨 */
		for (i=0; i<nTopTimer; i++)		/* ����ģ�ⶨʱ������ */
		{								/* ����ʱ���ж�ģ�ⶨʱ���Ƿ�ʱ���� */
			if(time(NULL) - sTimer[i].nLast >= sTimer[i].nInterval)
			{						
				FuncAlarm(sTimer[i]);	/* ִ��ģ�ⶨʱ��Ӧ���� */
				fflush(stdout);			/* ˢ�±�׼��� */
				sTimer[i]. nLast = time(NULL); 		/* ����ģ�ⶨʱ�� */
			}
		}
	}
}

