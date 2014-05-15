#include <comlib.h>
#include <signal.h>
#include <sys/wait.h>
void SigChild(int nSignal)
{
	pid_t pid_t;
	int nState;
	while ((pid_t = waitpid(-1, &nState, WNOHANG)) > 0) ;
	signal(SIGCLD, SigChild);
}
int InitServer()
{
	pid_t pid1;
	ASSERT((pid1 = fork()) >= 0);	/* 创建子进程 */
	if (pid1 != 0)					/* 父进程失败退出，子进程托管 */
	{
		sleep(1);
		exit(0);
	}
	ASSERT(setsid() >= 0);			/* 子进程脱离终端 */
	umask(0);						/* 清除文件创建掩码 */
	signal(SIGINT, SIG_IGN);		/* 忽略SIGINT信号 */
	signal(SIGCLD, SigChild);		/* 处理SIGCLD信号，预防僵死进程 */
	return 0;
}
