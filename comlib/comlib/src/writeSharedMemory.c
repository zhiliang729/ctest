//
//  writeSharedMemory.c
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

/*接收用户输入指定的共享内存记录块编号，并向该记录块中写入数据*/
int main8(int argc, char * argv[])
{
    int shmid, semid, index;
    char buf[1024], * paddr;
    VERIFYERR((semid = semget(SEMKEY, 1, 0666 | IPC_CREAT)) < 0, "Open (Create) Sem");//创建或打开信号量
    VERIFYERR((shmid = shmget(SHMKEY, 20 * 1024, 0666 | IPC_CREAT)) < 0, "Open (create) shm");//创建或打开共享内存
    VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "shmat");//申请内存记录
    VERIFYERR(AllocMemo(shmid, semid, 0, &index) != 0, "AllocMemo");//初始化共享内存
    PrintLog(stderr, "AllMemo index = [%d]", index);//打印记录编号
    printf("Please Input Data:");
    scanf("%s", buf);
    strcpy(GetMemoAddr(paddr, index), buf);/*写入数据到GetMemoAddr返回值位置*/
    printf("Input Data End.\n");
    shmdt(paddr);/*释放共享内存映射*/
    return 0;
}




