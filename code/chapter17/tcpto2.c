#include <comlib.h>	
#include  <setjmp.h>
static int nTimeOut = 0;		/* ① 定义超时标志变量 */
jmp_buf env;				/* ① 定义跳转结构 */
void OnTimeout(int nSignal)
{
	signal(nSignal, SIG_IGN); 	/* 超时一次后就忽略信号SIGALARM，防止循环超时 */
	nTimeOut = 1;				/* 设置超时标识为“已超时” */
	longjmp(env, 1);        		/* 语句跳转 */
	return;						/* 本句实际上已经不会执行了 */
}
/* 格式为 tcpto1 IP PORT */
int main(int argc, char *argv[])
{
	int nSock = -1, ret;
	if (argc != 3) return 1;
	nTimeOut = 0; 
	setjmp(env);
	if (nTimeOut == 1) printf("Connect Timeout.\n"); 	/* 超时处理 */
	else
	{
		signal(SIGALRM, OnTimeout);		/* ③ 捕获信号SIGALRM */
		alarm(10);						/* ③ 发送定时器信号SIGALRM */
		ret = ConnectSock(&nSock, atoi(argv[2]), argv[1]);	/* ④ 执行函数 */
		alarm(0);						/* ⑤ 取消定时器 */
		signal(SIGALRM, SIG_IGN);		/* ⑤ 忽略信号SIGALRM */
		if (ret == 0) printf("Connect Success.\n");	/* 非超时处理*/
		else printf("Connect Error!\n");
	}
	if (nSock != -1) close(nSock);						/* 关闭套接字连接 */
	return 0;
}
