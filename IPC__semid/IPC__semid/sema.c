//
//  sema.c
//  IPC__semid
//
//  Created by zhangliang on 14-4-23.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define VERIFYERR(a, b)\
        if (a) {\
            fprintf(stderr, "%s failed.\n", b);\
            return -1;\
        }else {\
            fprintf(stderr, "%s success.\n", b);\
        }
//定义关键字为1000的信号量集合中第0、1位信号量分别为信号量A、B
int main(int argc, char * argv[])
{
    int semid;
    struct sembuf sb;
    /*打开信号量*/
    VERIFYERR((semid = semget(1000, 2, 0666)) < 0, "open sem 1000");
    /*申请生产执照*/
    sb.sem_num = 0;//信号量集合1000中第0个信号量
    sb.sem_op = -1;//执行p操作
    sb.sem_flg = sb.sem_flg & ~ IPC_NOWAIT;
    VERIFYERR(semop(semid, &sb, 1) != 0, "P sem 1000:0");//执行
    
    //生产产品
    fprintf(stderr, "[%d] producing...\n", getpid());
    sleep(1);//假设生产耗时1秒钟
    fprintf(stderr, "[%d] produced\n", getpid());
    
    //提交产品
    sb.sem_num = 1;//信号量集合1000中第1个信号量
    sb.sem_op = 1;//执行v操作
    sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT;
    VERIFYERR(semop(semid, &sb, 1) != 0, "V sem 1000:1");
    return 0;
}


