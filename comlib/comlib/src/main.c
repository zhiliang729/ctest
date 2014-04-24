//
//  main.c
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

int main1(int argc, char * argv[])
{
    int i = 100000;
    PrintLog(stderr, "this is test[%d]", i);
    PrintTraceLog("this is test[%d]", i);
    PrintHexLog(stderr, &i, sizeof(i));
    PrintTraceHexLog(&i, sizeof(int));
    PrintTraceLog("----------------");
    Verify(0, NULL, __FILE__, __LINE__);
    PrintTraceLog("----------------");
    VERIFY(0);
    PrintTraceLog("-----------------");
    VERIFY(1);

    return 0;
}

int main2(int argc, char * argv[])
{
    char buf[11];
    ssize_t size = sizeof(buf);
    ReadFile(0, buf, &size);
    printf("%ld", size);
    return 0;
}

/*守护进程实例*/
int main3(int argc, char * argv[])
{
    InitServer();
    sleep(10);
    return 0;
}

/*打印今日、上日和下日日期*/
int main4(int argc, char * argv[])
{
    TIMESTRU now, yes, tom;
    now = GetTime();
    yes = GetOffsetTime(now, 0 - 24 * 60 * 60);
    tom = GetOffsetTime(now, 24 * 60 * 60);
    printf("now [%04d%02d%02d] yes [%04d%02d%02d], tom [%04d%02d%02d]\n",
           now.nYear, now.nMon, now.nDay,
           yes.nYear, yes.nMon, yes.nDay,
           tom.nYear, tom.nMon, tom.nDay);
    return 0;
}

/*定时读取键盘输入， 如果有键盘输入就立刻返回，否则10秒钟超时后就自动退出*/
void main5(int argc, char * argv[])
{
    char c;
    ssize_t size = 1;
    int i;
    i = ReadFileExt(0, &c, &size, 10);
    if (i == 0) {
        printf("%d\n", c);
    }else if(i == 1){
        printf("Timeout\n");
    }else{
        printf("Error\n");
    }
}

int main6(int argc, char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
    
//    main3(argc, argv);
//    main4(argc, argv);
    main5(argc, argv);
    return 0;
}
