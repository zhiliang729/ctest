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
	if (argc != 2) return;				/* 输入记录编号 */
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");	/* 创建或打开信号量 */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* 创建或打开共享内存 */
	index = atoi(argv[1]);				/* 获取记录编号 */
	VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "Shmat");
	PrintLog(stderr, "Read Memo index =[%d]", index);
	printf("Data: %s \n", GetMemoAddr(paddr, index));	/* 输入记录数据 */
	FreeMemo(shmid, semid, 0, index);		/* 释放内存记录 */
	shmdt(paddr);					/* 释放内存映射 */
}


