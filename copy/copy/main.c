//
//  main.c
//  copy
//
//  Created by zhangliang on 13-12-17.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "apue.h"

//eg 1-2
//const int BUFFSIZE = 4096;
//
//int main(int argc, const char * argv[])
//{
//    ssize_t n;
//    char buf[BUFFSIZE];
//    
//    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
//        if (write(STDOUT_FILENO, buf, n) != n) {
//            err_sys("write error");
//        }
//    }
//    
//    if (n > 0) {
//        err_sys("read error");
//    }
//    return 0;
//}

//eg 1-3
//int main(int argc, char * argv[])
//{
//    int c;
//    while ((c = getc(stdin)) != EOF) {
//        if (putc(c, stdout) == EOF) {
//            err_sys("output error!");
//        }
//    }
//    
//    if (ferror(stdin)) {
//        err_sys("input error!");
//    }
//    return 0;
//}


//eg 1-4
//int main(int argc, char * argv[])
//{
//    printf("hello world from process id %d\n", getpid());
//    return 0;
//}


//eg 1-5
//#include <sys/wait.h>
//
//int main(int argc, char * argv[])
//{
//    char buf[MAXLINE];
//    pid_t pid;
//    int status;
//    
//    printf("%% ");
//    while (fgets(buf, MAXLINE, stdin) != NULL) {
//        if (buf[strlen(buf) - 1] == '\n') {
//            buf[strlen(buf) - 1] = 0;
//        }
//        
//        if ((pid = fork()) < 0) {
//            err_sys("fork error");
//        }else if(pid == 0){
//            execlp(buf, buf, (char *)0);
//            err_ret("couldn't execute : %s", buf);
//            exit(127);
//        }
//        
//        if ((pid = waitpid(pid, &status, 0)) < 0) {
//            err_sys("waitpid error");
//        }
//        printf("%% ");
//    }
//    return 0;
//}

//eg 1-6
//#include <errno.h>
//int main(int argc, char * argv[])
//{
//    fprintf(stderr, "EACCES:%s\n", strerror(EACCES));
//    errno = ENOENT;
//    perror(argv[0]);
//    return 0;
//}

//eg 1-7 打印用户id和组id
//int main(int argc, char * argv[])
//{
//    printf("uid = %d, gid = %d\n", getuid(), getgid());
//    return 0;
//}


#include <sys/wait.h>
static void sig_int(int);

int main(int argc, char * argv[])
{
    char buf[MAXLINE];
    pid_t pid;
    int status;
    
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }
    
    printf("%% ");
    
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }
        
        if ((pid = fork()) < 0) {
            
        }
    }
}

