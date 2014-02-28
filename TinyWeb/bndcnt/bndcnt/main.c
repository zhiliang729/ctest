//
//  main.c
//  bndcnt
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"

#define NITERS 10000

void *count(void *arg);

unsigned int cnt = 0;

int main(int argc, const char * argv[])
{
    pthread_t tid1, tid2;
    Pthread_create(&tid1, NULL, count, NULL);
    Pthread_create(&tid2, NULL, count, NULL);
    
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);
    
    if (cnt != (unsigned) NITERS * 2) {
        printf("BOOM! cnt = %d \n", cnt);
    }else{
        printf("ok cnt = %d\n", cnt);
    }
    return 0;
}

void * count(void *arg)
{
    int i;
    for ( i = 0; i < NITERS; i++) {
        sem_t mutex;//使用信号量访问共享变量
        sem_init(&mutex, 0, 1);
        sem_wait(&mutex);
        cnt++;
        sem_post(&mutex);
        return NULL;
    }
    return NULL;
}

