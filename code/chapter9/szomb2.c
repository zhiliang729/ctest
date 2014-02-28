#include <unistd.h>
#include <stdio.h>
void main()
{
	pid_t pid1;
	if ((pid1 = fork()) == 0)	/* ��һ��fork */
	{
		printf("child[%d]\n", getpid());
		if ((pid1 = fork()) == 0)	/* �ڶ���fork */
		{
			printf("child[%d]\n", getpid());
			sleep(20);
			exit(0);		/* �ڶ��δ������ӽ����˳� */
		}
		exit(0);			/* ��һ�δ������ӽ����˳� */
	}
	waitpid(pid1, 0, 0);
	printf("parent[%d]\n", getpid());
	sleep(60);
}

