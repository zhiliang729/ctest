#include <unistd.h>
#include <termio.h>
#include <fcntl.h>
#include <stdio.h>
struct termio oldtty, newtty;
int OpenTTY(char *devname)
{
	int fd;
	if((fd=open(devname,O_RDWR))==-1)
	{ 
		fprintf(stderr, "���ն�[%s]����!", devname); 
		return(-1);
	}
	if((ioctl(fd, TCGETA, &oldtty))==-1)
	{
		fprintf(stderr, "ȡ�ն�[%s]��������!", devname); 
		close(fd); 
		return(-1);
	}
	newtty=oldtty;
	newtty.c_iflag &= ~ICRNL;
	newtty.c_oflag = newtty.c_oflag & ~ONLCR & TAB0 | OPOST;
	newtty.c_lflag = newtty.c_lflag & ~ICANON & ~ECHO;
	newtty.c_cflag |= B9600;
	newtty.c_cc[VMIN]=1;
	newtty.c_cc[VQUIT]=0;
	if((ioctl(fd, TCSETA, &newtty)) == -1) 
	{ 
		fprintf(stderr, "�����ն�[%s]��������!", devname);
		close(fd); 
		return(-1);
	}
	return(fd);
}
void InputSe(int fd)
{
	char buf[3];
	buf[0] = 0x82;
	buf[1] = 0;
	write(fd, buf, 1);
}
void CloseSe(int fd)
{
	char buf[3];
	buf[0] = 0x83;
	buf[1] = 0;
	write(fd, buf, 1);
}
int CloseTTY(int fd)
{
	if((ioctl(fd, TCSETA, &oldtty))==-1 ) 
	{ 
		fprintf(stderr, "�����ն˲�������!");
		close(fd); 
		return(-1);
	}
	close(fd);
	return(0);
}
int ReadFromSe(int fd, char *buf, int len)
{
	int i = 0;
	buf[0] = 0;
	while (i < len) 
	{
		read(fd,&buf[i],1);
		buf[i] &= 0x7f;
		i++;
	}
	buf[i]=0;
	return(i);
}

void main()
{
	int fd;
	char buf[10];
	memset(buf, 0, sizeof(buf));
	if ((fd = OpenTTY("/dev/tty1a")) == -1) return;	/* �򿪴���1 */
	InputSe(fd);			/* �������������롰���� */
	ReadFromSe(fd, buf, 6);		/* ��ȡ����������� */
	fprintf(stderr, "input [%s]\n", buf);	/* ��ʾ�������� */
	CloseSe(fd);			/* �ر�������� */;
	CloseTTY(fd);			/* �رմ����豸 */
}
