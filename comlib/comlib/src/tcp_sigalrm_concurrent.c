//
//  tcp_sigalrm_concurrent.c
//  comlib
//
//  Created by zhangliang on 14-5-4.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//


/*
 超时方式并发
 设计一个套接字连接函数connect超时处理的例子，它从命令行输入IP地址和端口号，程序向该IP地址和端口号建立连接，如果建立连接失败，10秒钟后超时退出
 */
#include "comlib.h"

static int nTimeOutFlag = 0;/*1.定义超时标志变量*/

void OnTimeout(int nSignal)/*2.信号处理函数*/
{
    signal(nSignal, SIG_IGN);/*超时一次后就忽略信号SIGALARM,防止循环超时*/
    nTimeOutFlag = 1;/*设置超时标志位“已超时”*/
    return;
}

/*参数格式为name IP PORT*/
int main18(int argc, char * argv[])
{
    int nSock = -1, ret;
    if (argc != 3) {
        return 1;
    }
    
    signal(SIGALRM, OnTimeout);/*3.捕获信号SIGALRM*/
    alarm(10);/*3.发送定时器信号SIGALRM*/
    int port = atoi(argv[2]);
    printf("%s", argv[1]);
    ret = ConnectSock(&nSock, port, argv[1]);/*4.执行函数*/
    alarm(0);/*5.取消定时器*/
    signal(SIGALRM, SIG_IGN);/*5.忽略信号SGIALRM*/
    /*6.函数返回，超时判断*/
    if (nTimeOutFlag == 1) {
        printf("Connect Timeout.\n");/*超时处理*/
    }else if (ret == 0){
        printf("Connect Success.\n");/*非超时处理*/
    }else{
        printf("Connect Error!\n");
    }
    
    if (nSock != -1) {
        close(nSock);/*关闭套接字连接*/
    }
    
    return 0;
}