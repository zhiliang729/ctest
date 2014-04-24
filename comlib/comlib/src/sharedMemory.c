//
//  sharedMemory.c
//  comlib
//
//  Created by zhangliang on 14-4-24.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

#define VERIFYERR(a, b)\
                if (a) {\
                    fprintf(stderr, "%s failed.\n", b);\
                    return -1;\
                }else {\
                    fprintf(stderr, "%s success.\n", b);\
                }
#define SEMKEY 0x1234
#define SHMKEY 0x1234

/*
 利用“1-n-n”模型操作共享内存的例子，用户首先格式化共享内存，然后可以向共享内存中指定的记录块写入和读取记录。
 使用前必须初始化共享内存，用1-n-n的要求格式共享内存。
 */

int main(int argc, char * argv[])
{
    int shmid, semid, *pd;
    VERIFYERR((semid = semget(SEMKEY, 1, 0666 | IPC_CREAT)) < 0, "Open (cretae) Sem");//创建或打开信号量，只用一个信号量集合，一个信号量值即可
    VERIFYERR((shmid = shmget(SHMKEY, 20 * 1024, 0666 | IPC_CREAT)) < 0, "Open (create) shm");/*创建或打开共享内存，大小为20kb*/
    if (argc == 2 && argv[1][0] == 'c') {
        union semun semunion;
        semunion.val = 1;
        semctl(semid, 0, SETVAL, semunion);/*初始化信号量值*/
        CreateMemo(shmid, 10, 512);//格式化共享内存，共享内存id为shmid，有10个数据区，每个数据区大小为512
    }
    
    if ((pd = shmat(shmid, NULL, 0)) == NULL) {
        return -1;//映射共享内存失败
    }
    
    PrintLog(stderr, "nVal = [%d] max[%d] used[%d] cur[%d] size[%d]",
                        semctl(semid, 0, GETVAL),/*信号量取值*/
                        pd[0],                   /*共享内存中最大可分配记录块数*/
                        pd[1],                   /*共享内存中已经分配的记录块数*/
                        pd[2],                   /*共享内存中最近一次分配的记录块编号*/
                        pd[3]);                  /*共享内存中一个记录块的字节大小*/
    shmdt(pd);
    return 0;
}



