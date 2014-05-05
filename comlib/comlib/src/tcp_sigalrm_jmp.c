//
//  tcp_sigalrm_jmp.c
//  comlib
//
//  Created by zhangliang on 14-5-5.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

static int nTimeOutFlag = 0;/*1.定义超时标志变量*/
jmp_buf env;/*1.定义跳转结构*/

void OnTimeout3(int nSignal)/*2.信号处理函数*/
{
    signal(nSignal, SIG_IGN);/*超时一次后就忽略信号SIGALARM,防止循环超时*/
    nTimeOutFlag = 1;/*设置超时标志位“已超时”*/
    longjmp(env, 1);/*语句跳转*/
    return;/*本句实际上已经不会执行了*/
}

/*main ip port*/
int main19(int argc, char * argv[])
{
    int nSock = -1, ret;
    if (argc != 3) {
        return 1;
    }
    
    nTimeOutFlag = 0;
    setjmp(env);/*3.记录跳转点*/
    if (nTimeOutFlag == 1) {
        printf("Connect Timeout.\n");/*4.超时判断*/
    }else{
        signal(SIGALRM, OnTimeout3);/*5.捕获信号SIGALRM*/
        alarm(10);/*5.发送定时器信号SIGALRM*/
        ret = ConnectSock(&nSock, atoi(argv[2]), argv[1]);/*6.执行函数*/
        alarm(0);/*7.取消定时器*/
        signal(SIGALRM, SIG_IGN);/*7.忽略信号SIGALRM*/
        if (ret == 0) {/*非超时处理*/
            printf("Connect Success!\n");
        }else printf("connect Error!\n");
    }
    
    return 0;
}
