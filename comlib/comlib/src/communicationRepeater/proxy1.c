//
//  proxy1.c
//  comlib
//
//  Created by zhangliang on 14-5-5.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int SendReceiveServer(int nLocalSock, int nRemoteSock);
int TransSock(int nReadSock, int nWriteSock);
int main_com1(int argc, char * argv[])
{
    int nListenSock = 0, nLocalSock = 0, nRemoteSock = 0;
    if (argc != 4) {/*命令行参数：“proxy1 远程IP地址 远程端口 本地端口”*/
        PrintLog(stdout, "proxy1 IP PORT LOCALPORT\n");
        return 1;
    }
    /*建立侦听套接字*/
    ASSERT(CreateSock(&nListenSock, atoi(argv[3]), 8) == 0);
    PrintLog(stdout, "listen service : service is activated.");
    while (1) {
        /*接收本地套接字连接*/
        if (!VERIFY(AcceptSock(&nLocalSock, nListenSock) == 0)) {            continue;
        }
        
        /*连接远程目标端*/
        if (!VERIFY(ConnectSock(&nRemoteSock, atoi(argv[2]), argv[1]) == 0)) {
            PrintLog(stderr, "Connect Server Fail, Addr [%s], Port [%d]", argv[1], argv[2]);
            return 2;
        }
        SendReceiveServer(nLocalSock, nRemoteSock);
        close(nLocalSock);/*关闭本地端套接字*/
        close(nRemoteSock);/*关闭目标端套接字*/
    }
}

int SendReceiveServer(int nLocalSock, int nRemoteSock)
{
    int nMaxSock;
    struct timeval waittime;
    fd_set fdset;
    int ret = 0;
    while (1) {
        /*1.创建套接字描述符集合*/
        FD_ZERO(&fdset);
        FD_SET(nLocalSock, &fdset);
        FD_SET(nRemoteSock, &fdset);
        /*2.准备超时时间*/
        waittime.tv_sec = 60;
        waittime.tv_usec = 0;
        /*3.调用select，检查返回*/
        nMaxSock = nLocalSock > nRemoteSock ? nLocalSock : nRemoteSock;
        if (select(nMaxSock + 1, &fdset, NULL, NULL, &waittime) == -1) {
            PrintLog(stderr, "Select Failed.\n");
            return 1;
        }
        
        /*4.监测状态变化的套接字*/
        /*如果套接字nLocalSock可以接收，则接收数据，发送数据到套接字nRemoteSock*/
        if (FD_ISSET(nLocalSock, &fdset)) {
            ret = TransSock(nLocalSock, nRemoteSock);
        }else if(FD_ISSET(nRemoteSock, &fdset)){/*如果套接字nRemoteSock可以接收，则接收数据，并发送到nLocalSock*/
            ret = TransSock(nRemoteSock, nLocalSock);
        }
        
        /*结果判断*/
        if (ret == 1) {/*套接字系统错误，退出函数*/
            return 1;
        }else if(ret == 2){/*套接字关闭，退出函数*/
            break;
        }
    }
    return 0;
}

/*从nReadSock中读取数据，转发到nWriteSock中*/
int TransSock(int nReadSock, int nWriteSock)
{
    ssize_t nread;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if ((nread = read(nReadSock, buf, sizeof(buf))) < 0) {/*接收数据*/
        if (errno != EINTR) {
            return 1;
        }else{
            return 0;
        }
    }else if(nread == 0){
        PrintLog(stderr, "client is close");
        return 2;
    }
    PrintLog(stderr, "[%d] : %s", nread, buf);
    if (WriteFile(nWriteSock, buf, nread) != 0) {/*转发数据*/
        PrintLog(stderr, "Write message to server error [%d]", nread);
        perror("write");
        return 1;
    }
    return 0;
}
