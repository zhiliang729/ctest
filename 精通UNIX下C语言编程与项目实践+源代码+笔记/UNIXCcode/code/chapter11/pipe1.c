#include <unistd.h>
#include <stdio.h>
void main()
{
	int fildes[2];
	pid_t pid;
	int i, j;
	char buf[256];
	if (pipe(fildes) < 0)	/* �����ܵ� */
	{
		fprintf(stderr, "pipe error!\n");
		return;
	}
	if ((pid = fork()) < 0)	/* �����ӽ��� */
	{
		fprintf(stderr, "fork error!\n");
		return;
	}
	if (pid == 0)		/* �ӽ��� */
	{
		close(fildes[1]);
		memset(buf, 0, sizeof(buf));
		j = read(fildes[0], buf, sizeof(buf));
		fprintf(stderr, "[child] buf=[%s]len[%d]\n", buf, j);
		return;
	}
	/* ������ */
	close(fildes[0]);
	write(fildes[1], "hello!", strlen("hello!"));
	write(fildes[1], "world!", strlen("world!"));
}
