//
//  main.c
//  cEvent--putenv
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

/*
 putenv（改变或增加环境变量）
 　　相关函数 getenv，setenv，unsetenv
 　　表头文件 #include<stdlib.h>
 　　定义函数 int putenv(const char * string);
 　　函数说明：putenv()用来改变或增加环境变量的内容。参数string的格式为name＝value，如果该环境变量原先存在，则变量内容会依参数string改变，否则此参数内容会成为新的环境变量。
 　　返回值：执行成功则返回0，有错误发生则返回-1。
 　　错误代码：ENOMEM 内存不足，无法配置新的环境变量空间。
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
    char * p;
    if ((p = getenv("USER"))) {
        printf("USER = %s \n", p);
    }
    
    putenv("USER=test");
    printf("USER=%s\n", getenv("USER"));
    putenv("USER=zhangliang");
    printf("USER=%s\n", getenv("USER"));
    return 0;
}

