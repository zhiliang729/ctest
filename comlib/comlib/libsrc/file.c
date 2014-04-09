//
//  file.c
//  comlib
//
//  Created by zhangliang on 14-4-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include "comlib.h"

/* 本函数从文件描述符nFile所指向的文件中读取pSize个字节的数据到内存pData中，仅当错误发生，或者全部数据读取完毕时，函数返回*/
int ReadFile(int nFile, void * pData, ssize_t * pSize)
{
    ssize_t nLeft, nRead;
    char * pcData = pData;
    ASSERT(pData != NULL && pSize != NULL);
    nLeft = *pSize;
    
    while (nLeft > 0) {
        if ((nRead = read(nFile, pcData, nLeft)) < 0) {
            if (errno != EINTR) {/*错误代码EINTR表示在读取数据之前，系统调用read被一个信号signal中断，可以继续执行读取操作。*/
                ASSERT(0);
            }
            nRead = 0;
        }else if (nRead == 0){
            break;
        }else{
            
        }
        
        nLeft -= nRead;
        pcData += nRead;
    }
    
    *pSize = *pSize - nLeft;
    return 0;
}

/*把pData指向的nSize个字节的数据写入文件描述符nFile所指向的文件中，仅当错误发生，或数据全部输出完毕时，函数返回*/
int WriteFile(int nFile, void* pData, ssize_t nSize)
{
    ssize_t nLeft = nSize, nWrite;
    const char * pcData = pData;
    ASSERT(pData != NULL);
    
    while (nLeft > 0) {
        if ((nWrite = write(nFile, pcData, nLeft)) <= 0) {
            if (errno != EINTR) {/*错误代码EINTR表示在写入数据之前，系统调用write被一个信号signal中断，可以继续执行写入操作。*/
                ASSERT(0);
            }
            nWrite = 0;
        }
        
        nLeft -= nWrite;
        pcData += nWrite;
    }
    return 0;
}

int ReadFileExt(int nFile, void * pData, int pnSize, int nTimeout);
int WriteFileExt(int nFile, void* pData, int nSize, int nTimeout);
