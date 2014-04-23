//
//  msgrcv.c
//  IPC__messageQueue
//
//  Created by zhangliang on 14-4-22.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

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
    ssize_t ret;
    /*打开（或创建）消息队列*/
    if ((msgid = msgget(0x1234, 0666 | IPC_CREAT)) < 0) {
        fprintf(stderr, "open msg %x failed.\n", 0x1234);
        return 0;
    }
    
    while (strncmp(buf.ctext, "exit", strlen("exit")) != 0) {
        memset(&buf, 0, sizeof(buf));/*清空消息缓冲区*/
        while ((ret = msgrcv(msgid, &buf, sizeof(buf), msgType, 0)) < 0) {//小于0则接收失败处理
            printf("error:%d", errno);
            if (errno == EINTR) {
                continue;//信号中断，重新接收
            }
            return 0;//否则退出
        }
        fprintf(stderr, "Msg: Type = %ld, Len = %ld, Text:%s\n", buf.mtype, ret, buf.ctext);//返回的消息长度比实际的多1，应为字符串末尾还有一个“\n”
    }
    return 0;
}

