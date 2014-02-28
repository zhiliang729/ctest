#ifndef __COM_H__
#define __COM_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <utmpx.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netdb.h>
#include <netinet/in.h>

#ifndef NULL
#define NULL 0
#endif
#define MAXBUF 4096
/*   日志输出文件 */
#ifndef TRACE_FILE
#define TRACE_FILE "./trace"
#endif
/* 简单宏函数 */
#ifndef min
#define min(A, B) ((A) < (B) ? (A) : (B))
#endif

#ifndef max
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif

#define minsize(A, B) (sizeof(A) < sizeof(B) ? sizeof(A) : sizeof(B))
#define maxsize(A, B) (sizeof(A) > sizeof(B) ? sizeof(A) : sizeof(B))

#ifndef isblank
#define isblank(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#endif

#ifndef isdigit
#define isdigit(c) ((c) <= '9' && (c) >= '0')
#endif

#ifndef isalpha
#define isalpha(c) ((c) <= 'z' && (c) >= 'a' || (c) <= 'Z' && (c) >= 'A')
#endif

#ifndef upper
#define upper(c) ((c) <= 'z' && (c) >= 'a' ? (c) - 'a' + 'A' : (c))
#endif

#ifndef lower
#define lower(c) ((c) <= 'Z' && (c) >= 'A' ? (c) - 'A' + 'a' : (c))
#endif

/* 调试功能库 */
#define VERIFY(b)        Verify((errno = 0) || (b), NULL, __FILE__, __LINE__)
#define ASSERT(b)        if (VERIFY(b)) ; else return -1;
#define ASSERTEXT(b, c)	 if (VERIFY(b)) ; else return(c);
int Verify(int bStatus, const char * pszInfo, const char * pszFile, int nLine);
int PrintLog(FILE* pfile, const char * pformat, ...);
int PrintTraceLog(const char* pformat, ...);
int PrintHexLog(FILE* pfile, void * pData, int nSize);
int PrintTraceHexLog(void * pData, int nSize);
int Verify(int bStatus, const char* szBuf, const char* szFile, int nLine);
/* 文件读写库 */
int ReadFile(int nFile, void * pData, int * pSize);
int WriteFile(int nFile, void* pData, int nSize);
int ReadFileExt(int nFile, void * pData, int pnSize, int nTimeout);
int WriteFileExt(int nFile, void* pData, int nSize, int nTimeout);
/* 消息队列库 */
int ReadMsg(int nPid, void * pText, int * pSize, int * pType);
int WriteMsg(int nPid, void * pText, int nSize, int nType);
int ReadMsgExt(int nPid, void * pText, int * pSize, int * pType, int nTimeout);
int WriteMsgExt(int nPid, void * pText, int nSize, int nType, int nTimeout);
/* 信号量库 */
int Semop(int nSid, int nIndex , int nVal);
int SemopNowait(int nSid, int nIndex , int nVal);
int SemopTime(int nSid, int nIndex, int nVal, int nTimeOut);
#define SEMP(nSid, nIndex) Semop(nSid, nIndex , -1)
#define SEMV(nSid, nIndex) Semop(nSid, nIndex , 1)
#define SEMZ(nSid, nIndex) Semop(nSid, nIndex , 0)
#define SEMPNW(nSid, nIndex) SemopNoWait(nSid, nIndex , -1)
#define SEMVNW(nSid, nIndex) SemopNoWait(nSid, nIndex , 1)
#define SEMZNW(nSid, nIndex) SemopNoWait(nSid, nIndex , 0)
#define SEMPT(nSid, nIndex, nTime) SemopTime(nSid, nIndex , -1, nTime)
#define SEMVT(nSid, nIndex, nTime) SemopTime(nSid, nIndex , 1, nTime)
#define SEMZT(nSid, nIndex, nTime) SemopTime(nSid, nIndex , 0, nTime)

/* 共享内存库 */
int CreateMemo(int shmid, int index, int size);
int AllocMemoExt(char *pc, int *index);
int AllocMemo(int shmid, int semid, int semindex, int *index);
int FreeMemo(int shmid, int semid, int semindex, int index);
char *GetMemoAddr(char *paddr, int index);

/* 后台服务 */
int InitServer();

/* 时钟库 */
typedef struct structTIMESTRU
{
    int nYear; /* From  2001 */
    int nMon;
    int nDay;
    int nHour; /* From 0 to 23 */
    int nMin;
    int nSec;
} TIMESTRU;
typedef TIMESTRU * PTIMESTRU;

TIMESTRU GetTime();
TIMESTRU GetOffsetTime(TIMESTRU timestru, int nOffsetSec);

/* TCP套接字库 */
int CreateSock(int *pSock, int nPort, int nMaxConnect);
int AcceptSock(int *pSock, int nSock);
int ConnectSock(int *pSock, int nPort, char * pszAddr);
int LocateRemoteAddr(int nSock, char* szAddr);
int LocateNativeAddr(int nSock, char* szAddr);
/* UDP套接字库 */
int CreateUdpSock(int * pnSock, int nPort);
int SendMsgByUdp(void * pMsg, int nSize, char * szAddr, int nPort);
int RecvMsgByUdp(int nFile, void * pData, int * pnSize);

/* 字符串报文库 */
typedef struct STRRESVARstu
{
	int nAmount;			/* 域总数 */
	int nFlag;				/* 0:定长域；1：分隔域 */
	int *filedlen;			/* 每个域的长度 */
	int nCompartlen;		/* 分隔符号的长度 */
	char szCompart[10];		/* 分隔字符串 */
	char **filedaddr;		/* 指向每个域首的指针 */
} STRRESVAR;
typedef STRRESVAR * PSTRRESVAR;

#define STRSTR 0
#define STRINT 1
#define STRFLOAT 2
#define STRDOUBLE 3

char * TrimString(char* szDest);
int strrespre(char *buf, PSTRRESVAR pStrstu);
int strresvalue(char *buf, STRRESVAR Strstu, int nIndex, void *pValue, int nType);
int GetConfigValue(char *szPath, char *szRoot, char *szName, void *pValue, int nType);

#endif /* __COM_H__ */


