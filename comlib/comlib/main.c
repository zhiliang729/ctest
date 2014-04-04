//
//  main.c
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"
#include <stdio.h>

int main(int argc, char * argv[])
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
