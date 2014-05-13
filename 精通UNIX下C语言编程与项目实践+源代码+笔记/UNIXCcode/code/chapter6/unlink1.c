#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fno;
	if ((fno = open("./tmpfile", O_RDWR|O_CREAT| O_EXCL, 0755)) < 0) 
	{
		fprintf(stderr, "open file error!\n");
		return 1;
	}
	unlink("./tmpfile");

	sleep(3);
	printf("end.\n");
    return 0;
}

	
