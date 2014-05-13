#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
/*VERIFYERR(a, b)是定义宏，当a成立是，它打印失败信息，否则打印成功信息。*/
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
	int shmid;
	char *pmat = NULL;
	char buf[1024];
	int no;

	/* 打开信号量 */	
	VERIFYERR((shmid = shmget(0x1234, 10*1024, 0666|IPC_CREAT)) == -1, "Open(Create) Shm");
	/* 映射共享内存，地址存储与pmat处 */
	VERIFYERR((pmat = (char *)shmat(shmid, 0, 0)) == 0, "Link Shm");
	/* 输入更新的内存块号 */
	printf("Please Input No.(0~9):");
	scanf("%d", &no);
	VERIFYERR(no < 0 || no > 9, "Input No.");
	/* 输入更新的数据 */
	printf("Please Input Data:");
	memset(buf, 0, sizeof(buf));
	scanf("%s", buf);
	/* 更新共享内存数据 */;
	memcpy(pmat+no*1024, buf, 1024);
	/* 释放共享内存映射 */
	shmdt(pmat);
}
