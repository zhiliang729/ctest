//
//  tcp.c
//  comlib
//
//  Created by zhangliang on 14-4-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*应用于tcp通信服务器端，参数pSock回传创建的侦听套接字描述符，整型nPort指定套接字的侦听端口，该侦听套接字的最大连接数由nMax指定。本函数调用成功时完成套接字的创建，命名和侦听等准备工作并返回0，否则关闭套接字并返回其他值。*/
int CreateSock(int *pSock, int nPort, int nMaxConnect)
{
    struct sockaddr_in addrin;
    struct sockaddr * paddr = (struct sockaddr *)&addrin;
    ASSERT(pSock != NULL && nPort > 0 && nMaxConnect > 0);
    memset(&addrin, 0, sizeof(struct sockaddr_in));
    
    /*协议地址组包*/
    addrin.sin_family = AF_INET;/*协议名*/
    addrin.sin_addr.s_addr = htonl(INADDR_ANY);/*自动分配地址*/
    addrin.sin_port = htons(nPort);/*端口号*/
    
    /*创建 TCP 套接字描述符*/
    ASSERT((*pSock = socket(AF_INET, SOCK_STREAM, 0)) >= 0);/*命名套接字*/
    int onReUseAddr = 1,retAdd;
    retAdd = setsockopt(*pSock, SOL_SOCKET, SO_REUSEADDR, &onReUseAddr, sizeof(onReUseAddr));
    
    if (VERIFY(bind(*pSock, paddr, sizeof(struct sockaddr_in)) >= 0) && VERIFY(listen(*pSock, nMaxConnect) >= 0)) {/*套接字绑定并进入侦听状态*/
        return 0;/*成功，返回0*/
    }
    VERIFY(close(*pSock) == 0);/*失败，关闭套接字描述符*/
    return 1;
}

/*
 接收客户端的套接字连接申请，它在TCP通信服务器端成功调用函数createsock后使用，整型nsock指定用于侦听的套接字描述符，参数pSock回传新创建的与客户端相连接的套接字描述符，函数调用成功时返回0，或者返回其他值*/
int AcceptSock(int *pSock, int nSock)
{
    struct sockaddr_in addrin;
    socklen_t lSize;
    ASSERT(pSock != NULL && nSock > 0);
    while (1) {
        lSize = sizeof(struct sockaddr_in);
        memset(&addrin, 0, sizeof(struct sockaddr_in));
        /*阻塞接受客户端连接请求，并创建新的套接字描述符*/
        if ((*pSock = accept(nSock, (struct sockaddr *)&addrin, &lSize)) > 0) {
            return 0;
        }else if (errno == EINTR){/*调用accept过程中接收到信号，调用中断*/
            continue;
        }else{
            
            fprintf(stderr, "errno: %d  desc:%s\n", errno, strerror(errno));
            ASSERT(0);
        }
    }
}

/*
 客户端调用connect连接服务器端时，必然先创建套接字描述符，填充协议地址结构，并做好网络字节顺序转换和IP地址格式转换等工作。参数psock回传了新创建的与服务器相连接的套接字描述符，字符串pAddr制定了服务器端的IP地址，整型nPort指定了服务器端套接字的端口号。函数调用成功时返回0，或者返回其他值。*/
int ConnectSock(int *pSock, int nPort, char * pszAddr)
{
    struct sockaddr_in addrin;
    int nSock;
    ASSERT(pSock != NULL && nPort > 0 && pszAddr != NULL);
    /*创建TCP套接字描述符*/
    ASSERT((nSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);
    /*协议地址组包*/
    addrin.sin_family = AF_INET;/*协议族*/
    addrin.sin_addr.s_addr = inet_addr(pszAddr);/*字符串ip地址转换为网络字节顺序*/
    addrin.sin_port = htons(nPort);/*端口号网络字节顺序*/
    if (VERIFY(connect(nSock, (struct sockaddr *)&addrin, sizeof(struct sockaddr_in)) >= 0)) {
        *pSock = nSock;
        return 0;
    }
    /*连接失败，关闭套接字描述符*/
    close(nSock);
    return 1;
}

/*获取与套接字描述符nSock相连接的客户端IP地址，并将该IP值转化为以点分割的字符串形式存储到指针pAddr指定的缓冲区中，函数调用成功时返回0，或者返回其他值*/
int LocateRemoteAddr(int nSock, char* pAddr)
{
    struct sockaddr_in addrin;
    socklen_t lSize;
    ASSERT(nSock > 0 && pAddr != NULL);
    memset(&addrin, 0, sizeof(struct sockaddr_in));
    lSize = sizeof(struct sockaddr_in);
    int ret;
    ASSERT((ret = getpeername(nSock, (struct sockaddr *)&addrin, &lSize)) >= 0);/*获取对方套接字协议地址信息*/
    strcpy(pAddr, (char *)inet_ntoa(addrin.sin_addr));/*转换套接字地址信息为以点分割的字符串形式*/
    return 0;
}

int LocateNativeAddr(int nSock, char* pAddr)
{
    struct sockaddr_in addrin;
    socklen_t lSize;
    ASSERT(nSock > 0 && pAddr != NULL);
    memset(&addrin, 0, sizeof(struct sockaddr_in));
    lSize = sizeof(struct sockaddr_in);
    int ret;
    ASSERT((ret =getsockname(nSock, (struct sockaddr *)&addrin, &lSize)) >= 0);/*获取本地套接字协议地址信息*/
    strcpy(pAddr, (char *)inet_ntoa(addrin.sin_addr));/*转换套接字地址信息为以点分割的字符串形式*/
    return 0;
}



