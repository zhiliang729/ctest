#include <signal.h>
#include <stdio.h>
void main()
{
	signal(SIGINT, SIG_IGN);
	sleep(100);
}
