//
//  udp.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"
/*
 在UDP通信服务端调用，调用成功时，完成套接字的创建和命名等工作并返回0，否则关闭套接字并返回其他值
 */
int CreateUdpSock(int * pnSock, int nPort)
{
    struct sockaddr_in addrin;
    struct sockaddr * paddr = (struct sockaddr *)&addrin;
    ASSERT(pnSock != NULL && nPort > 0);
    memset(&addrin, 0, sizeof(struct sockaddr_in));
    /*协议地址组包*/
    addrin.sin_family = AF_INET;/*协议名*/
    addrin.sin_addr.s_addr = htonl(INADDR_ANY);/*自动分配地址*/
    addrin.sin_port = htons(nPort);/*端口号*/
    
    ASSERT((*pnSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);/*创建UDP套接字描述符*/
    if (VERIFY(bind(*pnSock, paddr, sizeof(struct sockaddr_in)) >= 0)) {
        return 0;/*准备成功，正确返回*/
    }
    VERIFY(close(*pnSock) == 0);/*准备失败，关闭套接字描述符*/
    return 1;
}

/*
 封装系统调用sendto的函数，向目的地址发送UDP数据包，其中指针pMsg指向待发送的数据，该数据长nSize，目的地址IP是szAddr，目的端口号是nPort，函数首先创建UDP套接字，然后组建接收方的UDP协议地址，再调用sendto完成信息发送，最后关闭创建的UDP关键字，当函数成功调用时返回0，否则返回其他值
 */
int SendMsgByUdp(void * pMsg, int nSize, char * szAddr, int nPort)
{
    int nSock;
    struct sockaddr_in addrin;
    ASSERT((nSock = socket(AF_INET, SOCK_DGRAM, 0)) > 0);/*创建UDP套接字描述符*/
    memset(&addrin, 0, sizeof(struct sockaddr_in));
    
    /*接收方的协议地址组包*/
    addrin.sin_family = AF_INET;/*协议名*/
    addrin.sin_addr.s_addr = inet_addr(szAddr);/*接收方的地址*/
    addrin.sin_port = htons(nPort);/*接收方的服务器端口号*/
    VERIFY(sendto(nSock, pMsg, nSize, 0, (struct sockaddr *)&addrin, sizeof(struct sockaddr_in)) > 0);/*发送数据到接收方*/
    close(nSock);/*关闭套接字*/
    return 0;
}

/*封装了系统调用recvform的函数，接收UDP数据包，其中nFile是服务器端UDP套接字描述符，指针pData指向接收数据的内存缓冲区，参数pnsize是该缓冲区的可容纳的最大容量。函数调用成功时返回0，并将接收到的信息保存到缓冲区pData中，参数pnSize回传收到数据的字节长度，否则返回其他值*/
int RecvMsgByUdp(int nFile, void * pData, ssize_t * pnSize)
{
    /*接收数据，并不关心发送方协议地址*/
    VERIFY((*pnSize = recvfrom(nFile, pData, *pnSize, 0, NULL, NULL)) > 0);
    return 0;
}



