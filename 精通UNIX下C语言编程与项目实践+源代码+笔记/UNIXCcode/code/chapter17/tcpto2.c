#include <comlib.h>	
#include  <setjmp.h>
static int nTimeOut = 0;		/* �� ���峬ʱ��־���� */
jmp_buf env;				/* �� ������ת�ṹ */
void OnTimeout(int nSignal)
{
	signal(nSignal, SIG_IGN); 	/* ��ʱһ�κ�ͺ����ź�SIGALARM����ֹѭ����ʱ */
	nTimeOut = 1;				/* ���ó�ʱ��ʶΪ���ѳ�ʱ�� */
	longjmp(env, 1);        		/* �����ת */
	return;						/* ����ʵ�����Ѿ�����ִ���� */
}
/* ��ʽΪ tcpto1 IP PORT */
int main(int argc, char *argv[])
{
	int nSock = -1, ret;
	if (argc != 3) return 1;
	nTimeOut = 0; 
	setjmp(env);
	if (nTimeOut == 1) printf("Connect Timeout.\n"); 	/* ��ʱ���� */
	else
	{
		signal(SIGALRM, OnTimeout);		/* �� �����ź�SIGALRM */
		alarm(10);						/* �� ���Ͷ�ʱ���ź�SIGALRM */
		ret = ConnectSock(&nSock, atoi(argv[2]), argv[1]);	/* �� ִ�к��� */
		alarm(0);						/* �� ȡ����ʱ�� */
		signal(SIGALRM, SIG_IGN);		/* �� �����ź�SIGALRM */
		if (ret == 0) printf("Connect Success.\n");	/* �ǳ�ʱ����*/
		else printf("Connect Error!\n");
	}
	if (nSock != -1) close(nSock);						/* �ر��׽������� */
	return 0;
}
