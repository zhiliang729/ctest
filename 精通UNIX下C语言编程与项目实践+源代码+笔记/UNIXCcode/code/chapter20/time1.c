#include <comlib.h>
typedef struct stuSimTimer
{
	char szName[20];				/* ��ʱ������ */
	int  nInterval;					/* ��ʱ�����ȡֵΪ0ʱ��ʾȡ����ʱ */
	time_t nLast;					/* ��һ�ζ�ʱ��ϵͳʱ�� */
	int  nRemain;					/* ʣ�ඨʱʱ�� */
	int (*pfunc)(char *);			/* ����ָ�룬ָ��Ķ�ʱ������ִ�д��� */
} SIMTIMER;
typedef SIMTIMER * PSIMTIMER;
int func1(char *p)							/* ģ�ⶨʱ����Ӧ���� */
{
	PrintLog(stdout, "Now Timer : %s", p);
	return 0;
}
#define MAXTIMER 20
SIMTIMER sTimer[MAXTIMER] =					/* ģ�ⶨʱ������ */
{
	{"TIME1", 10, 0, 10, func1},			/* ��ʱ����TIMER1������ʱ10�� */
	{"TIMER2", 4, 0, 4, func1}				/* ��ʱ����TIMER2������ʱ4�� */
};
int nTopTimer = 2;									/* �ܹ�ģ��������ʱ�� */
int main()
{
	int i;
	while(1)										/* ѭ����ʱ */
	{	
		sleep(1);				
		/* ģ�ⶨʱ���㷨 */
		for (i=0; i<nTopTimer; i++)					/* ����ģ�ⶨʱ������ */
		{								/* ����ʱ���ж�ģ�ⶨʱ���Ƿ�ʱ���� */
			if(time(NULL) - sTimer[i].nLast >= sTimer[i].nInterval)
			{						
				sTimer[i].pfunc(sTimer[i].szName);	/* ִ��ģ�ⶨʱ��Ӧ���� */
				sTimer[i]. nLast = time(NULL); 		/* ����ģ�ⶨʱ�� */
			}
		}
	}
}
