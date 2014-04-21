//
//  main.c
//  jmp
//
//  Created by zhangliang on 14-4-18.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 unix中跳转函数原型：
 #include <setjmp.h>
 int setjmp(jmp_buf env);
 void longjmp(jmp_buf env, int val);
 
    函数setjmp存储当前的堆栈环境（包括程序的当前执行位置）到参数env中，当函数正常调用成功时返回0.函数longjmp恢复保存在env中的堆栈信息，并使程序转移到env中保存的位置处重新执行。这两个函数联合使用，可以实现程序的重复执行。
    函数longjmp调用成功后，程序转移到函数setjmp处执行，函数setjmp返回val。如果参数val取值为0，为了与上次正常调用setjmp相区别，函数setjmp将自动返回1.
    在unix中使用全局跳转的步骤如下：
    1.定义jmp_buf变量，如
            #include <setjmp.h>
            jmp_buf env;
    2.在需要程序重复执行的位置调用setjmp，如：
            i = setjmp(env);
    3.在调用setjmp后增加判断代码，确认程序的执行次数，避免产生死循环。例如以下语句实现程序回退一次后退出的功能：
        if(i != 0) exit(0);
    4.在程序开始回退的位置调用longjmp，如：
        longjmp(env, 1)；
 
跳转函数的最典型应用是与时钟信号相结合，完成输入输出的超时操作
*/

/*
 单进程I/O超时处理
    unix的I/O超时处理是一个常见的问题，它的通常做法是：接收输入（或发送输出）后立刻返回，如果无输入（或输出）则n秒钟后定时返回。比如银行柜台等待客户输入密码时，如果有输入则立刻返回，否则1分钟后超时返回，程序继续。
    一般情况下，处理unix中I/O超时的方式有终端方式、信号跳转方式和多路复用方式等多种。
 */


/*使用跳转语句的例子，跳转两次后退出*/

#include <setjmp.h>
int j = 0;
jmp_buf env;


int main1(int argc, const char * argv[])
{
    int i, k = 0;
    i = setjmp(env);
    printf("setjmp = [%d], j = [%d]; k = [%d]\n", i, j++, k++);
    if (j > 10) {
        exit(0);
    }
    sleep(1);
    int val = j;
    longjmp(env, val);
    printf("\nhelllo\n");
    return 0;
}


/*
    定时I/O操作的例子，它从文件描述符0（标准文件输入流）中读取一个字符，当有输入时继续，或者3秒钟后超时退出，并打印超时信息
 */

/*
 1 终端I/O超时方式
    利用ioctl函数，设置文件描述符对应的标准输入文件属性为“接受输入后立刻返回，如果无输入则10秒后定时返回模式”
 */
//#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
void main2(int argc, const char * argv[])
{
    struct termios old, new;
    char c = 0;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~ICANON;//原始模式输入（即键入即输出，不等回车）
    new.c_cc[VMIN] = 0;
    new.c_cc[VTIME] = 30;/*设置文件0的超时时间为3秒*/
    tcsetattr(0, TCSANOW, &new);
    if (read(0, &c, 1) != 1) {//读取字符
        printf("timeout!\n");//打印超时
    }else{
        printf("\n%d\n", c);//打印输入字符的ascii码
    }
    tcsetattr(0, TCSANOW, &old);
}


/*
 2. 信号与跳转I/O超时方式
    在read函数前调用setjmp保存堆栈数据并使用alarm设定3秒定时器。倘若输入超时则必然接收定时信号，此时只让longjmp函数从setjmp处返回，并跳过read语句即可。
 */
//#include <setjmp.h>
//#include <stdio.h>
//#include <unistd.h>
#include <signal.h>

//程序采用变量time1记载超时标志，并且分别编写了正常处理和超时处理   1.首次进入程序，执行正常处理代码段； 2输入超时，触发定时器事件，更改time1标志再次进入程序，执行超时处理代码段。
int time1 = 0;
jmp_buf env;

void timefunc1(int sig)/*定时事件*/
{
    time1 = 1;
    longjmp(env, 1);
}

void main3(int argc, const char * argv[])
{
    char c;
    signal(SIGALRM, timefunc1);//捕获定时信号
    setjmp(env);
    if (time1 == 0) {//正常处理
        alarm(3);//定时3秒
        read(0, &c, 1);//读取字符
        alarm(0);//取消定时
        printf("%d\n", c);//打印输入
    }else{
        printf("timeout!\n");//打印超时
    }
}

/*
 
 3.多路复用I/O超时方式
    一个进程可能同时打开多个文件，unix中函数select可以同时监控多个文件描述符输入输出，进程将一直阻塞，直到超时或者产生I/O为止，此时函数返回，通知进程读取或发送数据。
    函数select函数原型如下：
    #include <sys/types.h>
    #include <sys/times.h>
    #include <sys/select.h>
    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set * exceptfds, struct timeval * timeout);
    FD_CLR(int fd, fd_set * fdset);
    FD_ISSET(int fd, fd_set * fdset);
    FD_SET(int fd, fd_set * fdset);
    FD_ZERO(fd_set * fdset);
 
    参数nfds是select监控的文件描述符的数量，一般为监控的最大描述符编号加1.比如将空标准输入（文件描述符0），标准输出（文件描述符1），标准错误输出（文件描述符2）三个文件时，nfds应赋值为3.当不知道监控的最大描述符编号时，可以使用常数FD_SETSIZE.
    类型fd_set是文件描述符集合，其元素为监控的文件描述符。参数readfds是监控的可读文件描述符集合，参数writefds是监控的可写文件描述符集合，参数exceptfds是监控的异常文件描述符集合。当readfds中任意文件可读、writefds中任意文件可写，execptfds中任意文件异常或者定时器到达时，函数select返回。
    unix 中给定了一组以"FD"开头的宏操作文件描述符集合fd_set，其含义如下：
        宏           含义
        FD_ISSET     检查文件描述符集合fdset中的成员fd是否就绪，如果就绪则返回值大于0
        FD_CLR       从文件描述符集合fdset中删除文件描述符fd
        FD_SET       在文件描述符集合fdset中增加文件描述符fd
        FD_ZERO      清空文件描述符集合fdset中的元素
    参数timeout是描述精确时间的timeval结构，它确定了函数的超时时间，有三种取值情况：
        timeout         含义
        NULL            函数永远等待，直到有文件描述符就绪
        0               函数不等待，检查文件描述符状态后立刻返回
        其他值           函数等待文件描述符，或者定时完成时返回
    函数select将返回文件描述符结合中已准备好的文件总个数，如果在任意文件描述符准备好之前，定时到达，函数返回0，如果出错，返回-1.
    函数select返回就绪文件描述符数量后，必须执行read等输入函数，否则函数将继续返回就绪文件数。？？？
 */

//多路复用方式超时处理    进程定时timeout秒，并监控标准输入文件（描述符0）。如果有输入则读取输入，否则进程一直阻塞到定时完成，函数返回。
//技巧：  当参数readfds、writefds和execptfds都为NULL时，函数select退化成可精确到微秒级的sleep调用，如：select（0， NULL, NULL, NULL, &timeout）;
#include <sys/types.h>
#include <sys/times.h>
#include <sys/select.h>

void main4(int argc, const char * argv[])
{
    struct timeval timeout;
    fd_set readfds;
    int i;
    char c;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);/*监控文件0*/
    i = select(1, &readfds, NULL, NULL, &timeout);
    if (i > 0) {//文件0就绪
        read(0, &c, 1);
        printf("%d\n", c);
    }else if (i == 0){
        printf("timeout\n");//超时处理
    }else{
        printf("error!\n");
    }
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
//    main3(argc, argv);
    main4(argc, argv);
    return 0;
}


