//
//  http3.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

char szHtml1[] = "<html>\n"
                    "<head>\n"
                        "<title>Hello, world!</title>\n"
                    "</head>\n"
                    "<body>\n"
                        "<b>This is a test server!</b><br>"
                        "<b>作者：张良</b><br>"
                        "<b>您的IP是：";
char szHtml2[] =        "\n</b>"
                    "</body>\n"
                "</html>\n";
/*
 并发同步短连接实例
 一个简单地web服务器例子，服务器进程创建端口号为9999的侦听套接字。当接收到网页浏览器的访问申请时，此服务进程fork一个子进程与网页浏览器连接，自己继续侦听其他网页浏览器的访问申请，子进程则组建一个固定的HTTP协议应答包，将一个特定的网页显示到浏览器上，最后子进程断开连接并结束运行
 */
int main15(int argc, char * argv[])
{
    int nSock, nSock1;
    pid_t nChild;
    char buf[2048], szAddr[30];
    memset(buf, 0, sizeof(buf));
    ASSERT(CreateSock(&nSock, 9999, 9) == 0);/*创建端口号为9999的侦听套接字*/
    while (1) {
        ASSERT(AcceptSock(&nSock1, nSock) == 0);/*创建连接套接字*/
        nChild = fork();
        PrintLog(stderr, "process id : %d", nChild);
        if (nChild == 0) {/*创建子进程*/
            break;
        }
        close(nSock1);/*父进程关闭连接套接字*/
    }
    /*子进程*/
    PrintLog(stderr, "Process id : %d", nChild);
    close(nSock);/*子进程关闭侦听套接字*/
    memset(szAddr, 0, sizeof(szAddr));
    LocateRemoteAddr(nSock1, szAddr);/*获取远程ip地址*/
    recv(nSock1, buf, sizeof(buf), 0);/*接收HTTP协议报文*/
    fprintf(stderr, "%s", buf);
    /*组建应答包*/
    sprintf(buf, "HTTP/1.1 200 OK\n%s%s%s", szHtml1, szAddr,szHtml2);
    send(nSock1, buf, strlen(buf), 0);/*发送HTTP应答报文*/
    
    fprintf(stderr, "%s", buf);
    close(nSock1);/*终端套接字连接*/
    return 0;
}

