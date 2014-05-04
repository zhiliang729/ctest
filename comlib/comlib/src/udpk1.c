//
//  udpk1.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
 UDP客户端数据发送程序，每隔一秒向服务器端发送数据报信息。
 */
int main16(int argc,char * argv[])
{
    int ret, i = 0;
    char szBuf[100];
    while (1) {
        sprintf(szBuf, "第%d发送", i);/*组包*/
        ret = SendMsgByUdp(szBuf, (int)strlen(szBuf), "127.0.0.1", 9999);/*发送*/
        if (ret == 0) {
            printf("Send UDP Success:%s\n", szBuf);
        }else {
            printf("Send UDP Failed:%s\n", szBuf);
        }
        sleep(1);
        i++;/*i代表发送的次数*/
    }
    return 0;
}
