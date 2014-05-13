#include <comlib.h>

#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.[%s]\n", b, strerror(errno));  \
		return;\
	}

void main(int argc, char **argv)
{
	int shmid, semid, *pd;
	//���ź���
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");		/* ��������ź��� */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* ������򿪹����ڴ� */

	if (argc == 2 && argv[1][0] == 'c')		/* ��ʽ�������ڴ� */
	{
		semctl(semid, 0, SETVAL, 1);
		CreateMemo(shmid, 10, 512);
	}		
	if ((pd = shmat(shmid, NULL, 0)) == NULL) return;
	PrintLog(stderr, "nVal=[%d]max[%d]used[%d]cur[%d]size[%d]", 
		semctl(semid, 0, GETVAL), 		/* �ź���ȡֵ */
		pd[0],					/* �����ڴ������ɷ����¼���� */ 
		pd[1], 					/* �����ڴ����Ѿ�����ļ�¼���� */
		pd[2], 					/* �����ڴ������һ�η���ļ�¼���� */
		pd[3]);					/* �����ڴ���һ����¼����ֽڴ�С */
	shmdt(pd);
}


