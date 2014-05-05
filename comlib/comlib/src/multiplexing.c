//
//  multiplexing.c
//  comlib
//
//  Created by zhangliang on 14-5-5.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"


/*
    多路复用并发模型
    多路复用函数select把一些文件描述符集合在一起，如果某个文件描述符的状态发生变化，比如进入“写就绪”或者“读就绪”状态，函数select会立即返回，并且通知进程读取或写入数据；如果没有I/O到达，进程将进入阻塞，指导函数select超时退出为止。
    几种常见的使用函数select的套接字进程如下：
        1.交互式进程
            程序以便处理客户的交互式输入输出，一边使用套接字。多路复用标准输入、标准输出和套接字文件描述符
        2.多套接字进程
            程序同时使用侦听套接字和大量的连接套接字
        3.多协议进程
            程序同时使用TCP套接字和UDP套接字
        4.多服务进程
            程序同时应用多种服务，完成多种应用协议，比如inetd守护进程等。
 */

/*
 
 在上例中，进程采用多路复用方式，同时处理一个侦听套接字和n个连接套接字。如果在规定时间内没有产生套接字连接申请或者接受到通信数据，进程将处于阻塞状态；如果侦听套接字状态或任一连接套接字状态的状态发生变化，函数select将立即返回；如果超过了规定时间套接字描述符状态任然没有变化，函数select将超时返回，继续下一轮循环。
 采用函数select实现套接字的并发处理，有如下有点：
    1.在监控套接字描述符状态变化的过程中，函数select以阻塞方式执行，这样可以节省cpu时间。
    2.当规定的时间到达后，套接字仍然没有连接申请、接收或发送，函数select将自动返回，这样可以预防进程一直阻塞下去。
    3.函数select能够同时监控多个套接字描述符的状态，实现套接字的并发处理。
 */

#define MAX 5

int main20(int argc, char * argv[])
{
    if (argc != 3) {
        return 1;
    }
    int nLisSock, nSockVar[MAX];/*侦听套接字与连接套接字描述符数组*/
    int i, n = 0, ret;
    char buf[1024];/*接收套接字输入缓冲*/
    fd_set fdset;/*文件描述符集合*/
    struct timeval waittime;/*超时时间*/
    CreateSock(&nLisSock, atoi(argv[2]), MAX);/*创建侦听套接字*/
    while (1) {
        /*---------------------1.创建套接字描述符集合--------------*/
        FD_ZERO(&fdset);
        FD_SET(nLisSock, &fdset);/*监视侦听套接字*/
        for (i = 0; i < n; i++) {
            FD_SET(nSockVar[i], &fdset);/*监视连接套接字*/
        }
        /*---------------------2.准备超时时间---------------------*/
        waittime.tv_sec = 0;/*超时时间设为0.1秒*/
        waittime.tv_usec = 100000;
        /*---------------------3.调用select，检查返回--------------*/
        ret = select(nSockVar[n-1]+1, &fdset, NULL, NULL, &waittime);
        if (ret == 0) {
            continue;/*超时时间*/
        }else if (ret == -1)break;/*错误*/
                                /*产生了套接字连接或数据发送请求*/
        /*---------------------4.侦听套接字接收连接申请--------------*/
        if (FD_ISSET(nLisSock, &fdset)) {
            /*创建套接字描述符nSockVar[n] 与客户端套接字建立连接*/
            if ((nSockVar[n] = accept(nLisSock, NULL, NULL)) > 0) {
                n++;
            }
        }else{
            for (i = 0; i < n; i++) {/*遍历连接套接字，判断并读入数据*/
                /*----------------4.套接字数据收发------------------*/
                if (FD_ISSET(nSockVar[i], &fdset)) {
                    /*从连接套接字中读入数据*/
                    memset(buf, 0, sizeof(buf));
                    read(nSockVar[i], buf, sizeof(buf));
                    /*--------------------其他处理代码开始----------*/
                    
                    
                    
                    /*------------------其他处理代码完毕-------------*/
                }/*FD_ISSET*/
            }/*for*/
        }/*else*/
        
        
        
        
    }
    return 0;
}


