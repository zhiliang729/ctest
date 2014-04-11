//
//  main.c
//  ConcurrentProgramming
//
//  Created by zhangliang on 14-4-10.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>

int main1(int argc, const char * argv[])
{
    int i;
	fprintf(stderr, "argc=[%d]\n", argc);
	for (i=0; i<argc; i++)
	{
		fprintf(stderr, "argv[%d]=[%s]\n\n", i, argv[i]);
	}
    return 0;
}

#include<unistd.h>

void main2(int argc, const char * argv[])
{
    printf("pid=[%d], gid=[%d], ppid=[%d]\n", getpid(), getpgrp(), getppid());
	printf("uid=[%d], euid=[%d], gid=[%d], egid=[%d]\n", getuid(), geteuid(), getgid(), getegid());
}

extern char * * environ;
void main3(int argc, const char * argv[])
{
    char * * p = environ;
    while (* p) {
        fprintf(stderr, "%s\n", * p);
        p++;
    }
}

#include <stdlib.h>

void main4(int argc, const char * argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
        fprintf(stderr, "%s = %s \n", argv[i], getenv(argv[i]));
    }
}

void main5(int argc, const char * argv[])
{
    pid_t pid;
    int i = 0;
    printf("888888888888888888888888888888888\n");
    fprintf(stderr, "Process Begin. pid = [%d], i = [%d]\n", getpid(), i);
    if ((pid = fork()) > 0) {
        fprintf(stderr, "Parent pid = [%d], child pid = [%d]\n", getpid(), pid);
    }else if(pid == 0){
        fprintf(stderr, "child pid = [%d]   啊  [%d]\n", getpid(), pid);
        i++;
    }else{
        fprintf(stderr, "Fork failed.\n");
    }
    fprintf(stderr, "Process End. pid = [%d], i = [%d]\n", getpid(), i);
}

void main6(int argc, const char * argv[])
{
    fprintf(stderr, "--begin--\n");
    execl("/bin/ls", "-a", 0);
    fprintf(stderr, "--end--\n");
}

void main7(int argc, const char * argv[])
{//fork----exec模式
    pid_t pid;
    if ((pid = fork()) == 0) {//子进程
        main6(argc, argv);
    }else if(pid > 0){//父进程
        fprintf(stderr, "fork child pid = [%d]\n", pid);
    }else//调用失败
        fprintf(stderr, "Fork failed.\n");
}

void main8(int argc, const char * argv[])
{//vfork----exec模式
    //大多数情况下，fork--exec模型再创建子进程后立即执行新的程序，并不使用父进程的数据，因此unix提供了一个更快的函数vfork，原型为
    //#include <unistd.h>
    //pid_t vfork();

    
    
}


int main(int argc, const char * argv[])
{
    main1(argc, argv);
    main2(argc, argv);
    main3(argc, argv);
    main4(argc, argv);
    main5(argc, argv);
    
    printf("999999999999999999999999999\n");
    main7(argc, argv);
    return 0;
}

