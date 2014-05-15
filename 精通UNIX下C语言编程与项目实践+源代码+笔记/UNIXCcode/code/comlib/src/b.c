#include <comlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/hd.h>
#include <sys/vtoc.h>
void main()
{
	int fd, iosec, i, ret;
	unchar iobuf[512];
	struct absio absio;
	if ((fd = open("/dev/rhd00", 00)) == -1)
		{
			printf("1\n");
			 return;
			}
	iosec = 1;
	absio.abs_sec = iosec;
	absio.abs_buf=iobuf;
	if ((ret = ioctl(fd, V_RDABS, &absio)) == -1)
		{
			printf("2\n");
			 return;
			}
	PrintHexLog(stderr, absio.abs_buf, 512);
}

