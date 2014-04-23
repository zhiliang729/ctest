//
//  ipcmsg.c
//  IPC__messageQueue
//
//  Created by zhangliang on 14-4-23.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/msg.h>


char * GetFileMode(mode_t st_mode, char *resp)
{
	memset(resp, '-', 9);
	if (st_mode & S_IRUSR) resp[0] = 'r';
	if (st_mode & S_IWUSR) resp[1] = 'w';
	if (st_mode & S_IXUSR) resp[2] = 'x';
	if (st_mode & S_IRGRP) resp[3] = 'r';
	if (st_mode & S_IWGRP) resp[4] = 'w';
	if (st_mode & S_IXGRP) resp[5] = 'x';
	if (st_mode & S_IROTH) resp[6] = 'r';
	if (st_mode & S_IWOTH) resp[7] = 'w';
	if (st_mode & S_IXOTH) resp[8] = 'x';
	resp[9] = 0;
	return resp;
}

void StatMsg(int msgid)
{
	char resp[20];
	struct msqid_ds abuf;		/*申请消息队列结构缓冲区*/
	memset(&abuf, 0, sizeof(abuf));	/*置空消息队列结构缓冲区*/
	msgctl(msgid, IPC_STAT, &abuf);	/*读取消息队列结构*/
	/*转化消息队列结构并打印*/
	fprintf(stderr, "T     ID        KEY       MODE      OWNER      GROUP     CBYTES       QNUM     QBYTES\n");
	fprintf(stderr, "q %6d %#10.8x %10s %10d %10d %10ld %10ld %10ld\n",
            msgid, 					/*标识号*/
            abuf.msg_perm._key, 			/*关键字*/
            GetFileMode(abuf.msg_perm.mode, resp), 	/*访问权限*/
            abuf.msg_perm.uid, 			/*有效用户ID*/
            abuf.msg_perm.gid, 			/*有效组ID*/
            abuf.msg_cbytes, 			/*队列当前的消息字节数*/
            abuf.msg_qnum, 				/*队列当前的消息个数*/
            abuf.msg_qbytes);			/*队列可以容纳的最大字节数*/
}

//类似于命令ipcs和ipcrm的程序，从命令行参数中获取要执行的操作，包括创建消息队列、读取消息队列信息和删除消息队列等，
int main(int argc, const char * argv[])
{
    int msgid;
    if (argc != 2 && argc != 3) {
        return 0;
    }
    
    msgid = atoi(argv[1]);
    if (argc == 2) {//查询消息队列信息
        StatMsg(msgid);
    }else if(argc == 3 && strcmp(argv[2], "c") == 0){
        //创建消息队列
        if (msgget(msgid, 0666 | IPC_CREAT | IPC_EXCL) < 0) {
            fprintf(stderr, "create msg %#10.8x failed.\n", msgid);
        }else{
            fprintf(stderr, "create msg %#10.8x success.\n", msgid);
        }
    }else if(argc == 3 && strcmp(argv[2], "d") == 0){
        //删除消息队列
        if (msgctl(msgid, IPC_RMID, NULL) < 0) {
            fprintf(stderr, "Delete msg %d failed.\n", msgid);
        }else{
            fprintf(stderr, "Delete msg %d success.\n", msgid);
        }
    }
    return 0;
}
