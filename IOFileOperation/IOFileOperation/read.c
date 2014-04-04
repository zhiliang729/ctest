//
//  read.c
//  IOFileOperation
//
//  Created by zhangliang on 14-4-4.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
int main2()
{
    char buf[11];
    printf("%zd", read(STDIN_FILENO, buf, sizeof(buf)));
    return 0;
}