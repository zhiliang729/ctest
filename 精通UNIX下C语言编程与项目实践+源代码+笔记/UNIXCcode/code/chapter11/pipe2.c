#include <unistd.h>
#include <stdio.h>
void WriteG(int fd, char *str, int len)
{
	char buf[255];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s", str);
	write(fd, buf, len);	
}
char *ReadG(int fd, int len)
{
	char buf[255];
	memset(buf, 0, sizeof(buf));
	read(fd, buf, len);
	return(buf);
}
void WriteC(int fd, char *str)
{
	char buf[255];
	sprintf(buf, "%04d%s", strlen(str), str);
	write(fd, buf, strlen(buf));
}	
char *ReadC(int fd)
{
	char buf[255];
	int i, j;
	memset(buf, 0, sizeof(buf));
	j = read(fd, buf, 4);
	i = atoi(buf);
	j = read(fd, buf, i);
	return(buf);
}
void main()
{
	int fildes1[2], fildes2[2];
	pid_t pid;
	char buf[255];
	if (pipe(fildes1) < 0 || pipe(fildes2) < 0)	/* 创建管道 */
	{
		fprintf(stderr, "pipe error!\n");
		return;
	}
	if ((pid = fork()) < 0)	/* 创建子进程 */
	{
		fprintf(stderr, "fork error!\n");
		return;
	}
	if (pid == 0)		/* 子进程 */
	{
		close(fildes1[1]);
		close(fildes2[0]);
		strcpy(buf, ReadG(fildes1[0], 10));
		fprintf(stderr, "[child] buf=[%s]\n", buf);
		WriteC(fildes2[1], buf);
		strcpy(buf, ReadG(fildes1[0], 10));
		fprintf(stderr, "[child] buf=[%s]\n", buf);
		WriteC(fildes2[1], buf);
		return;
	}
	/* 父进程 */
	close(fildes1[0]);
	close(fildes2[1]);
	WriteG(fildes1[1], "hello!", 10);
	WriteG(fildes1[1], "world!", 10);
	fprintf(stderr, "[father] buf=[%s]\n", ReadC(fildes2[0]));
	fprintf(stderr, "[father] buf=[%s]\n", ReadC(fildes2[0]));
}
