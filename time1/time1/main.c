//
//  main.c
//  time1
//
//  Created by zhangliang on 14-4-15.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main1(int argc, const char * argv[])
{
    /*系统时间*/
    time_t start, finish;
    time(&start);
    sleep(6);
    time(&finish);
    printf("start = [%ld], finish = [%ld], difftime = [%f]\n", start, finish, difftime(finish, start));
    return 0;
}

/*打印本地时间的例子，它先读取系统时间，再转换为本地时间格式输出*/
int main2(int argc, const char * argv[])
{
    /*
     本地时间
     unix中更改系统时间为本地日历时间函数：
     #include <time.h>
     struct tm * localtime(const time_t * clock);//转换系统时间按clock为当地时间，并以tm结构返回。结构tm以人们熟悉的年月日等形式记载时间，定义在头文件time.h中
     time_t mktime(struct tm * timeptr);//实现localtime的反功能，它返回tm结构中对应的系统时间。主要应用在计算上日或次日日期中
     */
    
    struct tm when;
    time_t now;
    time(&now);
    when = * localtime(&now);
    printf("now = [%ld][%04d%02d%02d %02d:%02d:%02d]\n", now,
           when.tm_year + 1900,
           when.tm_mon + 1,
           when.tm_mday,
           when.tm_hour,
           when.tm_min,
           when.tm_sec);

    return 0;
}

/*
 
 在unix中，定时器的处理是通过信号来完成的，1程序使用系统调用向unix内核设置定时器信息。2unix内核记载定时器，当到达定时器时间时，unix内核向进程发送定时器类信号。3进程捕获定时器类信号，并且执行相关处理操作。
1.普通定时器设置
    unix中定时器设置函数alarm的原型如下：
        #include <unistd.h>
        unsigned int alarm(unsigned int seconds);
    函数alarm在unix内核中为调用进程设置一个定时器，参数seconds决定了定时的长度，进程将在调用函数alarm后seconds秒，接收到unix内核发送的SIGALRM信号。如果在定时未完成内重复调用alarm，后一次的时间设置将覆盖前面的。如果参数seconds取值为0，定时器将被取消。函数每调用一次只能产生一次定时操作，如果需要反复定时，就要多次调用alarm。调用fork后，子进程中的定时器将被取消，但调用exec后，定时器仍然有效。
    函数alarm总能调用成功，它返回上次定时器剩余的定时时间，如果是第一次设置定时器，返回0。
    在unix中使用普通定时器的三个步骤如下。
        1.设置捕捉定时信号。
            更改信号SIGALRM的默认处理机制，并设置捕获响应函数。signal(SIGALRM, func);
        2.定时。
            在需要定时的代码处理函数alarm即可。
        3.编写响应定时信号函数。
            捕获定时信号模型函数模型 实例：
            void func（int sig）
            {
                ------------------------  //函数执行
                signal(SIGALRM, func);//再次设置捕获定时信号
                alarm(1);//再次定时
            }//由于在部分unix版本中，信号的捕获只能响应一次，故函数中需要增加信号捕捉设置。同理，函数alarm设置的定时器也只能定时一次，故函数中需要重新定时。
 */

/*每隔一秒向进程发送定时信号SIGALRM， 进程在接收到信号时将打印定时的次数，用户可以键入“ctrl+c” 或delete键来结束程序*/
#include <signal.h>
int number = 0;

void timefunc(int sig);
void timefunc(int sig)
{
    fprintf(stderr, "Alarm sig %d\n", number++);
    signal(sig, timefunc);/*再次设置捕获信号*/
    alarm(1);//再次定时
}

void terminalSig(int sig)
{
    if (sig == SIGTERM) {
        fprintf(stderr, "SIGTERM!\n");
//        kill(getpid(), SIGKILL);//自动退出程序
    }
}

void main3(int argc, const char * argv[])
{
    signal(SIGTERM, terminalSig);
    signal(SIGALRM, timefunc);//设置捕获信号
    alarm(1);//定时开始
    
    while (1) {
        ;
    }
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
    main3(argc, argv);
    return 0;
}

