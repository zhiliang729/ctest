#include <unistd.h>
#include <stdio.h>
void main()
{
	pid_t pid1;
	if ((pid1 = fork()) == 0)	/* 第一次fork */
	{
		printf("child[%d]\n", getpid());
		if ((pid1 = fork()) == 0)	/* 第二次fork */
		{
			printf("child[%d]\n", getpid());
			sleep(20);
			exit(0);		/* 第二次创建的子进程退出 */
		}
		exit(0);			/* 第一次创建的子进程退出 */
	}
	waitpid(pid1, 0, 0);
	printf("parent[%d]\n", getpid());
	sleep(60);
}

