//
//  main.c
//  DirectoryFile
//
//  Created by zhangliang on 14-4-9.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/dirent.h>

#define ADIR "/Users/zhangliang/Desktop/dir"

/*
getcwd（取得当前的工作目录）
相关函数 get_current_dir_name，getwd，chdir
表头文件 #include<unistd.h>
定义函数 char * getcwd(char * buf,size_t size);
函数说明 getcwd()会将当前的工作目录绝对路径复制到参数buf所指的内存空间，参数size为buf的空间大小。在调用此函数时，buf所指的内存空间要足够大，若工作目录绝对路径的字符串长度超过参数size大小，则回值NULL，errno的值则为ERANGE。倘若参数buf为NULL，getcwd()会依参数size的大小自动配置内存(使用malloc())，如果参数size也为0，则getcwd()会依工作目录绝对路径的字符串程度来决定所配置的内存大小，进程可以在使用完此字符串后利用free()来释放此空间。
返回值 执行成功则将结果复制到参数buf所指的内存空间，或是返回自动配置的字符串指针。失败返回NULL，错误代码存于errno。*/

/*
 
 fchdir（改变当前的工作目录）
 相关函数 getcwd，chroot
 表头文件 #include<unistd.h>
 定义函数 int fchdir(int fd);
 函数说明 fchdir()用来将当前的工作目录改变成以参数fd 所指的文件描述
 词。
 返回值执 行成功则返回0，失败返回-1，errno为错误代码。
 附加说明
 */


/*目录定位使用模板*/
int main4(int argc, const char * argv[])
{
    long int l;
    DIR * dirp;
    
    if (mkdir(ADIR, 0777) != EOF) {
        
        dirp = opendir(ADIR);
        
        l = telldir(dirp);
        seekdir(dirp, l);
        
        closedir(dirp);
        rmdir(ADIR);
    }
    
    
    /*
     函数rewinddir重置目录流到开始位置，等价于语句
        seekdir(dirp, 0)；
     */
    return 0;
}

int main3(int argc, const char * argv[])
{
    DIR * pdir;
    struct dirent * pent;
    if (argc != 2) {
        return -1;
    }
    
    if ((pdir = opendir(argv[1])) == NULL) {
        fprintf(stderr, "open dir failed.\n");
        return -1;
    }
    
    while (1) {
        pent = readdir(pdir);
        if (pent == NULL) {
            break;
        }
        fprintf(stderr, "%llu %s\n", pent->d_ino, pent->d_name);
    }
    closedir(pdir);
    return 0;
}

int main2(int argc, const char * argv[])
{
    if (argc != 2) {
        return -1;
    }
    fprintf(stderr, "delete dir [%s] respcode = [%d]\n", argv[1], rmdir(argv[1]));
    return 0;
}

int main1(int argc, const char * argv[])
{
    if (argc != 2) {
        return -1;
    }
    if (mkdir(argv[1], 0777) == EOF) {
        printf("%s", strerror(errno));
        errno = 0;
        return -1;
    }
    fprintf(stderr, "create dir [%s] respcode = [%d]\n", argv[1], mkdir(argv[1], 0777));
    
    return 0;
}


int main(int argc, const char * argv[])
{
    char str[255];
    char * buf;
    
    
    buf = getcwd(str, sizeof(str));
    printf("%s\n", buf);
    buf = getwd(str);
    printf("%s\n", buf);
    
    fprintf(stderr, "pwd = [%s]\n", getcwd(str, sizeof(str)));
    chdir("../");
    fprintf(stderr, "pwd = [%s]\n", getcwd(str, sizeof(str)));
    
    main1(argc, argv);
    main2(argc, argv);
    
    main1(argc, argv);
    main3(argc, argv);
    main2(argc, argv);
    
    main4(argc, argv);
    return 0;
}

