#include <unistd.h>
#include <termio.h>
#include <fcntl.h>
void main()
{
	struct termio old, new;
	char buf[1024];
	ioctl (0, TCGETA, &old);
	new = old;
	new.c_iflag |= IUCLC; 
	ioctl (0, TCSETA, &new);
	scanf("%s", buf);
	ioctl (0, TCSETA, &old);	
}
