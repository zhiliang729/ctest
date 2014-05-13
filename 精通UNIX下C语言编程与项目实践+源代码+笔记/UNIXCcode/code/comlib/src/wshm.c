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
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");		/* ��������ź��� */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* ������򿪹����ڴ� */
	
	VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "Shmat");
	VERIFYERR(AllocMemo(shmid, semid, 0, &index) != 0, "AllocMemo");	/* �����ڴ��¼ */
	PrintLog(stderr, "AllMemo index =[%d]", index);				/* ��ӡ��¼��� */
	printf("Please Input Data:");
	scanf("%s", buf);
	strcpy(GetMemoAddr(paddr, index), buf);					/* д������ */
	printf("Input Data End.\n");
	shmdt(paddr);
}


