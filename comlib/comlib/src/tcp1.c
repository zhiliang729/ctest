//
//  tcp1.c
//  comlib
//
//  Created by zhangliang on 14-4-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

#define VERIFYERR(a, b)\
if (a) {\
fprintf(stderr, "%s failed.\n", b);\
}else {\
fprintf(stderr, "%s success.\n", b);\
}

/*创建socket侦听端口，与客户端建立连接，然后接收并打印客户端发送的数据*/
int main10(int argc, const char * argv[])
{
    int nSock, nSock1;/*定义socket描述符*/
    char buf[10240];
    /*创建端口号为9001的侦听套接字*/
    VERIFYERR(CreateSock(&nSock, 9001, 9) != 0, "Create Listen Socket");
    /*接收客户端连接申请，创建连接套接字nsock1*/
    VERIFYERR(AcceptSock(&nSock1, nSock) != 0, "Link");
    memset(buf, 0, sizeof(buf));
    /*接收客户端发送的TCP数据信息*/
    recv(nSock1, buf, sizeof(buf), 0);
    fprintf(stderr, "%s", buf);
    close(nSock1);/*关闭侦听套接字*/
    close(nSock);/*关闭连接套接字*/
    return 0;
}