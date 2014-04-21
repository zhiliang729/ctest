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

/*从文件描述符nFile所指向的文件中读取nSize个字节的数据到内存pData中，当错误发生、数据全部读取或者在nTimeout秒内数据未读取完毕时，函数返回。*/
int ReadFileExt(int nFile, void * pData, ssize_t * pnSize, int nTimeout)
{
    ssize_t nLeft, nRead;
    long nUseTime = 0;
    time_t t1, t2;
    char * pcData;
    int nFlag;
    fd_set fdsRead;
    struct timeval timeval1;
    ASSERT(pData != NULL);
    ASSERT(pnSize != NULL);
    pcData = pData;
    nLeft = * pnSize;
    while (nLeft > 0) {
        if (nTimeout != 0) {
            nTimeout -=nUseTime;
        }
        FD_ZERO(&fdsRead);/*监控当前文件*/
        FD_SET(nFile, &fdsRead);
        timeval1.tv_sec = nTimeout;/*设置超时时间*/
        timeval1.tv_usec = 0;
        t1 = 0;
        t2 = 0;
        time(&t1);
        nFlag = select(nFile + 1, &fdsRead, NULL, NULL, &timeval1);
        if (nFlag == 0) {
            * pnSize = * pnSize - nLeft;//记录已读个数
            ASSERTEXT(0, 1);/*记日志，返回1*/
        }
        
        if (nFlag < 0) {/*错误发生*/
            if (errno != EINTR) {/*错误代码EINTR表示在读取数据之前，系统调用read被一个信号signal中断，可以继续执行读取操作。*/
                ASSERT(0);
            }else{
                time(&t2);
                nRead = 0;
                nUseTime = t2 - t1;//计算用时
            }
        }else{/*正常读取*/
            time(&t2);
            nUseTime = t2 - t1;//计算用时
            if ((nRead = read(nFile, pcData, nLeft)) < 0) {/*读取*/
                if (errno != EINTR) {/*非信号中断类错误*/
                    ASSERT(0);
                }
                nRead = 0;
            }else if(nRead == 0){
                break;
            }
        }
        nLeft -= nRead;
        pcData += nRead;//移动指针
    }
    *pnSize = *pnSize - nLeft;//读取完成后, 修改读取的个数
    return 0;
}

/*
 把pData指向的nSize个字节的数据写入文件描述符nFile所指向所指向的文件中，当错误发生、数据全部输出或者在nTimeout秒内数据未输出完毕时，函数返回
 */
int WriteFileExt(int nFile, void* pData, ssize_t * nSize, int nTimeout)
{
    ssize_t nLeft, nWrite;
    long nUseTime = 0;
    const char * pcData;
    int nFlag;
    time_t t1, t2;
    fd_set fdsWrite;
    struct timeval timeval1;
    ASSERT(pData != NULL);
    pcData = pData;
    nLeft = *nSize;
    while (nLeft > 0) {
        if (nTimeout != 0) {
            nTimeout -= nUseTime;
        }
        FD_ZERO(&fdsWrite);/*设置监控文件集合*/
        FD_SET(nFile, &fdsWrite);
        timeval1.tv_sec = nTimeout;/*设置超时时间*/
        timeval1.tv_usec = 0;
        time(&t1);
        nFlag = select(nFile + 1, NULL, &fdsWrite, NULL, &timeval1);
        if (nFlag == 0) {
            ASSERTEXT(0, 1);/*超时，记日志，返回1*/
        }
        if (nFlag < 0) {/*错误发生*/
            if (errno != EINTR) {
                ASSERT(0);
            }else{
                time(&t2);
                nUseTime = t2 - t1;
                nWrite = 0;
            }
        }else{/*正常读取*/
            time(&t2);
            nUseTime = t2 - t1;
            if ((nWrite = write(nFile, pcData, nLeft)) <= 0) {
                if (errno != EINTR) {
                    ASSERT(0);
                    nWrite = 0;
                }
            }
        }
        nLeft -= nWrite;
        pcData += nWrite;//移动指针
    }
    *nSize = *nSize - nLeft;
    return 0;
}
