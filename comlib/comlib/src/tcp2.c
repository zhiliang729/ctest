//
//  tcp2.c
//  comlib
//
//  Created by zhangliang on 14-4-29.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

#define ASSURE(b) if(!b)\
                {\
                    close(nSock);\
                    return 0;\
                }

/*首先创建一个服务器端侦听套接字，如果接收到客户端连接申请，则创建连接、读取并打印对方IP地址，然后关闭连接*/
int main11(int argc, char * argv[])
{
    int nSock, nSock1;/*定义socket描述符*/
    char pAddr[30];
    /*创建端口号为9001的侦听套接字*/
    if (CreateSock(&nSock, 9001, 9) != 0) {
        return 0;
    }
    
    /*接受客户端连接申请，创建连接套接字*/
    ASSURE(AcceptSock(&nSock1, nSock) == 0);
    
    /*本地ip地址信息*/
    ASSURE(LocateNativeAddr(nSock1, pAddr) == 0);
    
    fprintf(stderr, "local ip = %s\n", pAddr);
    
    /*获取客户端的IP地址信息*/
    ASSURE(LocateRemoteAddr(nSock1, pAddr) == 0);
    
    fprintf(stderr, "ip = %s\n", pAddr);
    close(nSock);/*关闭侦听套接字*/
    close(nSock1);/*关闭连接套接字*/
    return 0;
}
