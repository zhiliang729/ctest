#include <comlib.h>
#include <sys/shm.h>
#include <setjmp.h>

int CreateMemo(int shmid, int index, int size)
{
	char *pc;
	int *pd;
	if ((pc = shmat(shmid, NULL, 0)) == NULL) return -1;
	pd = (int *) pc;
	pd[0] = index;		/* 共享内存数据区划分的记录块数 */
	pd[1] = 0;		/* 当前已经使用的记录块数 */
	pd[2] = 0;		/* 最近一次分配的记录编号 */
	pd[3] = size;		/* 每个记录块的长度 */
	memset(pc+sizeof(int)*4, 0, size);	/* 重置MAP区 */
	shmdt(pc);
	return 0;	
}

int AllocMemoExt(char *pc, int *index)
{
	int nSize, nUsed, nOff, i;
	int *pd = (int *)pc;
	nSize = pd[0];			/* 记录块数 */
	nUsed = pd[1];			/* 已分配数 */
	nOff = pd[2];			/* 最近分配序号 */
	if (nUsed >= nSize) return -1;
	pc = pc + sizeof(int)*4;	/* MAP区 */
	for (i=0; i<nSize; i++)		/* 从nOff处查找空闲记录 */
	{
		if ((pc[nOff] & 0xff) == 0) break;
		nOff = (nOff+1) % nSize;
	}
	if (i >=  nSize)  return -2;
	pc[nOff] = 1;			/* 更改记录空闲标记 */
	pd[1]++;			/* 已分配数加1 */
	pd[2] = nOff;			/* 最近分配记录序号 */
	*index = nOff;
	return(0);
}

int AllocMemo(int shmid, int semid, int semindex, int *index)
{
	char *paddr;
	int ret;
	if ((paddr = shmat(shmid, NULL, 0)) == NULL) return -2;
	if (SEMP(semid, semindex) != 0)		/* 信号P操作，申请操作共享内存资源 */
	{
		shmdt(paddr);
		return -3;
	}
	ret = AllocMemoExt(paddr, index);	/* 分配记录 */
	SEMV(semid, semindex);			/* 信号V操作，释放操作共享内存资源 */
	shmdt(paddr);				/* 取消共享内存映射 */
	return(ret);
}

int FreeMemo(int shmid, int semid, int semindex, int index)
{
	char *paddr, *pc;
	int *pd;
	if ((paddr = shmat(shmid, NULL, 0)) == NULL) return -2;
	if (SEMP(semid, semindex) != 0)		/* 信号P操作，申请操作共享内存资源 */
 	{
		shmdt(paddr);
		return -3;
	}
	pd = (int *)paddr;
	pc = paddr + sizeof(int)*4;			/* MAP区首地址 */
	if (index >=0  && index < pd[0])
	{						/* 释放记录 */
		pc[index] = 0;				/* 更改记录空闲标记 */
		pd[1]--;				/* 已分配数减1 */
	}
	SEMV(semid, semindex);			/* 信号V操作，释放操作共享内存资源 */
	shmdt(paddr);				/* 取消共享内存映射 */
	return(0);
}

char *GetMemoAddr(char *paddr, int index)
{
	int *pd = (int *)paddr;
	int nSize, nUsed;
	if (index < 0 || index >= pd[0]) return NULL;
	return (paddr + 
		sizeof(int)*4 + 		/* 信息区，4个整型 */
		pd[0]* sizeof(char) + 		/* MAP区，pd[0]个字节 */
		pd[3] * index); 		/* 数据区，每个记录pd[3]字节 */
}
