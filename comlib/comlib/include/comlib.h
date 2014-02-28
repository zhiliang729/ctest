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
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

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
int PrintLog(FILE* pfile, const char * pformat, ...);
int PrintTraceLog(const char* pformat, ...);

#endif
