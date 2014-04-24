//
//  shm.c
//  comlib
//
//  Created by zhangliang on 14-4-24.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*1-n-n模型实现共享内存管理*/



/*为了能顺利使用1-n-n模型，共享内存首地址处必须加入内存的划分信息，包括共享内存数据区划分的记录块数、当前已经使用的记录块数、最近一次分配的记录编号和每个记录块的长度等。内存块模型应该如下：信息区：记录块数、已用记录块数、最近分配记录编号、记录块大小；MAP区：0.....n; 数据区：第0块，第1块，……第n块；故在内核中创建共享内存后，必须调用下面函数来建立内存数据存储模型*/
/*
    初始化操作
    模型1-n-n的初始化函数
    本函数格式化标识号为shmid的共享内存，将其数据区划分成index个size大小的数据记录，修改其信息区数据，并重置MAP为数据记录全部未分配，函数成功返回0否则返回非0值*/
int CreateMemo(int shmid, int index, int size)
{
    char * pc;
    int * pd;
    if ((pc = shmat(shmid, NULL, 0)) == NULL) {/*映射共享内存到内核认为合适的进程空间的地址*/
        return -1;
    }
    pd = (int *)pc;//强转成整型指针，初始化信息区的内容   从地址空间的第一位开始设置信息区
    pd[0] = index;//共享内存数据区划分的记录块数
    pd[1] = 0;//当前已使用的记录块数
    pd[2] = 0;//最近一次分配的记录编号
    pd[3] = size;//每个记录块的大小
    memset(pc + sizeof(int) * 4, 0, index);//重置MAP区， 占用index个字节
    shmdt(pc);//释放共享内存映射
    return 0;
}

/*
    申请操作
    调用下面函数可以在标识号为shmid的共享内存中申请一块空闲记录区域，并将该区域的编号回传到参数index中。
        算法实现步骤：
            1.进程对信号量提交P操作，申请共享内存资源。
            2.进程访问MAP区，寻找值为0的字节，假设序号为m。
            3.置字节m的值为1，m同时也是分配的共享内存块号。
            4.进程对信号量提交V操作，申请过程完毕
 */
/*
 1-n-n内存记录分配函数
 1-n-n模型中，调用完成共享内存记录申请。
 与共享内存shmid配对的信号量集合的标识号为semid，信号量序号为semindex。在空闲记录块分配期间，有且仅有一个进程能够访问共享内存MAP区。
 函数调用成功时返回0，否则返回其他值。
 */
int AllocMemo(int shmid, int semid, int semindex, int * index)
{
    char * paddr;
    int ret;
    if ((paddr = shmat(shmid, NULL, 0)) == NULL) {//映射共享内存到进程中
        return -2;//映射失败返回-2
    }
    if (SEMP(semid, semindex) != 0) {//信号P操作，申请操作共享内存资源
        shmdt(paddr);
        return -3;//P操作失败返回-3
    }
    
    ret = AllocMemoExt(paddr, index);//分配记录， 分配失败时返回-1.
    SEMV(semid, semindex);//信号V操作，释放操作共享内存资源
    shmdt(paddr);//取消共享内存映射
    return ret;
}

/*
 1-n-n空闲记录查找分配函数
 函数实现了在MAP中查找空闲记录并分配的具体过程，指针pc指向共享内存的映射地址，参数index回传满足要求的记录序号，函数成功时返回0，否则返回-1.
 本算法并不从第0块记录块开始分配，而是从上次分配的共享块处开始查询空闲块，如果遍历到MAP末尾仍未找到目标则再从第0块记录开始查找
 */
int AllocMemoExt(char * pc, int * index)
{
    int nSize, nUsed, nOff, i;
    int * pd = (int *)pc;
    nSize = pd[0];//记录块数
    nUsed = pd[1];//已分配数
    nOff = pd[2];//最近分配序号
    if (nUsed >= nSize) {//已分配数等于记录块数，返回-1
        return -1;
    }
    pc = pc + sizeof(int) * 4;//指针移到到MAP区首地址
    for (i = 0; i < nSize; i++) {//从nOff处查找空闲记录
        if ((pc[nOff] & 0xffff) == 0) {//字符转整型采用此种处理办法，当值为0时，说明该MAP区对应的共享内存区未分配。返回
            break;
        }
        nOff = (nOff + 1) % nSize;//移动指针，本来此处只用自增1就好了，但是如果只是自增1了，那么一方面要判断到达nSize区与否，到达nSize区如果还没找到，还得返回去在0-nOff中间查找，用求余操作，一方面让循环继续从nOff到nSize循环，当到达nSize之后，自动从0开始到nOff循环。故查询的顺序为 p[noff],p[noff+1],……,p[nSize],p[0],……,p[noff-1]。因为限定的循环个数是nSize个。故到p[noff-1]时，停止
    }
    
    if (i >= nSize) {//当i=nSize，说明已经查完p[noff-1]，仍然没有找到空闲记录，即没有空闲记录，所以退出，返回-2.
        return -2;
    }
    
    //若找到空闲记录，则做如下操作
    pc[nOff] = 1;//更改记录空闲标记为1，表示已经被使用
    pd[1]++;//已分配数目加1
    pd[2] = nOff;//最近分配记录序号记录
    *index = nOff;//修改找到的空闲记录为noff
    return 0;
}

/*
 1-n-n中完成共享内存记录块的回收
 函数回收标识号为shmid的共享内存中编号为index的记录区域。
 步骤如下：
        1.进程对信号量提交P操作，申请释放共享内存资源
        2.置MAP区第index字节值为0.
        3.进程对信号量提交V操作，回收过程完毕
 与共享内存shmid配对的信号量集合的标识号为semid，信号量序号为semindex，函数调用成功时返回0，否则返回其他值。
 */
int FreeMemo(int shmid, int semid, int semindex, int index)
{
    char * paddr, *pc;
    int * pd;
    if ((paddr = shmat(shmid, NULL, 0)) == NULL) {
        return -2;//如果映射共享内存失败则返回-2
    }
    if (SEMP(semid, semindex) != 0) {/*信号P操作，申请操作共享内存资源*/
        shmdt(paddr);//取消映射
        return -3;//P操作失败返回-3
    }
    pd = (int *)paddr;
    pc = paddr + sizeof(int) * 4;//MAP区首地址
    
    assert(index >= 0 && index < pd[0]);
    
    if (index >= 0 && index < pd[0]) {//在MAP区范围内释放记录
        pc[index] = 0;//更改记录空闲标志,设为空闲
        pd[1]--;//已分配数减1
    }
    SEMV(semid, semindex);//信号V操作，释放操作共享内存资源
    shmdt(paddr);//取消共享内存映射
    return 0;
}

/*
 模型1-n-n 内存记录寻址函数
 函数查询共享内存记录块的地址
 本模型将共享内存块划分为多个记录，并以标号区分之，其中paddr是共享内存映射地址，index是记录编号，函数成功时，返回记录首地址，否则返回NULL。
 */
char * GetMemoAddr(char * paddr, int index)
{
    int * pd = (int *)paddr;
//    int nSize, nUsed;
    if (index < 0 || index >= pd[0]) {
        return NULL;
    }
    return (paddr +//首地址
            sizeof(int) * 4 +//信息区，4个整型
            pd[0] * sizeof(char) +//MAP区，pd[0]个字节
            pd[3] * index);//数据区，每个记录pd[3]个字节
}


