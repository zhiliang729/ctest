#include <stdio.h>
#include <unistd.h>
void main()
{
	pid_t i, j;
	int status;
	if ((i = fork()) == 0)
	{
		fprintf(stderr, "child begin. pid=[%d]\n", getpid());
		sleep(40);
		fprintf(stderr, "child end. pid=[%d]\n", getpid());
		exit(10);
	}
	j = wait(&status);
	fprintf(stderr, "child pid=[%d], exit pid[%d], status[%d]\n", i, j, status);
}

