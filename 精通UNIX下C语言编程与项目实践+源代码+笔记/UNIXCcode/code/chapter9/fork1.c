#include<unistd.h>
#include<stdio.h>
int main()
{
	pid_t pid;
	int i;
	/* Óï¾ä¢Ù */
	fprintf(stderr, "Process Begin. pid=[%d], i=[%d]\n", getpid(), i); 
	if ((pid = fork()) > 0)
	{
		/* Óï¾ä¢Ú */
		fprintf(stderr, "Parent pid=[%d], child pid=[%d]\n", getpid(), pid);			
	}
	else if (pid == 0)
	{
		/* Óï¾ä¢Û */
		fprintf(stderr, "Child pid=[%d]\n", getpid());
		i++;
	}
	else 	fprintf(stderr, "Fork failed.\n");
	/* Óï¾ä¢Ü*/
	fprintf(stderr, "Process end. pid=[%d], i=[%d]\n", getpid(), i);
}
