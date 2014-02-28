//
//  main.c
//  Posix--Pthreads
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "csapp.h"

void *thread(void *vargp);

/*
 *主线程创建一个对等线程，然后等待它的终止。对等线程输出“Hello， World！\n”并且终止。
 *当主线程检测到对等线程终止后，它就通过调用exit来终止该线程
 */


/*
 *主线程代码由main函数开始，主线程声明了一个本地变量tid，它可以用来存放对等线程的线程id
 *主线程通过调用pthread_create函数创建一个新的对等线程。当对pthread_create的调用返回时，
 *主线程和新创建的对等线程并发运行，并且tid包含系in线程的ID。通过调用pthread——join，主线
 *程等待对等线程终止。最后主线程调用exit，终止当时运行在这个进程中的所有线程（在这个实例中，
 *就只有主线程）。
 */
int main(int argc, const char * argv[])
{
    pthread_t tid;
    Pthread_create(&tid, NULL, thread, NULL);
    Pthread_join(tid, NULL);
    return 0;
}

/*
 *线程的代码和本地数据被封装在一个线程例程中。每个线程例程都以一个通用指针作为输入，
 *并返回一个通用指针。如果你想传递多个参数给线程例程，那么你应该将参数放到一个结构中，
 *并传递一个指向该结构的指针。相似地，如果你想要线程例程返回多个参数，你可以返回一个
 *指向结构的指针。
 */
void *thread(void * vargp)//thread routine
{
    printf("Hello, world!\n");
    return NULL;//执行return语句来终止对等线程
}

