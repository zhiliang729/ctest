//
//  lseek.c
//  IOFileOperation
//
//  Created by zhangliang on 14-4-4.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BFILE "/Users/zhangliang/Desktop/Hole.dat"

int main(int argc, const char * argv[])
{
    int foo;
    if ((foo = open(BFILE, O_WRONLY | O_CREAT, 0755)) < 0)
    {
        printf("open file hole.dat failed.\n");
        return 1;
    }
    
    lseek(foo, 1000000, SEEK_SET);
    write(foo, "333", 3);
    close(foo);
    
    return 0;
}