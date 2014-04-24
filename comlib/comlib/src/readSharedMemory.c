//
//  readSharedMemory.c
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

/*从命令行参数中接收指定的共享内存记录块编号，并读取该记录的数据*/

int main(int argc, char * argv[])
{
    int shmid, semid, index;
    char buf[1024], *paddr;
    if (argc != 2) {//输入编号
        return -1;
    }
    
    VERIFYERR((semid = semget(SEMKEY, 1, 0666 | IPC_CREAT)) < 0, "Open (create) sem");//创建或打开信号量
    VERIFYERR((shmid = shmget(SHMKEY, 20 * 1024, 0666 | IPC_CREAT)) < 0, "Open (create) shm");//创建或打开共享内存
    index = atoi(argv[1]);//获取记录编号
    VERIFYERR((paddr = shmat(shmid, NULL, 0)) == NULL, "Shmat");//映射共享内存到进程
    PrintLog(stderr, "Read Memo index = [%d]", index);
    printf("Data:%s\n", GetMemoAddr(paddr, index));//输入记录数据
    strcpy(buf, GetMemoAddr(paddr, index));
    printf("Data:%s\n", buf);//输入记录数据
    FreeMemo(shmid, semid, 0, index);//释放内存记录
    shmdt(paddr);//释放内存映射
    return 0;
}


