//
//  read.c
//  pipe
//
//  Created by zhangliang on 14-4-22.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#define FILEPATH "/Users/zhangliang/Desktop/myfifo"
//读有名管道
#include <fcntl.h>
int main100(int argc, const char * argv[])
{
    FILE * fp;
    char buf[255];
    while (1) {
        if ((fp = fopen(FILEPATH, "r")) == NULL) {
            return 0;
        }
        
        fgets(buf, sizeof(buf), fp);
        printf("gets:[%s]", buf);
        if (strncmp(buf, "exit", 4) == 0 || strncmp(buf, "quit", 4) == 0) {
            fclose(fp);
            break;
        }
        fclose(fp);
    }
    return 0;
}