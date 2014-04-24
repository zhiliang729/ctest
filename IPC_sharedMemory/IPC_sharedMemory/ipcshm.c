//
//  ipcshm.c
//  IPC_sharedMemory
//
//  Created by zhangliang on 14-4-24.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define VERIFYERR(a, b)\
                if (a) {\
                    fprintf(stderr, "%s failed.\n", b);\
                    return -1;\
                }else {\
                    fprintf(stderr, "%s success.\n", b);\
                }

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

void StatShm(int shmid)
{
    char resp[20];
    struct shmid_ds buf;
    memset(&buf, 0, sizeof(buf));/*置空消息队列结构缓冲区*/
    shmctl(shmid, IPC_STAT, &buf);
    /*转化共享内存结构并打印*/
    fprintf(stderr, "T     ID        KEY       MODE      OWNER      GROUP     NATTCH       SEGSZ      CPID       LPID\n");
	fprintf(stderr, "m %6d %#10.8x %10s %10d %10d %10d %10ld %10d %10d\n",
            shmid, 					/*标识号*/
            buf.shm_perm._key, 			/*关键字*/
            GetFileMode(buf.shm_perm.mode, resp), 	/*访问权限*/
            buf.shm_perm.uid, 			/*有效用户id*/
            buf.shm_perm.gid, 			/*有效组id*/
            buf.shm_nattch,				/*共享内存链接数*/
            buf.shm_segsz, 				/*共享内存字节数*/
            buf.shm_cpid, 				/*创建共享内存的用户id*/
            buf.shm_lpid);				/*最近一次操作共享内存的用户id*/
}

int main(int argc, char * argv[])
{
    int shmid;
    int size;
    
    if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "help") == 0))) {
        fprintf(stderr, "Usage:  %s key v/d/10.\n", argv[0]);
        return -1;
    }
    
    if (argc != 3) {
        return -1;
    }
    shmid = atoi(argv[1]);
    if (strcmp(argv[2], "v") == 0) {//查询共享内存
        StatShm(shmid);
    }else if (strcmp(argv[2], "d") == 0){//删除共享内存
        VERIFYERR(shmctl(shmid, IPC_RMID, NULL) < 0, "Delete shm");
    }else {//创建共享内存
        size = atoi(argv[2]);
        VERIFYERR(shmget(shmid, size, 0666 | IPC_CREAT | IPC_EXCL) < 0, "Create shm");
    }
    return 0;
}



