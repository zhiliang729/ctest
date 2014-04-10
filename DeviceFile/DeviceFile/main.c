//
//  main.c
//  DeviceFile
//
//  Created by zhangliang on 14-4-9.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/*
    fd = open(devname, O_RDWR);//打开设备文件
    ioctl(fd, TCGETA, &oldtty);//备份设备文件设置
    ioctl(fd, TCSETA, &newtty);//设置备份文件设置
 ……………………………………………………………………
    read(fd, buf, len);//循环设备文件读写
    write(fd, buf, len);
 ……………………………………………………………………
    ioctl(fd, TCSETA, &oldtty);//恢复设备文件设置
    close(fd);//关闭设备文件
 
 养成先保存后恢复的习惯
 
 字符设备文件以字符为单位进行I/O操作，它的种类繁多。终端设备文件是标识用户与终端的典型字符设备文件
 */

int main(int argc, const char * argv[])
{
    return 0;
}

