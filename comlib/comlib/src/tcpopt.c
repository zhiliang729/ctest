//
//  tcpopt.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int optname[] = 	/*opt内容*/
{
	SO_DEBUG,	SO_REUSEADDR,	SO_REUSEPORT,	SO_KEEPALIVE,
	SO_DONTROUTE,	SO_BROADCAST,	SO_OOBINLINE,	SO_SNDBUF,
	SO_RCVBUF,	SO_SNDLOWAT,	SO_RCVLOWAT,	SO_TYPE,
	SO_ERROR
};

char optstr[][20] = 	/*opt描述，打印提示用*/
{
	"SO_DEBUG",	"SO_REUSEADDR",	"SO_REUSEPORT",	"SO_KEEPALIVE",
	"SO_DONTROUTE",	"SO_BROADCAST",	"SO_OOBINLINE",	"SO_SNDBUF",
	"SO_RCVBUF",	"SO_SNDLOWAT",	"SO_RCVLOWAT",	"SO_TYPE",
	"SO_ERROR"
};

/*
 本处设计一个读取套接字选项取值的例子，它首先创建一个TCP套接字但不命名，在建立连接，然后获取它全部的SOL_SOCKET层次选项取值，并打印结果
 */
int main12(int argc, char * argv[])
{
    int nSock;
	int i, result;
    socklen_t nSize;
	struct linger ling;		/* 选项SO_LINGER传输数据结构 */
	struct timeval val1, val2;	/* 发送、接收超时时间数据结构 */
	ASSERT((nSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);	/* 创建TCP套接字*/
	for (i=0; i<sizeof(optname)/sizeof(4); i++)
	{
		result = -1;
		nSize = sizeof(result);
		/*获取套接字选项取值，并存储到result中*/
		getsockopt(nSock, SOL_SOCKET, optname[i], &result, &nSize);
		printf("%s:%d\n", optstr[i], result);
	}
	/*----------------获取套接字选项SO_LINGER取值，并存储到ling中–-------- */
	nSize = sizeof(ling);
	getsockopt(nSock, SOL_SOCKET, SO_LINGER, &ling, &nSize);
	printf("SO_LINGER:%d.%d\n", ling.l_onoff, ling.l_linger);
	/*----------------获取套接字选项SO_SNDTIMEO取值，并存储到结构val1中–-------- */
	nSize = sizeof(val1);
	getsockopt(nSock, SOL_SOCKET, SO_SNDTIMEO, &val1, &nSize);
	printf("SO_SNDTIMEO:%ld.%d\n", val1.tv_sec, val1.tv_usec);
	/*----------------获取套接字选项SO_RCVTIMEO取值，并存储到结构val2中–-------- */
	nSize = sizeof(val2);
	getsockopt(nSock, SOL_SOCKET, SO_RCVTIMEO, &val2, &nSize);
	printf("SO_RCVTIMEO:%ld.%d\n", val2.tv_sec, val2.tv_usec);
    return 0;
}
