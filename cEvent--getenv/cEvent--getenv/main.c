//
//  main.c
//  cEvent--getenv
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

/*
 getenv()—-取得环境变量内容
 
 相关函数：putenv, setenv, unsetenv
 
 头文件：#include <stdlib.h>
 
 定义函数：char * getenv(const char *name);
 
 函数说明：getenv()用来取得参数name 环境变量的内容. 参数name 为环境变量的名称, 如果该变量存
        在则会返回指向该内容的指针. 环境变量的格式为name＝value.
 
 返回值：执行成功则返回指向该内容的指针, 找不到符合的环境变量名称则返回NULL.
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
    char * pUsr;
    if ((pUsr = getenv("USER"))) {
        printf("USER = %s\n", pUsr);
    }
    return 0;
}

