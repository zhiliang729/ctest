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
 
 */
int main(int argc, const char * argv[])
{
    main1(argc, argv);
    return 0;
}


