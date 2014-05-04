//
//  desktop1.c
//  comlib
//
//  Created by zhangliang on 14-5-1.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int main(int argc, char * argv[])
{
    int nSock;
    ssize_t nSize;
    char szBuf[1024];
    if (argc != 2) {
        return 1;
    }
    
    /*创建接收UDP套接字*/
    if (CreateUdpSock(&nSock, atoi(argv[1])) != 0) {
        return 1;
    }
    
    while (1) {
        memset(szBuf, 0, sizeof(szBuf));
        nSize = sizeof(szBuf);
        /*接收转发进程发送的数据*/
        RecvMsgByUdp(nSock, szBuf, &nSize);
        fprintf(stderr, "Get [%s]\n", szBuf);
    }
}
