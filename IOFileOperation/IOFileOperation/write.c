//
//  write.c
//  IOFileOperation
//
//  Created by zhangliang on 14-4-4.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>

int main3()
{
    char buf[11] = "abcchina";
    printf("%zd", write(1, buf, sizeof(buf)));
    return 0;
}