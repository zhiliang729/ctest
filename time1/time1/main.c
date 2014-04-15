//
//  main.c
//  time1
//
//  Created by zhangliang on 14-4-15.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main1(int argc, const char * argv[])
{
    /*系统时间*/
    time_t start, finish;
    time(&start);
    sleep(6);
    time(&finish);
    printf("start = [%ld], finish = [%ld], difftime = [%f]\n", start, finish, difftime(finish, start));
    return 0;
}

/*打印本地时间的例子，它先读取系统时间，再转换为本地时间格式输出*/
int main2(int argc, const char * argv[])
{
    /*
     本地时间
     unix中更改系统时间为本地日历时间函数：
     #include <time.h>
     struct tm * localtime(const time_t * clock);//转换系统时间按clock为当地时间，并以tm结构返回。结构tm以人们熟悉的年月日等形式记载时间，定义在头文件time.h中
     time_t mktime(struct tm * timeptr);//实现localtime的反功能，它返回tm结构中对应的系统时间。主要应用在计算上日或次日日期中
     */
    
    struct tm when;
    time_t now;
    time(&now);
    when = * localtime(&now);
    printf("now = [%ld][%04d%02d%02d %02d:%02d:%02d]\n", now,
           when.tm_year + 1900,
           when.tm_mon + 1,
           when.tm_mday,
           when.tm_hour,
           when.tm_min,
           when.tm_sec);

    return 0;
}

int main(int argc, const char * argv[])
{
    main1(argc, argv);
    main2(argc, argv);
    return 0;
}

