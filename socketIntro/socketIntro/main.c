//
//  main.c
//  socketIntro
//
//  Created by zhangliang on 14-4-25.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define VERIFYERR(a, b)\
if (a) {\
fprintf(stderr, "%s failed.\n", b);\
}else {\
fprintf(stderr, "%s success.\n", b);\
}

/*设计一个查询系统服务名称及其服务端口的例子，它将服务名称作为命令行参数输入，并打印该服务占用的端口号
 服务可以查看/etc/services中都有什么服务
 */
int main1(int argc, const char * argv[])
{
    struct servent * serv;
    if (argc != 2) {
        return 1;
    }
    
    if ((serv = getservbyname(argv[1], "tcp")) == NULL) {
        herror("getservbyname");
        return 2;
    }
    
    printf("serv name : %s\n", serv->s_name);
    printf("serv port : %d\n", ntohs(serv->s_port));
    return 0;
}

/*
 本处设计一个查询系统服务名称及其服务端口的例子，它将服务端口号作为命令行参数输入，并打印该服务的系统名称。
 */
int main3(int argc, const char * argv[])
{
    struct servent * serv;
    if (argc != 2) {
        return 1;
    }
    if ((serv = getservbyport(htons(atoi(argv[1])), "tcp")) == NULL) {
        herror("getservbyname");
        return 2;
    }
    printf("Serv name:%s \n", serv->s_name);
    printf("Serv Port : %d\n", ntohs(serv->s_port));
    return 0;
}


/*设计一个根据主机域名显示IP地址的例子，用户运行程序transip，并将主机域名作为命令行参数输入，程序打印该主机的域名和IP地址*/
int main2(int argc, const char * argv[])
{
    struct hostent * host;
    if (argc != 2) {
        return 1;
    }
    
    if ((host = gethostbyname(argv[1])) == NULL) {
        herror("get host by name");
        return 2;
    }
    
    printf("Host name : %s \n", host->h_name);
    
    for (int i = 0; ; i++) {
        if (host->h_addr_list[i] != NULL) {
            printf("host ip %d: %s\n", i+1, inet_ntoa((*(struct in_addr *)host->h_addr_list[i])));
        }else{
            break;
        }
    }
    
    printf("Host IP : %s \n", inet_ntoa((*(struct in_addr *)host->h_addr_list[0])));
    
    return 0;
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
    main3(argc, argv);
    return 0;
}

