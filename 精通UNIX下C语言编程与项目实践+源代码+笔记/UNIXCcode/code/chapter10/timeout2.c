#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int timeout = 0;
jmp_buf env;
void timefunc(int sig)				/* 定时事件代码 */
{
	timeout = 1;
	longjmp(env, 1);
}
void main ()
{
	char c;
	signal(SIGALRM, timefunc);  	/* 捕获定时信号 */
	setjmp(env);
	if (timeout == 0)				/* 正常处理 */
	{
		alarm(3);				/* 定时3秒 */
		read(0, &c, 1); 			/* 读取字符 */
		alarm(0);				/* 取消定时 */
		printf("%d\n", c);			/* 打印输入 */
	}
	else 						/* 超时处理 */
		printf("timeout\n");			/* 打印超时 */
}

