//
//  main.c
//  stat
//
//  Created by zhangliang on 14-2-26.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int getFileType(mode_t st_mode, char * resp);
int getFileMode(mode_t st_mode, char * resp);
int getFileOtherAttr(struct stat info, char * resp);

int getFileType(mode_t st_mode, char * resp)/*根据st_mode判断文件类型， 并将结果写入字符串中，函数返回写入字符串resp的字符个数*/
{
    if (resp == NULL) {
        return 0;
    }
    
    if (S_ISDIR(st_mode)) {/*测试是否为目录文件*/
        resp[0] = 'd';
    }else if (S_ISCHR(st_mode)){/*测试是否为字符文件*/
        resp[0] = 'c';
    }else if (S_ISBLK(st_mode)){/*测试是否为块文件*/
        resp[0] = 'b';
    }else if (S_ISREG(st_mode)){/*测试是否为普通文件*/
        resp[0] = '-';
    }else if (S_ISFIFO(st_mode)){/*测试是否为管道文件*/
        resp[0] = 'p';
    }else if (S_ISLNK(st_mode)){/*测试是否为符号链接*/
        resp[0] = 'l';
    }else resp[0] = ' ';
    return 1;
}

int getFileMode(mode_t st_mode, char * resp)/*程序逐个判断文件的权限，并将结果写入字符串中，函数返回字符串resp的字符个数*/
{
    if (resp == NULL) {
        return 0;
    }
    
    memset(resp, '-', 9);
    
    if (st_mode & S_IRUSR) resp[0] = 'r';
    if (st_mode & S_IWUSR) resp[1] = 'w';
    if (st_mode & S_IXUSR) resp[2] = 'x';
    
    if (st_mode & S_IRGRP) resp[3] = 'r';
    if (st_mode & S_IWGRP) resp[4] = 'w';
    if (st_mode & S_IXGRP) resp[5] = 'x';
    
    if (st_mode & S_IROTH) resp[6] = 'r';
    if (st_mode & S_IWOTH) resp[7] = 'w';
    if (st_mode & S_IXOTH) resp[8] = 'x';
    
    return 9;
}

int getFileOtherAttr(struct stat info, char * resp)/*函数将文件的属性信息转化为字符串存入参数resp中， 并返回resp的字符数*/
{
    
    if (resp == NULL) {
        return 0;
    }
    
    struct tm * mtime = localtime(&info.st_mtime);
    
    struct tm * ctime = localtime(&info.st_ctime);
    
    struct tm * atime = localtime(&info.st_atime);
    
    return (sprintf(resp, " \nNumber of hard links:%3d\n User ID of the file:%6d\n Group ID of the file:%6d\n file size, in bytes:%11lld\n Last data modification time：%04d年%02d月%02d日 %02d时%02d分%02d秒\n Time of last status change：%04d年%02d月%02d日 %02d时%02d分%02d秒\n Time of last access：%04d年%02d月%02d日 %02d时%02d分%02d秒", info.st_nlink, info.st_uid, info.st_gid, info.st_size, mtime->tm_year+1900, mtime->tm_mon+1, mtime->tm_mday, mtime->tm_hour, mtime->tm_min, mtime->tm_sec, ctime->tm_year+1900, ctime->tm_mon+1, ctime->tm_mday, ctime->tm_hour, ctime->tm_min, ctime->tm_sec , atime->tm_year+1900, atime->tm_mon+1, atime->tm_mday, atime->tm_hour, atime->tm_min, atime->tm_sec));
    
}

int insertSpace(char * resp)
{
    resp[0] = '\n';
    return 1;
}


int main(int argc, const char * * argv)
{
    struct stat info;
    char buf[1000];
    char * p = buf;
    
    if (argc != 2) {
        fprintf(stderr, "stat filename\n");
        return 1;
    }
    
    for (int i = 0; i < argc; i++) {
        printf("%d : %s\n", i, argv[i]);
    }
    
    memset(p, 0, sizeof(buf));
    
    printf("\n\n");
    if (lstat(argv[1], &info) == 0) {/*int	lstat(const char *, struct stat *)*/
        p += getFileType(info.st_mode, p);//写入几位就移动几位，妙啊！读文件类型
        p += getFileMode(info.st_mode, p);//读文件权限
        
        p += insertSpace(p);
        
        p += getFileOtherAttr(info, p);
        
        p += insertSpace(p);
        
        printf("%s %s\n", buf, argv[1]);
    }else{
        fprintf(stderr, "open file failed!\n");
    }
    
    return 0;
}


