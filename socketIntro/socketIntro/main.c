//
//  main.c
//  socketIntro
//
//  Created by zhangliang on 14-4-25.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define VERIFYERR(a, b)\
if (a) {\
fprintf(stderr, "%s failed.\n", b);\
}else {\
fprintf(stderr, "%s success.\n", b);\
}

/*创建socket侦听端口，与客户端建立连接，然后接收并打印客户端发送的数据*/
int main(int argc, const char * argv[])
{
    int nSock, nSock1;/*定义socket描述符*/
    char buf[1024];
    /*创建端口号为9001的侦听套接字*/
    return 0;
}

