//
//  main.c
//  ExternalDevice
//
//  Created by zhangliang on 14-4-10.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct termios oldtty, newtty;

/*
 外设与主机之间存在两种连接方式：
    （1）外设直接连接主机串口或并口
    （2）外设连接物理终端的串口或并口，物理终端通过主机串口、多串口卡或者Telnet路由器等方式与主机相连接。无论那种连接方式，外设最终都与主机设备文件相联系。
 */

/*1 操作连接串口的外设
    外设操作可分为打开终端、外设读写和关闭串口等三个步骤
 
    串口连接外设和主机，打开了串口终端设备，就开启了主机与外设之间的一道门，开启了它们之间的逻辑联系。
 */

/*打开终端，devname根据unix系统不同传入设备文件名*/
int OpenTTY(char * devname)
{
    int fd;
    if ((fd = open(devname, O_RDWR)) == -1) {
        fprintf(stderr, "打开终端【%s】出错！", devname);
        return -1;
    }
    
    
    /*
     注意，设置终端参数时，切记备份原有终端设置
     */
    /*获取终端设置*/
    if (tcgetattr(fd, &oldtty) == -1) {
        fprintf(stderr, "取终端【%s】参数出错！", devname);
        close(fd);
        return -1;
    }
    
    //重新设置参数
    
    newtty = oldtty;
    newtty.c_iflag &= ~ICRNL;/*输入不进行CR和NL转换*/
    newtty.c_oflag = newtty.c_oflag & ~ONLCR &( TAB0 | OPOST);/*输出不进行NL和CR的转换， 字符处理后输出， 水平制表符输出延时为0，即字符处理后不延时输出*/
    newtty.c_lflag = newtty.c_lflag & ~ICANON & ~ECHO;/*原始模式输入（即键入即输出，不等回车），不回显字符*/
    newtty.c_cflag |= B9600;/*波特率为9600*/
    
    /*每次只接受一个字符（直到输入一个字符后返回）*/
    newtty.c_cc[VMIN] = 1;
    newtty.c_cc[VQUIT] = 0;
    
    if (tcsetattr(fd, TCSANOW, &newtty) == -1) {
        fprintf(stderr, "设置终端[%s] 参数出错！", devname);
        close(fd);
        return -1;
    }
    return fd;
}

/*
 外设读写
 直接向打开的串口设备文件中执行read或write操作
 */

/*向文件fd中写入数据82H*/
void InputSe(int fd)
{
    char buf[3];//指令
    buf[0] = 0x82;//82H 输入密码提示
    buf[1] = 0;
    write(fd, buf, 1);
}

/*读取密码键盘输入（长度为6字符）
 理论上应该加上输入有效性判断和超时处理：只读取密码键盘中输入的数字和回车符，忽略其他错误性输入；长时间未接收密码键盘输入时，函数应该自动退出。
 */
int ReadFromSe(int fd, char * buf, int len)
{
    int i = 0;
    buf[0] = 0;
    while (i < len) {
        read(fd, &buf[i], 1);
        buf[i] &= 0x7f;/*字符转整型与操作*/
        i++;
    }
    buf[i] = 0;
    return i;
}

/*密码键盘执行“关闭密码键盘”命令*/
void CloseSe(int fd)
{
    char buf[3];
    buf[0] = 0x83;
    buf[1] = 0;
    write(fd, buf, 1);
}

/*
    关闭终端
    恢复终端参数设置后关闭文件描述符
 */
int CloseTTY(int fd)
{
    if (tcsetattr(fd, TCSANOW, &oldtty) == -1) {
        fprintf(stderr, "设置职工段参数出错！");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int main(int argc, const char * argv[])
{
    int fd;
    char buf[10];
    memset(buf, 0, sizeof(buf));
    if ((fd = OpenTTY("/dev/ttys000")) == -1) {
        return -1;
    }
    
    InputSe(fd);
    ReadFromSe(fd, buf, 6);
    fprintf(stderr, "input:[%s]", buf);
    CloseSe(fd);
    CloseTTY(fd);
    return 0;
}



/*
            操作连接终端的外设
    当外设与物理终端相连接时，主机与外设之间的通信必须通过终端转发。因此在终端设备打开后，外设数据读写前，主机必须向物理终端发送命令以建立与外设的通道。
 */

