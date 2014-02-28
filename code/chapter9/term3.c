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
		fprintf(stderr, "打开终端[%s]出错!", devname); 
		return(-1);
	}
	if((ioctl(fd, TCGETA, &oldtty))==-1)
	{
		fprintf(stderr, "取终端[%s]参数出错!", devname); 
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
		fprintf(stderr, "设置终端[%s]参数出错!", devname);
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
		fprintf(stderr, "设置终端参数出错!");
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
	if ((fd = OpenTTY(ttyname(0))) == -1) return;	/* 打开终端设备 */
	write(fd,"\x1b[2h",4);		/* 锁定大键盘*/
	write(fd,"\x1b[>",3);		/* 选择辅串口1*/
	InputSe(fd);			/* 发布”输入密码“命令 */
	ReadFromSe(fd, buf, 6);		/* 读取密码键盘输入 */
	write(fd,"\x1b[<",3);		/* 关闭辅串口1*/
	write(fd,"\x1b[2l",4);		/* 解锁大键盘*/
	fprintf(stderr, "input [%s]\n", buf);	/* 显示输入数据 */
	CloseSe(fd);			/* 关闭密码键盘 */;
	CloseTTY(fd);			/* 关闭终端设备 */
}
