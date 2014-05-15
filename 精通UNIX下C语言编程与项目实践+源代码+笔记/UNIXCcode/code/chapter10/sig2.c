#include <signal.h>
#include <stdio.h>
int usr1 = 0, usr2 = 0;
void func(int);
void main()
{
	signal(SIGUSR1, func);
	signal(SIGUSR2, func);
	for(;;) sleep(1);
}
void func(int sig)
{
	if (sig == SIGUSR1) usr1++;
	if (sig == SIGUSR2) usr2++;
	fprintf(stderr, "SIGUSR1[%d]; SIGUSR2[%d]\n", usr1, usr2);
	signal(SIGUSR1, func);
	signal(SIGUSR2, func);
}

