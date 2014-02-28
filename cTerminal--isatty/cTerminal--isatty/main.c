//
//  main.c
//  cTerminal--isatty
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//


/*
 
 文件描述符fd，打开的文件都有一个fd，fd对应内存中文件管理数据结构，也就是很多国人写的教材中所谓的FCB（文件控制块），类unix系统的文件IO操作有关的系统调用都使用fd作为其参数，如write，read，dup，dup2，fcntl和访问文件属性信息的函数之类的，因此，fd类似windows平台的文件操作API所使用的文件句柄。c语言中文件io操作的库函数是基于FILE结构，FILE只是对file descriptor做了封装。记住 stdin对应的fd为0，stdout为1，stderr为2，如果你打开的文件的fd<3,说明出错了。fd就是整型数，这个对于编程者是透明的，只要操作系统理解它的含义。
 
 函数名: isatty
 功 能: 检查设备类型 ， 判断文件描述词是否是为终端机, 。
 用 法: int isatty(int handle);
 返回值：如果参数desc所代表的文件描述词为一终端机则返回1，否则返回0。
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#define ISATTY 0
int main(int argc, const char * argv[])
{
    int  handle;
    handle = fileno(stdout);

#if ISATTY
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (isatty(clientfd)) {
#else
    if (handle) {
#endif
        printf("Handle %d is a device type \n", handle);
    }else{
        printf("Handle %d isn't a device type \n", handle);
    }
#if ISATTY
    close(clientfd);
#endif
    return 0;
}

