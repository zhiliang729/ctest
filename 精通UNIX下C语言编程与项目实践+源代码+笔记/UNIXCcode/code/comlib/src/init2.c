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
	//打开信号量
	VERIFYERR((semid = semget(0x1234, 1, 0666|IPC_CREAT)) < 0, "Open(Create) Sem");		/* 创建或打开信号量 */
	VERIFYERR((shmid = shmget(0x1234, 20*1024, 0666|IPC_CREAT))<0, "Open(Create) Shm");	/* 创建或打开共享内存 */

	if (argc == 2 && argv[1][0] == 'c')		/* 格式化共享内存 */
	{
		semctl(semid, 0, SETVAL, 1);
		CreateMemo(shmid, 10, 512);
	}		
	if ((pd = shmat(shmid, NULL, 0)) == NULL) return;
	PrintLog(stderr, "nVal=[%d]max[%d]used[%d]cur[%d]size[%d]", 
		semctl(semid, 0, GETVAL), 		/* 信号量取值 */
		pd[0],					/* 共享内存中最大可分配记录块数 */ 
		pd[1], 					/* 共享内存中已经分配的记录块数 */
		pd[2], 					/* 共享内存中最近一次分配的记录块编号 */
		pd[3]);					/* 共享内存中一个记录块的字节大小 */
	shmdt(pd);
}


