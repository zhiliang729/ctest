//
//  main.c
//  ls
//
//  Created by zhangliang on 13-12-16.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <dirent.h>
#include "apue.h"

int main(int argc, const char * argv[])
{
    DIR * dp;
    struct dirent * dirp;
    if (argc != 2) {
        err_quit("usage: ls directory_name");
    }
    
    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("can't open %s", argv[1]);
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp -> d_name);
    }
    
    closedir(dp);
    return 0;
}

