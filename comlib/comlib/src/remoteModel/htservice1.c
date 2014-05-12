//
//  htservice1.c
//  comlib
//
//  Created by zhangliang on 14-5-12.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
用户控制升级
客户端必须传送给服务器端分配的用户名和口令，服务器端进程在核实无误后，才可以调用程序控制进程
 
 客户端与服务器建立连接后，必须马上发送授权报文，以提供服务器验证用户信息
 报文格式为：
    user|password
如用户zyx，完整报文为
  0012zyx|password
 */


/*套接字通信中一个难点是通信双方无法判断一次通信报文是否结束，为了处理这一难题，远程调用模型中将使用一种显式长度报文的通信报文：  报文长度+报文内容   报文长度占4字节，*/
/*
 发送报文
 将数据打包成“报文长度+报文内容”的格式后，写入文件nFile中。  数据存储在szMsg指向的缓冲区中，共计nSize字节。函数调用成功时返回0，失败时返回其他值*/
int SetFileLen(int nFile, char *szMsg, ssize_t nSize)
{
	char szMsgSize[4 + 1];
	memset(szMsgSize, 0, sizeof(szMsgSize));
	sprintf(szMsgSize, "%04ld", nSize);			/* 报文长度组包 */
	if (VERIFY(WriteFile(nFile, szMsgSize, 4) == 0) && VERIFY(WriteFile(nFile, szMsg, nSize) == 0))	/* 分两次分别输出报文长度和内容，在某些应用中也可以一次性输出 */
		return 0;
	return 1;
}

/*
 接收报文
 接收格式为：“报文长度+报文内容”的报文，它首先接收4个字节的数据，再通过函数atoi将之转化为一个整数，这个整数就是后续报文的长度，然后再读取相应长度的报文即可。nfile是输入报文的描述符，szMsg制定了存储报文的缓冲区，参数nSize传入该缓冲区的最大容量，回传实际读入的报文长度。函数调用成功时返回0，否则返回其他值
 */
int GetFileLen(int nFile, char *szMsg, ssize_t *nSize)
{
	char szMsgSize[4 + 1];
	ssize_t nLen = 4, nLen1;
	memset(szMsgSize, 0, sizeof(szMsgSize));		/* 读取4字节报文 */
	ASSERT((ReadFile(nFile, szMsgSize, &nLen) == 0 && nLen == 4))
	nLen = nLen1 = atoi(szMsgSize);					/* 转化为后续报文长度 */
    /* 读取后续报文 */
	ASSERT((ReadFile(nFile, szMsg, &nLen) == 0 && nLen == nLen1));
	*nSize = nLen;									/* 回传后续报文长度 */
    PrintTraceLog("szMsg:%s", szMsg);
	return 0;
}


/*
 服务器端主函数
 服务器端进程在后台以守护进程的方式进行，采用了多进程并发套接字模型。它首先创建侦听套接字（侦听套接字的端口号从配置文件中读取），然后等待客户端的连接申请。当客户端发起连接申请时，它创建子进程，并由子进程负责客户端进程套接字通信，父进程则继续等待客户端的连接申请）
 */

#define INIFILE "/Users/zhangliang/Desktop/Project/C/comlib/comlib/src/remoteModel/config.ini"

int ChildExec(int nFile);
int SetAuth(int nFile);

int bShutdown = 0;
int main()
{
	int nListenSock = 0, nSock = 0;
	int nPort;
	pid_t _pid_tChild;
	ASSERT(GetINIConfigValue(INIFILE, "LOCAL", "PORT", &nPort, STRINT) == 0);/* 读取套接字侦听端口号 */
    
//	ASSERT(InitServer() == 0)							/* 进程转后台运行 */
	ASSERT(CreateSock(&nListenSock, nPort, 8) == 0)	/* 创建侦听套接字 */
	PrintLog(stdout, "htpservice: service is activated.");
	while (!bShutdown)
	{													/* 创建连接套接字 */
		if (!VERIFY(AcceptSock(&nSock, nListenSock) == 0)) continue;
		VERIFY((_pid_tChild = fork()) >= 0);			/* 创建子进程 */
		if (_pid_tChild > 0)
		{
		    close(nSock);						/* 父进程关闭连接套接字 */
		    nSock = 0;
		    continue;
		}
		break;
	}
	close(nListenSock);							/* 子进程关闭侦听套接字 */
	nListenSock = 0;
    if (VERIFY(SetAuth(nSock) == 0)) {/*服务开始前先认证用户, 若不正确，则推出*/
        ChildExec(nSock);/* 子服务 */
    }
	close(nSock);								/* 服务结束，关闭连接套接字 */
	return 0;
}


/*
 子服务函数
 子服务函数是远程调用的核心处理函数，它负责解析客户端的请求报文，创建新shell进程，并把进程输出信息转发到客户端。
 */
int ChildExec(int nFile)
{
	char szMsg[4096 + 1], szLine[8192 + 1], *p;
	ssize_t  nSize;
	FILE *pfile;
	memset(szMsg, 0, sizeof(szMsg));
	ASSERT(GetFileLen(nFile, szMsg, &nSize) == 0);   /* 读取远程调用申请报文 */
	if (strncmp(szMsg, "pipe", 4) == 0)			/* 判断报文格式是否正确 */
	{
		p = TrimString(szMsg + strlen("pipe"));	/* p指向shell命令行 */
		PrintLog(stderr, p);
		/* 以上解析客户端请求报文，现p指向客户端请求远程调用的shell命令行 */
		if ((pfile = popen(p, "r")) == NULL)		/* 以管道形式运行进程 */
		{											/* 错误提示 */
            strcpy(szLine, "[E]:pipe failed!");
            nSize = strlen(szLine);
            ASSERT(SetFileLen(nFile, szLine, nSize) == 0);
		}
		else
		{									/* 读取shell进程的每一行输出 */
			memset(szLine, 0, sizeof(szLine));
			while (fgets(szLine, sizeof(szLine), pfile) != NULL)
			{
				nSize = strlen(szLine);
				PrintLog(stderr, "【%d】%s", nSize, szLine);
				/* 将shell进程的输出转发到客户端 */
				if (!VERIFY(SetFileLen(nFile, szLine, nSize) == 0)) break;
				memset(szLine, 0, sizeof(szLine));
            }
            pclose(pfile);					/* 关闭管道 */
		}
	}
	return 0;
}

/*
 授权函数
 函数setAuth验证客户端的授权报文是否正确。它读取客户端传送的用户名和密码，并从配置文件中查找对应的用户信息。如果配置文件中存在该用户且密码正确，则继续下一步远程程序调用；否则返回错误信息并结束函数。
 */

int SetAuth(int nFile)
{
	char szUser[100 + 1], szPass[100 + 1], szPass1[100 + 1], szBuf[200];
	char *nCeAddr[2], szError[] = "Bad User or Password!\n";
	ssize_t  nSize, nCeLen[2];
	STRRESVAR stuStr;
	memset(szBuf, 0, sizeof(szBuf));
	memset(szUser, 0, sizeof(szUser));
	memset(szPass, 0, sizeof(szPass));
	memset(szPass1, 0, sizeof(szPass1));
    nSize = sizeof(szUser);
	/* 读取授权报文的用户名和密码 */
	ASSERT(GetFileLen(nFile, szUser, &nSize)==0);
	stuStr.nAmount = 2;				/* 数据域总数 */
	stuStr.nFlag = 1;				/* 固定分隔 */
	stuStr.filedlen = nCeLen;		/* 每个域的长度 */
	stuStr.nCompartlen = 1;			/* 分隔符号的长度 */
	strcpy(stuStr.szCompart, "|");	/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;		/* 指向每个域首的指针 */
	strrespre(szBuf, &stuStr);		/* 解析字符串报文 */
	if (strresvalue(szBuf, stuStr, 0, szUser, STRSTR) != 0 ||
	    strresvalue(szBuf, stuStr, 1, szPass, STRSTR) != 0)/*解析配置文件出错*/
	{
		ASSERT(SetFileLen(nFile, szError, strlen(szError)) == 0);
		return 3;
	}
	TrimString(szUser);								/* 清除用户名中的空格 */
	TrimString(szPass);								/* 清除密码中的空格 */
	/* 从配置文件中读取用户及密码 */
	if (GetINIConfigValue(INIFILE,"USER",szUser,szPass1,STRSTR)==0 &&
        strcmp(szPass, TrimString(szPass1)) != 0 )/*读取配置文件中密码，并与用户传得密码比较，若连用户都没有，肯定出错*/
	{	/* 判断用户密码是否正确 */
		ASSERT(SetFileLen(nFile, szError, strlen(szError)) == 0);/*出错*/
		return 3;
	}
	return 0;
}




