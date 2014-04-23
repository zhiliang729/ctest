//
//  main.c
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

#define VERIFYERR(a, b)\
        if (a) {\
            fprintf(stderr, "%s failed.\n", b);\
        }else {\
            fprintf(stderr, "%s success.\n", b);\
        }

int main(int argc, const char * argv[])
{
    int semid, index, i;
    union semun sem;
    unsigned short array[100];
    
    struct semid_ds ds;
    if (argc != 4) {
        return -1;
    }
    
    semid = atoi(argv[1]);
    index = atoi(argv[2]);
    if (argv[3][0] == 'c') {//创建信号量
        VERIFYERR(semget(semid, index, 0666 | IPC_CREAT | IPC_EXCL) < 0, "create sem");
    }else if(argv[3][0] == 'd'){//删除信号量
        VERIFYERR(semctl(semid, index, IPC_RMID, NULL) < 0, "Delete sem");
    }else if(argv[3][0] == 'v'){//查询信号量信息
        fprintf(stderr, "T\t   ID\t   INDEX\t   SEMVAL\t   SEMPID\t  SEMNCNT\t  SEMZCNT\n");
        fprintf(stderr, "s\t%6d\t%6d\t%10d\t%10d\t%10d\t%10d\n",
                semid,//标识号
                index,//信号量序号
                semctl(semid, index, GETVAL),//信号量值
                semctl(semid, index, GETPID),//最近访问进程ID
                semctl(semid, index, GETNCNT),//p阻塞进程数
                semctl(semid, index, GETZCNT)//z阻塞进程数
                );
    }else if(argv[3][0] == 'a'){//查询全体信号量取值
        ds.sem_nsems = 0;
        sem.buf = &ds;
        VERIFYERR(semctl(semid, 0, IPC_STAT, sem), "Get sem stat");
        sem.array = array;
        VERIFYERR(semctl(semid, 0, GETALL, sem), "Get sem all");
        for (i = 0; i < ds.sem_nsems; i++) {
            fprintf(stderr, "sem no [%d][%d]\n", i, array[i]);
        }
    }else{//设置单个信号量取值
        sem.val = atoi(argv[3]);
        VERIFYERR(semctl(semid, index, SETVAL, sem) != 0, "Set sem val");
    }
    
    return 0;
}

