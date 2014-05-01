//
//  trans1.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

struct desktop/*监控前台结构*/
{
    char szIP[20];
    int nPort;
};

struct desktop top[] = /*监控前台列表*/
{
    {"127.0.0.1", 10000},
    {"127.0.0.1", 10001}
};

int main(int argc, char * argv[])
{
    int nSock, i;
    ssize_t nSize;
    char szBuf[1024];
    if (CreateUdpSock(&nSock, 8888) != 0) {/*创建接收UDP套接字*/
        return 1;
    }
    
    while (1) {
        memset(szBuf, 0, sizeof(szBuf));
        nSize = sizeof(szBuf);
        RecvMsgByUdp(nSock, szBuf, &nSize);/*接收主机进程监控信息*/
        fprintf(stderr, "Get [%s]\n", szBuf);
        for (i = 0; i < sizeof(top)/sizeof(struct desktop); i++) {/*获取数组中元素个数的技巧，它可以抽象为“全部数组空间/数组元素空间”，即“sizeof（数组名）/sizeof（数组元素类型名）”*/
            /*发送接收到的信息到每个前台进程*/
            fprintf(stderr, "Send [%s] [%d]", top[i].szIP, top[i].nPort);
//            SendMsgByUdp(szBuf, (int)strlen(szBuf), top[i].szIP, top[i].nPort);
            if (strcmp("0:FILE", szBuf) == 0 && top[i].nPort == 10001) {//FILE送到10001端口的进程
                SendMsgByUdp(szBuf, (int)strlen(szBuf), top[i].szIP, top[i].nPort);
            }
            if (strcmp("0:DATA", szBuf) == 0 && top[i].nPort == 10000) {//只将DATA送到10000端口的进程
                SendMsgByUdp(szBuf, (int)strlen(szBuf), top[i].szIP, top[i].nPort);
            }

        }
    }
    
}
