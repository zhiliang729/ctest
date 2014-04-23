//
//  sem.c
//  comlib
//
//  Created by zhangliang on 14-4-23.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*信号量阻塞操作  信号量的阻塞P操作（信号值减少）、释放V操作(信号值增加)和Z（信号值判断）操作功能  本函数通过封装系统调用semop来操作标识号为nSid，序号为nIndex的信号量，函数Semop将信号量的取值增加nVal。当nVal>0时，函数等同于V操作；当nVal<0时，函数的效果是信号量取值减小，等同于信号量P操作；当nVal=0时，函数等同于Z操作。简化了结构sembuf的设置过程，并增加了错误信息的判断  函数成功时返回0，信号中断时返回1，否则返回其他值*/
int Semop(int nSid, int nIndex , int nVal)
{
    struct sembuf sb;
    sb.sem_num = nIndex;
    sb.sem_op = nVal;
    sb.sem_flg = sb.sem_flg & ~ IPC_NOWAIT;//阻塞
    sb.sem_flg = sb.sem_flg & ~SEM_UNDO;//进程结束还原
    if (!VERIFY(semop(nSid, &sb, 1) == 0)) {
        if (errno == EINTR) {
            return 1;
        }
        return -1;
    }
    return 0;
}

/*非阻塞操作  以非阻塞方式执行P（信号量减少）、V（信号量增加）和Z（信号值判断）操作功能。 成功返回0 ，信号中断时返回1，不满足P操作或Z操作条件时立即返回2，否则返回其他值*/
int SemopNowait(int nSid, int nIndex , int nVal)
{
    struct sembuf sb;
    sb.sem_num = nIndex;
    sb.sem_op = nVal;
    sb.sem_flg = sb.sem_flg & IPC_NOWAIT;//阻塞
    sb.sem_flg = sb.sem_flg & ~SEM_UNDO;//进程结束还原
    if (!VERIFY(semop(nSid, &sb, 1) == 0)) {
        if (errno == EINTR) {
            return 1;
        }
        if (errno == EAGAIN) {
            return 2;
        }
        return -1;
    }
    return 0;
}

/*定时操作  对标识号为nSid，序号为nIndex的信号量执行P、V或Z操作。当操作被阻塞时，进程被挂起，直到阻塞条件消失或者阻塞nTimeout秒后函数返回*/
static int nTimeOutFlag = 0;
jmp_buf env;

static void OnTimeOut(int nSignal)
{
    signal(nSignal, SIG_IGN);
    nTimeOutFlag = 1;
    longjmp(env, 1);
    return;
}

int SemopTime(int nSid, int nIndex, int nVal, int nTimeOut)
{
    nTimeOutFlag = 0;
    alarm(nTimeOut);
    signal(SIGALRM, OnTimeOut);
    setjmp(env);
    if (nTimeOutFlag == 0) {
        return (Semop(nSid, nIndex, nVal));
    }
    alarm(0);//取消定时
    signal(SIGALRM, SIG_IGN);
    if (nTimeOutFlag != 0) {
        return 3;
    }
    return 0;
}
