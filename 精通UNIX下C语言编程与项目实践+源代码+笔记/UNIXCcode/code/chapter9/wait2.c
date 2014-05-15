#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void main()
{
	pid_t pid1, pid2, pid3;
	int status;
	while (1)
	{
		if ((pid1 = fork()) < 0) exit(1);
		else if (pid1 == 0)	/* 子进程1 */
		{
			sleep(30);
			exit(0);
		}
		if ((pid2 = fork()) < 0) exit(1);
		else if (pid2 == 0)	/* 子进程2 */
		{
			sleep(40);
			exit(0);
		}
		fprintf(stderr, "Parent [%d] Fork child pid=[%d][%d]\n", getpid(), pid1, pid2);
		pid3 = wait(&status);
		kill(pid1, SIGTERM);
		kill(pid2, SIGTERM);
		fprintf(stderr, "Kill child pid=[%d][%d], exit pid[%d], status[%d]\n", pid1, pid2, pid3, status);
		pid3 = wait(&status);
		sleep(1);
	}
}

