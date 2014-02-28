//
//  main.c
//  Sharing
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"
#define N 2

void *thread(void * vargp);

char * * ptr;//global variable

int main(int argc, char *  argv[])
{
    int i;
    pthread_t tid;
    char * msgs[2] = {
                        "Hello from foo",
                        "Hello from bar"
                    };
    
    ptr = msgs;
    
    for (i = 0; i < N; i++) {
        Pthread_create(&tid, NULL, thread, (void *)i);
    }
    
    for (i = 0; i < N; i++) {
        printf("%s\n", msgs[i]);
    }
    Pthread_exit(NULL);
    return 0;
}


void * thread(void * vargp)
{
    int myid = (int) vargp;
    static int cnt = 0;
//    printf("[%d]:%s (cnt = %d)\n", myid, ptr[myid], ++cnt);//可访问全局变量
    ptr[myid] = "am";//可修改共享变量
    return NULL;
}












