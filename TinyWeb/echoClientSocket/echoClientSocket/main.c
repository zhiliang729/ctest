//
//  main.c
//  echoClientSocket
//
//  Created by zhangliang on 13-11-6.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"

int main(int argc, char ** argv)
{
    int clientfd, port;
    char * host, buf[MAXLINE];
    rio_t rio;
    
    if (argc != 3) {
        fprintf(stderr, "usage:%s<host><port>\n", argv[0]);
        exit(0);
    }
    
    host = argv[1];
    port = atoi(argv[2]);
    
    clientfd = Open_clientfd(host, port);//创建套接字
    Rio_readinitb(&rio, clientfd);
    
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    
    Close(clientfd);
    
    return 0;
}

