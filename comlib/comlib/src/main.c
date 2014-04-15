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

int main(int argc, char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
    
    main3(argc, argv);
    return 0;
}
