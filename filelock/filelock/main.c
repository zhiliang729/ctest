//
//  main.c
//  filelock
//
//  Created by zhangliang on 14-4-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define AFILE "/Users/zhangliang/Desktop/ctest/filelock"

/*查询描述符fd对应文件从偏移start处开始的len字节中的锁信息*/
void SeeLock(int fd, int start, int len);

/*以阻塞模式在文件描述符fd对应的文件中申请共享读锁， 锁定的区域为从偏移start处开始的len字节*/
void GetReadLock(int fd, int start, int len);

/*以阻塞模式在文件描述符fd对应的文件中申请互斥锁，锁定的区域为从偏移start处开始的len字节*/
void GetWriteLock(int fd, int start, int len);

/*释放fd对应文件中从偏移start处开始的len字节的文件锁*/
void ReleaseLock(int fd, int start, int len);

int main(int argc, const char * argv[])
{
    int fd;
    if ((fd = open(AFILE, O_RDWR | O_CREAT, 0755)) < 0 ) {
        fprintf(stderr, "open file failed.\n");
        return -1;
    }
    
    SeeLock(fd, 0, 10);
    GetReadLock(fd, 0, 10);
    SeeLock(fd, 11, 20);
    GetWriteLock(fd, 11, 20);
    
    sleep(30);
    
    ReleaseLock(fd, 0, 10);
    ReleaseLock(fd, 11, 20);
    
    return 0;
}

void SeeLock(int fd, int start, int len)
{
    struct flock arg;
    arg.l_type = F_WRLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    if (fcntl(fd, F_GETLK, &arg) == -1) {
        fprintf(stderr, "See Lock failed.\n");
    }else if (arg.l_type == F_UNLCK) {
        fprintf(stderr, "No lock from %d to %d.\n", start, len);
    }else if (arg.l_type == F_WRLCK) {
        fprintf(stderr, "Write lock from %d to %d, id = %d.\n", start, len, arg.l_pid);
    }else if (arg.l_type == F_RDLCK) {
        fprintf(stderr, "Read lock from %d to %d, id = %d.\n", start, len, arg.l_pid);
    }
}

void GetReadLock(int fd, int start, int len)
{
    struct flock arg;
    arg.l_type = F_RDLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    if (fcntl(fd, F_SETLKW, &arg) == -1) {
        fprintf(stderr, "[%d] Set Read Lock failed.\n", getpid());
    }else{
        fprintf(stderr, "[%d] Set Read Lock From %d to %d.\n", getpid(), start, len);
    }
}

void GetWriteLock(int fd, int start, int len)
{
    struct flock arg;
    arg.l_type = F_WRLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    if (fcntl(fd, F_SETLKW, &arg) == -1 ) {
        fprintf(stderr, "[%d] Set Write Lock failed.\n", getpid());
    }else{
        fprintf(stderr, "[%d] Set Write Lock Form %d to %d.\n", getpid(), start, len);
    }
}

void ReleaseLock(int fd, int start, int len)
{
    struct flock arg;
    arg.l_type = F_UNLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    if (fcntl(fd, F_SETLKW, & arg) == -1) {
        fprintf(stderr, "[%d] Unlock failed.\n", getpid());
    }else{
        fprintf(stderr, "[%d] Unlock From %d to %d.\n", getpid(), start, len);
    }
}
