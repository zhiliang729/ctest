//
//  pid.c
//  comlib
//
//  Created by zhangliang on 14-4-15.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

void ClearChild(int nSignal)
{
    pid_t _pid_t;
    int nState;
    while ((_pid_t = waitpid(-1, &nState, WNOHANG)) > 0) {/*等待进程组识别码为1的任何子进程。结束状态保存在nState中，*/
        ;
    }
    signal(SIGCHLD, ClearChild);
}

int InitServer()
{
    pid_t pid1;
    ASSERT((pid1 = fork()) >= 0);/*创建子进程*/
    if (pid1 != 0) {/*父进程失败退出，子进程托管*/
        sleep(1);
        exit(0);
    }
    ASSERT(setsid() >= 0);/*子进程脱离终端*/
    umask(0);/*清除文件创建掩码*/
    signal(SIGINT, SIG_IGN);/*忽略SIGINT信号*/
    signal(SIGCHLD, ClearChild);/*处理SIGCHLD信号，预防僵死进程*/
    
    /*
     程序在接收到SIGCHLD信号后立即执行函数ClearChild，并调用非阻塞的waitpid函数接收子进程结束消息,
     如果接收到子进程结束信息则释放该子进程占用的进程表资源，否则函数立即返回。这样既保证了不增加守护进程
     负担，又成功地预防了僵死进程的产生。
     */
    return 0;
}

