#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>

#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.\n", b);  \
		return;\
	}\
	else \
		fprintf(stderr, "%s success.\n", b);  
void main()
{
	int semid;
	struct sembuf sb;
	/* ���ź��� */	
	VERIFYERR((semid = semget(1000, 2, 0666)) < 0, "Open sem 1000"); 

	/* ��������ִ�� */
	sb.sem_num = 0;		/* �ź�������1000�ĵ�0���ź��� */
	sb.sem_op = -1;		/* P���� */
	sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;
	VERIFYERR(semop(semid, &sb, 1) != 0, "P sem 1000:0");	
	/* ������Ʒ */
	fprintf(stderr, "[%d]producing...\n", getpid());
	sleep(1);
	fprintf(stderr, "[%d]produced\n", getpid());
	/* �ύ��Ʒ */
	sb.sem_num = 1;		/* �ź�������1000�ĵ�1���ź��� */
	sb.sem_op = 1;		/* V���� */
	sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;
	VERIFYERR(semop(semid, &sb, 1) != 0, "V sem 1000:0");
}
