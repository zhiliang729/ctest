//
//  main.c
//  IPC__messageQueue
//
//  Created by zhangliang on 14-4-22.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//


//发送消息的例子
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <string.h>
#include <unistd.h>

#include "define.h"


extern int errno;

struct mymsgbuf/*定义消息结构*/
{
    long mtype;/*消息类型*/
    char ctext[100];/*消息数据*/
};

int main(int argc, const char * argv[])
{
    struct mymsgbuf buf;/*申请消息缓冲*/
    int msgid;
    /*打开（或创建）消息队列*/
    if ((msgid = msgget(0x1234, 0666 | IPC_CREAT)) < 0) {
        fprintf(stderr, "open msg %x failed.\n", 0x1234);
        return 0;
    }
    
    while (strncmp(buf.ctext, "exit", strlen("exit")) != 0) {
        memset(&buf, 0, sizeof(buf));/*清空*/
        printf("Please input:\n");
        //从键盘输入消息数据内容
        fgets(buf.ctext, sizeof(buf.ctext), stdin);
        //设置消息类型为进程id
        buf.mtype = msgType;
        //发送消息
        while ((msgsnd(msgid, &buf, strlen(buf.ctext), 0)) < 0) {//小于0则发送失败处理
            printf("error:%d", errno);
            if (errno == EINTR) {
                continue;//信号中断，重新发送
            }
            return 0;//否则退出
        }
    }
    return 0;
}

