#include <comlib.h>

#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.[%s]\n", b, strerror(errno));  \
		return;\
	}

void main(int argc, char **argv)
{
	int shmid, semid, index;
	char buf[1024], *paddr;
	//���ź���
	if (argc != 2) return;				/* �����¼��� */
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");	/* ��������ź��� */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* ������򿪹����ڴ� */
	index = atoi(argv[1]);				/* ��ȡ��¼��� */
	VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "Shmat");
	PrintLog(stderr, "Read Memo index =[%d]", index);
	printf("Data: %s \n", GetMemoAddr(paddr, index));	/* �����¼���� */
	FreeMemo(shmid, semid, 0, index);		/* �ͷ��ڴ��¼ */
	shmdt(paddr);					/* �ͷ��ڴ�ӳ�� */
}


