#include <unistd.h>
#include <stdio.h>
void main()
{
	int fildes[2];
	pid_t pid;
	int i, j;
	char buf[256];
	if (pipe(fildes) < 0 || (pid = fork()) < 0)	/* 创建管道和子进程 */
	{
		fprintf(stderr, "error!\n");
		return;
	}
	if (pid == 0)		/* 子进程 */
	{
		close(fildes[1]);
		dup2(fildes[0], 0);
		close(fildes[0]);
		gets(buf);
		fprintf(stderr, "child:[%s]\n", buf);
		return;
	}
	/* 父进程 */
	close(fildes[0]);
	dup2(fildes[1], 1);
	close(fildes[1]);
	puts("Hello!");
}
