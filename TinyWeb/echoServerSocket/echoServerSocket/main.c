//
//  main.c
//  echoServerSocket
//
//  Created by zhangliang on 13-11-6.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include "csapp.h"

void output(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    
    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %ld bytes : %s \n", n, buf);
        Rio_writen(connfd, buf, n);
    }
}

int main(int argc, char * *argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent * hp;
    char * haddrp;
    if (argc != 2) {
        fprintf(stderr, "usage:%s<port>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    
    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        
        //determine the domain name and ip address of the client
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s(%s)\n", hp->h_name, haddrp);
        
        output(connfd);
        Close(connfd);
    }
    return 0;
}

