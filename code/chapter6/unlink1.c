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
	/* ���²��ֶ���ʱ�ļ�tmpfile���� */
	sleep(60);
	printf("end.\n");
}

	
