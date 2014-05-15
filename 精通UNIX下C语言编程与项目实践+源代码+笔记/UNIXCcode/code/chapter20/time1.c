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
int func1(char *p)							/* 模拟定时器响应函数 */
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
int nTopTimer = 2;									/* 总共模拟两个定时器 */
int main()
{
	int i;
	while(1)										/* 循环定时 */
	{	
		sleep(1);				
		/* 模拟定时器算法 */
		for (i=0; i<nTopTimer; i++)					/* 遍历模拟定时器数组 */
		{								/* 绝对时间判断模拟定时器是否定时到达 */
			if(time(NULL) - sTimer[i].nLast >= sTimer[i].nInterval)
			{						
				sTimer[i].pfunc(sTimer[i].szName);	/* 执行模拟定时响应函数 */
				sTimer[i]. nLast = time(NULL); 		/* 重置模拟定时器 */
			}
		}
	}
}
