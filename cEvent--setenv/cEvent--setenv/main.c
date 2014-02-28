//
//  main.c
//  cEvent--setenv
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//


/*
 setenv（改变或增加环境变量）
 　　相关函数 getenv，putenv，unsetenv
 　　表头文件 #include<stdlib.h>
 　　定义函数 int setenv(const char *name,const char * value,int overwrite);
 　　函数说明 setenv()用来改变或增加环境变量的内容。
 
 　　参数 name为环境变量名称字符串。
 　　参数 value则为变量内容。
 
 　　参数 overwrite用来决定是否要改变已存在的环境变量。如果overwrite不为0，则改变环境变量原有内容，原有内容会被改为参数value所指的变量内容。如果overwrite为0，且该环境变量已有内容，则参数value会被忽略。
 
 
 　　返回值 执行成功则返回0，有错误发生时返回-1。
 　　错误代码 ENOMEM 内存不足，无法配置新的环境变量空间
 */
#include <stdio.h>
#include <stdlib.h>

#define UNSET 1

int main(int argc, const char * argv[])
{
    char *p;
    if ((p = getenv("USER"))) {
        printf("USER = %s\n", p);
    }
    
    int resu = setenv("USER", "test", 1);
    
#if UNSET
    unsetenv("USER");
#endif
    printf("USER=%s\n", getenv("USER"));
    return 0;
}

