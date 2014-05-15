#include <unistd.h>
#include <fcntl.h>
void main()
{
	int fno;
	if ((fno = open("hole.dat", O_WRONLY|O_CREAT,0755)) < 0)
	{
		printf("open file hole.dat failed.\n");
		return;
	}
	lseek(fno, 1000000000, SEEK_SET);
	write(fno, "eee", 3);
	close(fno);
}

