#include <unistd.h>
#include <stdio.h>
void main()
{
	pid_t pid1;
	if ((pid1 = fork()) == 0)
	{
		printf("child[%d]\n", getpid());
		exit(0);
	}
	/* wait(); */			/* ¢Ù */
	printf("parent[%d]\n", getpid());
	sleep(60);
}

