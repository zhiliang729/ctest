#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void childfunc(int sig)
{
	fprintf(stderr, "Get Sig\n");
	exit(6);
}
void main()
{
	pid_t pid;
	int status;
	if ((pid = fork()) < 0) exit(1);
	else if (pid == 0)	/* вс╫ЬЁл */
	{
		signal(SIGTERM, childfunc);
		sleep(30);
		exit(0);
	}
	fprintf(stderr, "Parent [%d] Fork child pid=[%d]\n", getpid(), pid);
	sleep(1);
	kill(pid, SIGTERM);
	wait(&status);
	fprintf(stderr, "Kill child pid=[%d], exit status[%d]\n", pid, status>>8);
}

