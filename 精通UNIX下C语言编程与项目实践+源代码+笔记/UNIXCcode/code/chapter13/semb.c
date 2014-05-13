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
	/* 打开信号量 */	
	VERIFYERR((semid = semget(1000, 2, 0666)) < 0, "Open sem 1000"); 
	/* 申请消费 */
	sb.sem_num = 1;		/* 信号量集合1000的第1个信号量 */
	sb.sem_op = -1;		/* P操作 */
	sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;
	VERIFYERR(semop(semid, &sb, 1) != 0, "P sem 1000:1");
	/* 消费产品 */
	fprintf(stderr, "[%d]consuming...\n", getpid());
	sleep(1);
	fprintf(stderr, "[%d]consumed\n", getpid());
	/* 提交生产执照 */
	sb.sem_num = 0;		/* 信号量集合1000的第0个信号量 */
	sb.sem_op = 1;		/* V操作 */
	sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;
	VERIFYERR(semop(semid, &sb, 1) != 0, "V sem 1000:0");
}
