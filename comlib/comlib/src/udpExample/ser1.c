//
//  ser1.c
//  comlib
//
//  Created by zhangliang on 14-4-30.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*监控主机端程序
    命令格式：
            ser1 主机信息   转发进程所在主机的ip  转发进程占用的udp端口号
  模拟本应该适时读取的系统信息，然后将此信息发送到“监控转发器段”
 */
int main_1(int argc, char * argv[])
{
    int i = 0;
    char szBuf[1024];
    if (argc != 4) {
        return -1;
    }
    
    while (1) {/*循环发送*/
        sprintf(szBuf, "%d:%s", i, argv[1]);/*组包*/
        SendMsgByUdp(szBuf, (int)strlen(szBuf), argv[2], atoi(argv[3]));
        fprintf(stderr, "Send[%s][%d][%s]\n", argv[2], atoi(argv[3]), szBuf);
        sleep(5);
    }
    return 0;
}

