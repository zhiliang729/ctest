#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int n = 0;
void timefunc(int sig)		/* �ӽ��̲����źź��� */
{
	fprintf(stderr, "Alarm %d\n", n++);
	signal(SIGALRM, timefunc);
	alarm(1);
}
void main()
{
	pid_t pid;
	int status;
	signal(SIGALRM, timefunc);
	alarm(1);
	while (1);
}
