//
//  configINIParser1.c
//  comlib
//
//  Created by zhangliang on 14-5-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
例子，读取代码INI文件中记载的配置信息，并打印远程主机的IP地址和端口号配置
 */

#define INICONFIGFILEPATH "/Users/zhangliang/Desktop/Project/C/comlib/comlib/src/configINIParse/config.ini"

int main_conf(int argc, char * argv[])
{
    int nNumber, i, nPort;
    char szRoot[100], szIp[100], szProtocol[100];
    ASSERT(GetINIConfigValue(INICONFIGFILEPATH, "SERVER", "NUMBER", &nNumber, STRINT) == 0);
    PrintLog(stderr, "no = %d", nNumber);
    for (i = 0; i < nNumber; i++) {
        sprintf(szRoot, "SERVER%02d", i + 1);
        memset(szIp, 0, sizeof(szIp));
        memset(szProtocol, 0, sizeof(szProtocol));
        GetINIConfigValue(INICONFIGFILEPATH, szRoot, "IP", szIp, STRSTR);
        GetINIConfigValue(INICONFIGFILEPATH, szRoot, "PORT", &nPort, STRINT);
        GetINIConfigValue(INICONFIGFILEPATH, szRoot, "PROCOTOL", szProtocol, STRSTR);
        PrintLog(stderr, "IP=%s PORT=%d PROTOCOL=%s", szIp, nPort, szProtocol);
    }
    return 0;
}


