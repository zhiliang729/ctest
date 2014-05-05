//
//  comlib.h
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#ifndef comlib_comlib_h
#define comlib_comlib_h


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <setjmp.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAXBUF 4096

/*日志文件路径*/
#ifndef TRACE_FILE
#define TRACE_FILE "/Users/zhangliang/Desktop/trace_file"
#endif

/*最大最小*/
#ifndef min
#define min(A, B) ((A) < (B) ? (A) : (B))
#endif

#ifndef max
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif

#define VERIFY(b)        Verify((errno = 0) || (b), NULL, __FILE__, __LINE__)
#define ASSERT(b)        if (VERIFY(b)) ; else return -1;
#define ASSERTEXT(b, c)	 if (VERIFY(b)) ; else return(c);

typedef struct structTIMESTRU
{
    int nYear; /* From  1900 */
    int nMon;
    int nDay;
    int nHour; /* From 0 to 23 */
    int nMin;
    int nSec;
} TIMESTRU;
typedef TIMESTRU * PTIMESTRU;

TIMESTRU GetTime();
TIMESTRU GetOffsetTime(TIMESTRU timestru, int nOffsetSec);

/*socket TCP 连接*/
int CreateSock(int *pSock, int nPort, int nMaxConnect);
int AcceptSock(int *pSock, int nSock);
int ConnectSock(int *pSock, int nPort, char * pszAddr);
int LocateRemoteAddr(int nSock, char* szAddr);
int LocateNativeAddr(int nSock, char* szAddr);
/*socket UDP连接*/
int CreateUdpSock(int * pnSock, int nPort);
int SendMsgByUdp(void * pMsg, int nSize, char * szAddr, int nPort);
int RecvMsgByUdp(int nFile, void * pData, ssize_t * pnSize);

#define BYTESIZE 2048

/*格式化日志输出函数*/
int PrintLog(FILE * pfile, const char * pformat, ...);
int PrintTraceLog(const char * pformat, ...);
int PrintHexLog(FILE * pfile, void * pData, int nSize);
int PrintTraceHexLog(void * pData, int nSize);
int Verify(int bStatus, const char * szBuf, const char * szFile, int nLine);

/*阻塞方式的读写功能库*/
int ReadFile(int nFile, void * pData, ssize_t * pSize);
int WriteFile(int nFile, void* pData, ssize_t nSize);
/*定时读写功能库，函数在收到足够输入或写入足够输出后返回，如果无输入输出则定时完成后函数返回*/
int ReadFileExt(int nFile, void * pData, ssize_t * pnSize, int nTimeout);
int WriteFileExt(int nFile, void* pData, ssize_t * nSize, int nTimeout);


/*阻塞方式消息发送与接收*/
int ReadMsg(int nPid, void * pText, ssize_t * pSize, long * pType);
int WriteMsg(int nPid, void * pText, int nSize, int nType);
/*定时方式消息发送与接收*/
int ReadMsgExt(int nPid, void * pText, ssize_t * pSize, long * pType, int nTimeout);
int WriteMsgExt(int nPid, void * pText, int nSize, int nType, int nTimeout);
//判断队列中是否存在消息
int CheckMsgData(int nPid, long * pType);

/*信号量阻塞操作  信号量的阻塞P操作（信号值减少）、释放V操作(信号值增加)和Z（信号值判断）操作功能*/
int Semop(int nSid, int nIndex , int nVal);
/*非阻塞操作  以非阻塞方式执行P（信号量减少）、V（信号量增加）和Z（信号值判断）操作功能。 成功返回0 ，信号中断时返回1，不满足P操作或Z操作条件时立即返回2，否则返回其他值*/
int SemopNowait(int nSid, int nIndex , int nVal);
/*定时操作  对标识号为nSid，序号为nIndex的信号量执行P、V或Z操作。当操作被阻塞时，进程被挂起，直到阻塞条件消失或者阻塞nTimeout秒后函数返回*/
int SemopTime(int nSid, int nIndex, int nVal, int nTimeOut);
//通常情况下，我们对信号量的操作仅限于信号值减小1，增加1或判断信号值是否等于0，所以宏定义如下：
//减少1 P操作  阻塞
#define SEMP(nSid, nIndex) Semop(nSid, nIndex, -1)
//增加1 V操作 阻塞
#define SEMV(nSid, nIndex) Semop(nSid, nIndex, 1)
//比较操作 Z操作 阻塞
#define SEMZ(nSid, nIndex) Semop(nSid, nIndex, 0)

//减少1 P操作  非阻塞
#define SEMPNW(nSid, nIndex) SemopNoWait(nSid, nIndex, -1)
//增加1 V操作 非阻塞
#define SEMVNW(nSid, nIndex) SemopNoWait(nSid, nIndex, 1)
//比较操作 Z操作 非阻塞
#define SEMZNW(nSid, nIndex) SemopNoWait(nSid, nIndex, 0)

//减少1 P操作  定时
#define SEMPT(nSid, nIndex, nTime) SemopTime(nSid, nIndex, -1, nTime)
//增加1 V操作 定时
#define SEMVT(nSid, nIndex, nTime) SemopTime(nSid, nIndex, 1, nTime)
//比较操作 Z操作 定时
#define SEMZT(nSid, nIndex, nTime) SemopTime(nSid, nIndex, 0, nTime)

//共享内存 1-n-n模型
int CreateMemo(int shmid, int index, int size);
int AllocMemoExt(char *pc, int *index);
int AllocMemo(int shmid, int semid, int semindex, int *index);
int FreeMemo(int shmid, int semid, int semindex, int index);
char *GetMemoAddr(char *paddr, int index);

/*守护进程生成器*/
int InitServer();


#endif
