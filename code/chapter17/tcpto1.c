#include <comlib.h>	
static int nTimeOut = 0;		/* ① 定义超时标志变量 */
void OnTimeout(int nSignal)		/* ② 信号处理函数 */
{
	signal(nSignal, SIG_IGN);	/* 超时一次后就忽略信号SIGALARM，防止循环超时 */
	nTimeOut = 1;				/* 设置超时标识为“已超时” */
	return;
}
/* 格式为 tcpto1 IP PORT */
int main(int argc, char *argv[])
{
	int nSock = -1, ret;
	if (argc != 3) return 1;
	nTimeOut = 0; 
	signal(SIGALRM, OnTimeout);		/* ③ 捕获信号SIGALRM */
	alarm(10);						/* ③ 发送定时器信号SIGALRM */
	ret = ConnectSock(&nSock, atoi(argv[2]), argv[1]);	/* ④ 执行函数 */
	alarm(0);						/* ⑤ 取消定时器 */
	signal(SIGALRM, SIG_IGN);		/* ⑤ 忽略信号SIGALRM */
	/* ⑥ 函数返回，超时判断 */
	if (nTimeOut == 1) printf("Connect Timeout.\n"); 	/* 超时处理 */
	else if (ret == 0) printf("Connect Success.\n");	/* 非超时处理*/
	else printf("Connect Error!\n");
	if (nSock != -1) close(nSock);						/* 关闭套接字连接 */
	return 0;
}
