//
//  main.c
//  File-open
//
//  Created by zhangliang on 13-12-20.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

//#include <stdio.h>
//#include <sys/fcntl.h>

//int	open(const char * pathname, int, .../* mode */);
//int creat(const char * pathname, mode_t mode); //等效于open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
//ssize_t read(int filedes, void * buf, size_t nbytes);

//int main(int argc, const char * argv[])
//{
//    
//    // insert code here...
//    printf("Hello, World!\n");
//    return 0;
//}


#include "apue.h"

// eg 1
//int main(int argc, char * argv[])
//{
//    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == EOF) {
//        printf("cannot seek\n");
//    }else{
//        printf("seek ok \n");
//    }
//
//    exit(0);
//}

//eg 2
//#include <sys/fcntl.h>
//
//char buf1[] = "abcdefghij";
//char buf2[] = "ABCDEFGHIJ";
//
//int main(int argc, char * argv[])
//{
//    int fd;
//    
//    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
//        err_sys("creat error");
//    }
//    
//    if (write(fd, buf1, 10) != 10) {
//        err_sys("buf1 write error");
//    }
//    //offset now = 10
//    
//    if (lseek(fd, 16384, SEEK_SET) == -1) {
//        err_sys("lseek error");
//    }
//    //offset now = 16384
//    
//    if (write(fd, buf2, 10) != 10) {
//        err_sys("buf2 write error");
//    }
//    //offset now = 16394
//    
//    return 0;
//}


//eg 3
//#define BUFFSIZE 4096
//#include <unistd.h>
//int main(int argc, char * argv[])
//{
//    ssize_t n;
//    char buf[BUFFSIZE];
//
//    
//    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
//        if ((write(STDOUT_FILENO, buf, n)) != n) {
//            err_sys("write error");
//        }
//    }
//    
//    if (n < 0) {
//        err_sys("read error");
//    }
//    return 0;
//}


//eg 4
//#include <fcntl.h>
//
//int main(int argc, char * argv[])
//{
//    int val;
//    if (argc != 2) {
//        err_quit("usage : %s <descriptor#>", argv[0]);
//    }
//    
//    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
//        err_sys("fctnl error for fd %d", atoi(argv[1]));
//    }
//    
//    switch (val & O_ACCMODE) {
//        case O_RDONLY:
//            printf("read only");
//            break;
//        case O_WRONLY:
//            printf("write only");
//            break;
//        case O_RDWR:
//            printf("read and write");
//            break;
//        default:
//            err_dump("unknown access mode");
//            break;
//    }
//    
//    if (val & O_APPEND) {
//        printf(", append");
//    }
//    if (val & O_NONBLOCK) {
//        printf(", nonblocking");
//    }
//    
//#ifdef O_SYNC
//    if (val & O_SYNC) {
//        printf(", synchronous writes");
//    }
//#endif
//    
//
//    if (val & O_FSYNC) {
//        printf(", synchronous writes");
//    }
//    
//    putchar('\n');
//    return 0;
//}

#include <fcntl.h>

//对一个文件描述符打开一个或多个文件状态标志
void set_f1(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl F_GETFL error");
    }
    
    val |= flags;//turn on flags
    
    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("fcntl F_SETFL error");
    }
}

void clr_f1(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl F_GETFL error");
    }
    
    val &= ~flags;//turn flags off
    
    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("fcntl F_SETFL error");
    }
}


int main(int argc ,char * argv[])
{
    set_f1(0, 1);
    return 0;
}

