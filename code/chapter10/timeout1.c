#include <unistd.h>
#include <termio.h>
#include <fcntl.h>
void main()
{
	struct termio old, new;
	char c = 0;
	ioctl (0, TCGETA, &old);
	new = old;
	new.c_lflag &= ~ICANON; 
	new.c_cc[VMIN] = 0;
	new.c_cc[VTIME] = 30;
	ioctl(0, TCSETA, &new);	
	if ((read(0, &c , 1)) != 1) 
		printf("timeout\n");
	else printf("\n%d\n", c);
	ioctl (0, TCSETA, &old);	
}
