#include <comlib.h>
typedef struct stuSimTimer
{
	char szName[20];				/* 定时器名称 */
	int  nInterval;					/* 定时间隔，取值为0时表示取消定时 */
	time_t nLast;					/* 上一次定时的系统时间 */
	int  nRemain;					/* 剩余定时时间 */
	int (*pfunc)(char *);			/* 函数指针，指向的定时到达后的执行代码 */
} SIMTIMER;
typedef SIMTIMER * PSIMTIMER;
int func1(char *p)					/* 模拟定时器响应函数 */
{
	PrintLog(stdout, "Now Timer : %s", p);
	return 0;
}
#define MAXTIMER 20
SIMTIMER sTimer[MAXTIMER] =					/* 模拟定时器数组 */
{
	{"TIME1", 10, 0, 10, func1},			/* 定时器“TIMER1”，定时10秒 */
	{"TIMER2", 4, 0, 4, func1}				/* 定时器“TIMER2”，定时4秒 */
};
int nTopTimer = 2;							/* 总共模拟两个定时器 */
void timefunc(int sig)						/* 基本定时器响应函数 */
{
	int i;
	/* 模拟定时器算法 */
	for (i=0; i<nTopTimer; i++)						/* 遍历模拟定时器数组 */
	{
		sTimer[i].nRemain--;						/* 定时剩余时间减1 */
		if (sTimer[i].nRemain <= 0)					/* 模拟定时器定时到达 */
		{
			sTimer[i].pfunc(sTimer[i].szName);		/* 执行模拟定时响应函数 */
			sTimer[i].nRemain = sTimer[i].nInterval; 	/* 重置模拟定时器 */
		}
	}
	signal(SIGALRM, timefunc);						/* 再次设置捕获定时信号 */
	alarm(1);										/* 基本定时器再次定时 */
}
int main()
{
	signal(SIGALRM, timefunc);
	alarm(1);										/* 基本定时器首次定时 */
	PrintLog(stdout, "---Begin---");
	while(1)								/* 主进程保持运行、不退出的状态 */
	{
		sleep(100);
	}
}
