//
//  udps1.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int main17(int argc, char * argv[])
{
    int nSock;
    ssize_t nSize;
    char szBuf[256];
    /*创建端口号为9999的UDP服务描述符*/
    CreateUdpSock(&nSock, 9999);
    /*循环接收UDP数据报文*/
    nSize = sizeof(szBuf);
    while (RecvMsgByUdp(nSock, szBuf, &nSize) == 0) {
        printf("Recv UDP Data:[%ld][%s]\n", strlen(szBuf), szBuf);
        nSize = sizeof(szBuf);
    }
    /*关闭UDP套接字描述符*/
    close(nSock);
    return 0;
}

