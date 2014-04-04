//
//  main.c
//  IOFileOperation
//
//  Created by zhangliang on 14-4-3.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

/*临时文件使用模板  只使用一次的文件可以先删除后使用*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define AFILE "/Users/zhangliang/Desktop/tmpfile"

int main()
{
	int fno;
	if ((fno = open(AFILE, O_RDWR|O_CREAT| O_EXCL, 0755)) < 0)
	{
		fprintf(stderr, "open file error!\n");
		return 1;
	}
	unlink(AFILE);
	
	sleep(5);
	printf("end.\n");
    return 0;
}
