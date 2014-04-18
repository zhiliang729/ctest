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
 
 2.精确定时器设置
    函数alarm设置的定时器只能精确到秒，而以下函数理论上可以精确到微秒：
        #include <sys/select.h>
        #include <sys/itimer.h>
        int getitimer(int which, struct itimerval *value);
        int setitimer(int which, const struct itimerval * value, struct itimerval * ovalue);
    函数setitimer可以提供三种定时器，它们互相独立，任意一个定时完成都将发送定时信号进程，并且自动重新计时。参数which确定了定时器的类型。
        取值           含义                                 信号发送
        ITIMER_REAL   定时真实时间，与alarm类型相同             SIGALRM
        ITIMER_VIRTUAL   定时进程在用户态下的实际执行时间           SIGVTALRM
        ITIMER_PROF   定时进程在用户态和核心态下的实际执行时间      SIGPROF
    这三个定时发送不同的信号。
    函数alarm本质上设置的是低精确、非重载的ITIMER_REAL类定时器，它只能精确到秒，并且每次设置只能产生一次定时。函数setitimer设置的定时器则不同，它们不但可以计时到微妙（理论上），还能自动循环定时。在一个unix进程中，不能同时使用alarm和ITIMER_REAL类定时器。结构itimerval描述了定时器的组成：
        struct itimerval
        {
            struct timeval it_interval; //下次定时取值
            struct timeval it_value;//本次定时设置值
        }
 
        结构timeval描述了一个精确到微妙的时间：
        struct timeval
        {
            long tv_sec;//秒（1000000微妙）
            long tv_usec;//微妙
        }
        函数setitimer设置一个定时器，参数value指向一个itimerval结构，该结构决定了设置的定时器信息，结构成员it_value指定首次定时的时间，结构成员it_interval指定下次定时的时间。定时器工作时，先将it_value的时间值减到0，发送一个信号，再将it_value赋值为it_interval的值，重新开始定时，如此反复。如果it_value值被设置为0，则定时器停止定时；如果it_value值不为0但it_interval值为0，则定时器在一次定时后终止。
        函数setitimer调用成功时返回0，否则返回-1，参数ovalue如果不为空，返回上次的定时器状态。
        函数getitimer获取当前的定时器状态，整型参数which指定了读取的定时器类型，参数value返回定时器状态。函数调用成功返回0，否则返回-1.
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

/*
 设置一个定时器，每2.5秒产生一个SIGALRM 信号
 */
#include <sys/select.h>
#include <sys/time.h>
void timefunc1(int sig);
void timefunc1(int sig)
{
    fprintf(stderr, "Alarm sig %d\n", number++);
    signal(sig, timefunc1);/*再次设置捕获信号*/
}

void main4(int argc, const char * argv[])
{
    signal(SIGALRM, timefunc1);//设置捕获信号
    
    struct itimerval value;
    value.it_value.tv_sec = 2;
    value.it_value.tv_usec = 500000;
    value.it_interval.tv_sec = 2;
    value.it_interval.tv_usec = 500000;
    setitimer(ITIMER_REAL, &value, NULL);//setitimer设置的定时器可以重复定时，无需多次调用
    
    //前10次每2.5秒一次，后面10次0.5秒一次，之后退出
//    int i = 0;
//    while (1) {
//        sleep(3);
//        if (i < 10) {
//            value.it_value.tv_sec = 2;
//            value.it_value.tv_usec = 500000;
//            value.it_interval.tv_sec = 2;
//            value.it_interval.tv_usec = 500000;
//            setitimer(ITIMER_REAL, &value, NULL);
//        }else if (i < 20){
//            value.it_value.tv_sec = 0;
//            value.it_value.tv_usec = 500000;
//            value.it_interval.tv_sec = 0;
//            value.it_interval.tv_usec = 500000;
//            setitimer(ITIMER_REAL, &value, NULL);
//        }else{
////            value.it_value.tv_sec = 0;
////            value.it_value.tv_usec = 0;
////            value.it_interval.tv_sec = 0;
////            value.it_interval.tv_usec = 0;
////            setitimer(ITIMER_REAL, &value, NULL);
//            break;
//        }
//        i++;
//    }
}

/*设置一个定时器，进程在用户态下执行1秒钟后发出首次信号，以后进程每在用户态下执行3秒钟，发送一个信号*/
void timefunc3(int sig)
{
    fprintf(stderr, "signal %d\n", number++);
    signal(SIGPROF, timefunc3);
}

void main5(int argc, const char * argv[])
{
    signal(SIGVTALRM, timefunc3);
    struct itimerval value;
    value.it_value.tv_sec = 10;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &value, NULL);
    
    while (1) {
        ;
    }
}

/*取消一个ITIMER_PROF类定时器*/
void timefunc4(int sig)
{
    fprintf(stderr, "signal %d\n", number++);
    signal(SIGPROF, timefunc4);
}

void main6(int argc, const char * argv[])
{
    signal(SIGVTALRM, timefunc4);
    struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF, &value, NULL);
}

/*设置一个定时1.5秒的真实时间定时器，它仅发送一次信号就自动取消*/

void timefunc5(int sig)
{
    fprintf(stderr, "signal %d\n", number++);
    signal(SIGPROF, timefunc5);
}

void main7(int argc, const char * argv[])
{
    signal(SIGPROF, timefunc5);
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 500000;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_PROF, &value, NULL);
    while (1) {
        ;
    }
}

void timefunc6(int sig)
{
    fprintf(stderr, "signal %d\n", number++);
    signal(SIGPROF, timefunc6);
}

void main8(int argc, const char * argv[])
{
    signal(SIGPROF, timefunc6);//捕获信号
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 500000;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 500000;
    setitimer(ITIMER_PROF, &value, NULL);//定时开始
    while (1) {
        ;
    }
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
//    main3(argc, argv);
//    main4(argc, argv);
//    main5(argc, argv);
//    main6(argc, argv);
//    main7(argc, argv);
    main8(argc, argv);
    return 0;
}

