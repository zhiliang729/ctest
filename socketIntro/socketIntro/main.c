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


int main(int argc, const char * argv[])
{
    int soc;
    soc = socket(AF_INET, SOCK_STREAM, 0);
    soc = socket(AF_INET, SOCK_DGRAM, 0);
    
    int nIp;
    nIp = inet_addr("127.0.0.1");
    printf("ip:%d\n", nIp);
    struct in_addr addr;
    int ret = inet_aton("127.0.0.1", &addr);
    if (ret == 1) {
        printf("success : %d\n", addr.s_addr);
    }
    char *buf;
    buf = inet_ntoa(addr);
    printf("ip: %s\n", buf);
    struct sockaddr_in addr_in;
//    bind(soc, , )
    
    return 0;
}

