//
//  debug.c
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"


int PrintLog(FILE* pfile, const char * pformat, ...)
{
    va_list _va_list;
    
    char szBuf[BYTESIZE];
    int nLen;
    if (pformat == NULL || pfile == NULL) {/*判断指针是否正确*/
        return -1;
    }
    
    
    TIMESTRU timestru;
    timestru = GetTime();
    
    nLen = sprintf(szBuf, "$Time:%04d.%02d.%02d %02d:%02d:%02d  $Process:[%d] $Log: ",
                   timestru.nYear, timestru.nMon, timestru.nDay,
                   timestru.nHour, timestru.nMin, timestru.nSec, getpid());
    
    va_start(_va_list, pformat);/*初始化变长参数列表*/
    nLen += vsprintf(szBuf+nLen, pformat, _va_list);/*传递变长参数*/
    va_end(_va_list);/*结束使用变长参数列表*/
    
    nLen += sprintf(szBuf + nLen, "\n");/*添加换行符*/
    
    if (fputs(szBuf, pfile) != EOF && fflush(pfile) != EOF) {//输出并刷新文件流  fputs输出失败则返回EOF  任何时候，都可以使用fflush刷新缓冲区，并将缓冲区的内容强制输出到文件中，参数stream指明了更新的文件流，当其值为NULL时，系统将刷新全部文件流的缓冲区。
        return 0;
    }
    
    
    if (fflush(pfile) == EOF) {
        printf("%s", strerror(errno));
    }
    
    return -2;//错误返回
}
