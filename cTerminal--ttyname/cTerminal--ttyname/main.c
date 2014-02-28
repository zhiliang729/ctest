//
//  main.c
//  cTerminal--ttyname
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

/*
 ttyname()—返回一终端机名称
 
 此函数在编程工具和terminal上运行结果是不同的！
 相关函数：Isatty
 
 头文件：#include <unistd.h>
 
 定义函数：char * ttyname(int desc);
 
 函数说明：如果参数desc 所代表的文件描述词为一终端机, 则会将此终端机名称由一字符串指针返回, 否则返回NULL.
 
 返回值：如果成功则返回指向终端机名称的字符串指针, 有错误情况发生时则返回NULL.
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

#define ISATTY 0


int main(int argc, const char * argv[])
{
    int fd;
#if ISATTY
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(isatty(fd)){
#else
    char *file = "/dev/tty";
    fd = open(file, O_RDONLY);
    printf("%s\n", file);
    int result;
    result = isatty(fd);
    if (result) {
#endif
        printf("is a tty .\n");
        printf("ttyname = %s \n", ttyname(fd));
    }else{
        printf("is not a tty.\n");
    }
    
    close(fd);
    
    return 0;
}

