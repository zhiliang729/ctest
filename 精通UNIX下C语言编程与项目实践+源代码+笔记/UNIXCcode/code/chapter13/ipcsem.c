#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>

#define VERIFYERR(a, b)  \
	if (a) fprintf(stderr, "%s failed.\n", b);  \
	else fprintf(stderr, "%s success.\n", b);  

void main(int argc, char *argv[])
{
	int semid, index, i;
	struct semid_ds ds;	
	unsigned short array[100];
	if (argc != 4) return;
	semid = atoi(argv[1]);
	index = atoi(argv[2]);

	if (argv[3][0] == 'c')
	{	/* �����ź��� */
		VERIFYERR(semget(semid, index, 0666|IPC_CREAT|IPC_EXCL) < 0, "Create sem");
	}
	else if (argv[3][0] == 'd')
	{	/* ɾ����Ϣ���� */
		VERIFYERR(semctl(semid, 0, IPC_RMID, NULL) < 0, "Delete sem");

	}	
	else if (argv[3][0] == 'v')
	{	/* �鿴�ź�����Ϣ */
		fprintf(stderr, "T     ID      INDEX       SEMVAL  SEMPID      SEMNCNT    SEMZCNT\n");
		fprintf(stderr, "s %6d %6d %10d %10d %10d %10d\n",
			semid, 					/* ��ʶ�� */
			index,					/* �ź������ */
			semctl(semid, index, GETVAL), 		/* �ź���ֵ */
			semctl(semid, index, GETPID), 		/* ������ʽ���ID */
			semctl(semid, index, GETNCNT), 		/* P���������� */
			semctl(semid, index, GETZCNT));		/* Z���������� */
	}
	else if (argv[3][0] == 'a')
	{	/* �鿴�ź�����Ϣ */
		ds.sem_nsems = 0;
		VERIFYERR(semctl(semid, 0, IPC_STAT, &ds) != 0, "Get Sem Stat");
		VERIFYERR(semctl(semid, 0, GETALL, array)!= 0, "Get Sem All");
		for (i=0; i<ds.sem_nsems; i++)
		{
			fprintf(stderr, "sem no[%d]  [%d]\n", i, array[i]);
		}
	}
	else 
	{
		VERIFYERR(semctl(semid, index, SETVAL, atoi(argv[3])) != 0, "Set Sem Val");
	}		
}
