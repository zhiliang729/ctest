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
	//打开信号量
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");		/* 创建或打开信号量 */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* 创建或打开共享内存 */
	
	VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "Shmat");
	VERIFYERR(AllocMemo(shmid, semid, 0, &index) != 0, "AllocMemo");	/* 申请内存记录 */
	PrintLog(stderr, "AllMemo index =[%d]", index);				/* 打印记录编号 */
	printf("Please Input Data:");
	scanf("%s", buf);
	strcpy(GetMemoAddr(paddr, index), buf);					/* 写入数据 */
	printf("Input Data End.\n");
	shmdt(paddr);
}


