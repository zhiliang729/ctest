//
//  main.c
//  sbuf
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"

typedef struct {
    int *buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
} sbuf_t;

void sbuf_init(sbuf_t * sp, int n)
{
    sp -> buf = Calloc(n, sizeof(int));
    sp -> n = n;
    sp -> front = sp -> rear = 0;
    Sem_init(&sp -> mutex, 0, 1);
    Sem_init(&sp -> slots, 0, n);
    Sem_init(&sp -> items, 0, 0);
}

void sbuf_insert(sbuf_t * sp, int item)
{
    P(&sp -> slots);
    P(&sp -> mutex);
    sp -> buf[(++sp -> rear) % (sp -> n)] = item;
    V(&sp -> mutex);
    V(&sp -> items);
}

int sbuf_remove(sbuf_t * sp)
{
    int item;
    P(&sp -> items);
    P(&sp -> mutex);
    item = sp -> buf[(++sp -> front) % (sp -> n)];
    V(&sp -> mutex);
    V(&sp -> slots);
    return item;
}

int main(int argc, const char * argv[])
{
    
    return 0;
}

