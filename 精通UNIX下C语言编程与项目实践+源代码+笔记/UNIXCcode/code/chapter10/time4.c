#include  <sys/select.h>
#include  <sys/itimer.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int n = 0;
void timefunc(int sig)						/* ��ʱ�¼����� */
{
	fprintf(stderr, "ITIMER_PROF[%d]\n", n++);
	signal(SIGPROF, timefunc);				/* ����ʱ�ź� */
}
void main()
{
	struct itimerval value;
	value.it_value.tv_sec=1;
	value.it_value.tv_usec=500000;
	value.it_interval.tv_sec=1;
	value.it_interval.tv_usec=500000;
	signal(SIGPROF, timefunc);  			/* ����ʱ�ź� */
	setitimer(ITIMER_PROF, &value, NULL);			/* ��ʱ��ʼ */
	while (1);
}
