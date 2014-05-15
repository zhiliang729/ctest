#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int timeout = 0;
jmp_buf env;
void timefunc(int sig)				/* ��ʱ�¼����� */
{
	timeout = 1;
	longjmp(env, 1);
}
void main ()
{
	char c;
	signal(SIGALRM, timefunc);  	/* ����ʱ�ź� */
	setjmp(env);
	if (timeout == 0)				/* �������� */
	{
		alarm(3);				/* ��ʱ3�� */
		read(0, &c, 1); 			/* ��ȡ�ַ� */
		alarm(0);				/* ȡ����ʱ */
		printf("%d\n", c);			/* ��ӡ���� */
	}
	else 						/* ��ʱ���� */
		printf("timeout\n");			/* ��ӡ��ʱ */
}

