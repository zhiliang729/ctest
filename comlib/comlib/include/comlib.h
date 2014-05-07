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

#include <pwd.h>
#include <grp.h>


#define MAXBUF 4096

/*日志文件路径*/
#ifndef TRACE_FILE
#define TRACE_FILE "/Users/*userName()/Desktop/trace_file"
#endif
static char tracefile[1024];
static char * trace_file = tracefile;
char * traceFile();

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

/*字符串报文解析结构*/
typedef struct STRRESVARstu
{
	int nAmount;			/*数据与总数----代表字符串报文所包含的数据与总数*/
	int nFlag;				/*0：定长域；1：分割域----代表字符串报文的类型。0表示固定长度字符串报文，1表示固定分隔符串报文*/
	unsigned long *filedlen;			/*每个数据域的长度----记载一个整型数组的首地址，这个数组中的每个元素分别对应字符串报文中相应数据域所占用的字节长度。对于固定长度报文，全部报文中的数据域占用的字节长度是一定的，因此数组fildlen中的元素的取值也是一定的。对于固定分割报文，数组fildlen中元素的取值随着每条字符串报文的不同而不同。filedlen仅仅是一个指针，用户必须先申请整型数组缓冲区，再把这个缓冲区地址赋值给它。*/
	int nCompartlen;		/*分割符号的长度----代表数据域之间的分割字符串的长度。比如“|”的长度为1，“|！”的长度是2.固定长度报文没有分割字符串，则此值为0*/
	char szCompart[10];		/*分割字符串----字符数组，它记载了分割在数据域之间的字符串。在固定长度无分割字符串的报文中，这个是长度为0的空字符串*/
	char **filedaddr;		/*指向每个数据域首地址的指针----这是一个指向数组的指针，这个数组中的每个元素都是一个字符指针，它们分别指向了字符串报文中每个数据域的首地址。与filedlen相同，filedaddr也仅仅是一个指针，用户必须先申请一个字符指针数组，再把数组的首地址赋值给它*/
} STRRESVAR;
typedef STRRESVAR * PSTRRESVAR;

#define STRSTR 0
#define STRINT 1
#define STRFLOAT 2
#define STRDOUBLE 3

/*填充字符串报文解析的结构*/
int strrespre(char * buf, PSTRRESVAR pStrstu);
/*字符串报文数据域的读取,一旦字符串报文解析结构填充完毕，用户就可调用下面函数获取每一个数据域的值*/
int stresvalue(char * buf, STRRESVAR Strstu, int nIndex, void * pValue, int nType);

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

/*用户信息打印*/
void printUser();
char * userName();


#endif
