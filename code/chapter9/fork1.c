#include<unistd.h>
#include<stdio.h>
int main()
{
	pid_t pid;
	int i;
	/* ���� */
	fprintf(stderr, "Process Begin. pid=[%d], i=[%d]\n", getpid(), i); 
	if ((pid = fork()) > 0)
	{
		/* ���� */
		fprintf(stderr, "Parent pid=[%d], child pid=[%d]\n", getpid(), pid);			
	}
	else if (pid == 0)
	{
		/* ���� */
		fprintf(stderr, "Child pid=[%d]\n", getpid());
		i++;
	}
	else 	fprintf(stderr, "Fork failed.\n");
	/* ����*/
	fprintf(stderr, "Process end. pid=[%d], i=[%d]\n", getpid(), i);
}
