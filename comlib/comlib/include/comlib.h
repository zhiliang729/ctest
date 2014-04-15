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
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

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

#define BYTESIZE 1024

/*格式化日志输出函数*/
int PrintLog(FILE * pfile, const char * pformat, ...);
int PrintTraceLog(const char * pformat, ...);
int PrintHexLog(FILE * pfile, void * pData, int nSize);
int PrintTraceHexLog(void * pData, int nSize);
int Verify(int bStatus, const char * szBuf, const char * szFile, int nLine);

/*读写功能库*/
int ReadFile(int nFile, void * pData, ssize_t * pSize);
int WriteFile(int nFile, void* pData, ssize_t nSize);
int ReadFileExt(int nFile, void * pData, int pnSize, int nTimeout);
int WriteFileExt(int nFile, void* pData, int nSize, int nTimeout);




/*守护进程生成器*/
int InitServer();
#endif
