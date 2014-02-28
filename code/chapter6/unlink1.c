#include <stdio.h>
#include <fcntl.h>
void main()
{
	int fno;
	if ((fno = open("./tmpfile", O_RDWR|O_CREAT| O_EXCL, 0755)) < 0) 
	{
		fprintf(stderr, "open file error!\n");
		return ;
	}
	unlink("./tmpfile");
	/* 以下部分对临时文件tmpfile操作 */
	sleep(60);
	printf("end.\n");
}

	
