//
//  main.c
//  TinyWeb--ConcurrentBasedOnThread
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"

void echo(int connfd);
void * thread(void *vargp);
void output(int connfd);


int main(int argc, const char * argv[])
{
    int listenfd, *confdp, port, clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    
    listenfd = Open_listenfd(port);
    while (1) {
        confdp = Malloc(sizeof(int));//为避免竞争，必须将每个accept返回的已连接描述符分配到它自己的动态分配的存储器块
        *confdp = Accept(listenfd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
        Pthread_create(&tid, NULL, thread, confdp);
    }
    return 0;
}

void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    Pthread_detach(Pthread_self());//在线程例程中，为了避免存储器泄漏，既然
                                    //我们不显式地收回线程，我们就必须分离
                                    //每个线程，使得它的存储器资源在它终止
                                    //时能够被收回。
    Free(vargp);//更进一步，我们必须小心释放主线程分配的存储器块。
    echo(connfd);
    Close(connfd);
    return NULL;
}

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


