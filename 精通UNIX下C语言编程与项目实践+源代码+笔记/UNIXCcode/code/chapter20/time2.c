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
int func1(char *p)					/* ģ�ⶨʱ����Ӧ���� */
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
int nTopTimer = 2;							/* �ܹ�ģ��������ʱ�� */
void timefunc(int sig)						/* ������ʱ����Ӧ���� */
{
	int i;
	/* ģ�ⶨʱ���㷨 */
	for (i=0; i<nTopTimer; i++)						/* ����ģ�ⶨʱ������ */
	{
		sTimer[i].nRemain--;						/* ��ʱʣ��ʱ���1 */
		if (sTimer[i].nRemain <= 0)					/* ģ�ⶨʱ����ʱ���� */
		{
			sTimer[i].pfunc(sTimer[i].szName);		/* ִ��ģ�ⶨʱ��Ӧ���� */
			sTimer[i].nRemain = sTimer[i].nInterval; 	/* ����ģ�ⶨʱ�� */
		}
	}
	signal(SIGALRM, timefunc);						/* �ٴ����ò���ʱ�ź� */
	alarm(1);										/* ������ʱ���ٴζ�ʱ */
}
int main()
{
	signal(SIGALRM, timefunc);
	alarm(1);										/* ������ʱ���״ζ�ʱ */
	PrintLog(stdout, "---Begin---");
	while(1)								/* �����̱������С����˳���״̬ */
	{
		sleep(100);
	}
}
