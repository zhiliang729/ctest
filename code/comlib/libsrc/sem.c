/*
 * 操作信号量的函数库，包括超时和不超时的。
 */
#include <comlib.h>
#include <sys/sem.h>
#include <setjmp.h>

typedef union semun{
	int val;
	struct semid_ds *Buf;
	unsigned short *array;
}SEMUN;

jmp_buf _jmp_bufReturn;

static int nTimeoutFlag = 0;
static void OnTimeout(int nSignal)
{
    signal(nSignal, SIG_IGN);
    nTimeoutFlag = 1;
    longjmp(_jmp_bufReturn, 1);
}

int Semop(int nSid, int nIndex , int nVal)
{
	struct sembuf sb;
	sb.sem_num = nIndex;
	sb.sem_op = nVal;
	sb.sem_flg = sb.sem_flg & ~SEM_UNDO;
	sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;

	if (!VERIFY(semop(nSid, &sb, 1) == 0))
	{
		if (errno == EINTR) return 1;
		return -1;
	}			
	return 0;
}

int SemopNowait(int nSid, int nIndex , int nVal)
{
	struct sembuf sb;
	sb.sem_num = nIndex;
	sb.sem_op = nVal;
	sb.sem_flg = sb.sem_flg & ~SEM_UNDO;
	sb.sem_flg = sb.sem_flg | ~IPC_NOWAIT;

	if (!VERIFY(semop(nSid, &sb, 1) == 0))
	{
		if (errno == EINTR) return 1;
		if (errno == EAGAIN) return 2;			
		return -1;
	}			
	return 0;
}
int SemopTime(int nSid, int nIndex, int nVal, int nTimeOut)
{
	nTimeoutFlag = 0;
	alarm(nTimeOut);
	signal(SIGALRM, OnTimeout);
	setjmp(_jmp_bufReturn);
	if (nTimeoutFlag != 1)
	{
		return(Semop(nSid, nIndex , nVal));
	}
	alarm(0);
	if (nTimeoutFlag != 0) return 3;
	return 0;
}


