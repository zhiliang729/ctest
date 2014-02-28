//
//  main.c
//  FileOperation
//
//  Created by zhangliang on 14-2-26.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <string.h>

//文件操作模式有 基本模式  1.r  2.w  3.a   增强模式4.r+ 5.w+ 6.a+ 10.r+a  文件内容模式 7.rb+ 8.wb+ 9.ab+  11.w+b 12.a+b
//r模式一般应用于读文件处理中，比如批量代发时读入第三方上送的批交易数据，导入第三方提供的报表文件数据等。w模式一般应用在单进程文件写入处理中，比如报表文件的创建与完成等。a模式一版应用与多个进程同时写入同一文件的处理中，比如记录日志文件等。

//FILE * fopen(const char *restrict filename, const char *restrict type);//成功打开文件时，函数返回指向该文件流的指针，否则返回null，因此在调用fopen后检查返回值是否为空是个好习惯。文件指针在打开后务必检查其是否可用
//FILE * freopen(const char *restrict filename, const char *restrict type, FILE *restrict stream);//本函数实现文件流的替换。它首先关闭原文件流stream，然后再以fopen的方式打开一个新的文件流，此后对原文件流的任意操作都自动转换为对新文件流的操作。成功时返回指向新文件流的指针，否则返回null。 unix进程默认打开三个文件：标准输出、标准输入和标准错误输出，它们的FILE标识符分别是stdout、stdin和stderr。函数freopen常用于将以上三个文件流重定向。
//int fclose(FILE * stream);//关闭文件流，成功时返回0，否则返回EOF。
//int remove(const char * filename);//删除字符串filename指定的文件或目录，当filename指定文件时，remove等同于函数unlink， 当filename指定目录时，remove等同于函数rmdir. 进程必须具备相应权限，才可以成功删除文件。方函数操作成功时返回0，否则返回其它值。
//int rename(const char * oldname, const char * newname);//函数rename更改原文件名oldname为新文件名newname，注意以下情况： 1.倘若oldname代表文件（非目录），仅当newname不错在，或者newname存在且并非目录时，函数rename执行更名操作。2.倘若oldname代表目录，仅当newname不存在，或者newname存在且为空目录（空目录指的是该目录中只有“.”和“..”项）时，函数rename才执行更名操作。同时，路径newname不能包含oldname为其路径前缀。例如。以下语句执行错误：rename("/tmp/tmp1/", "/tmp/tmp1/tmp"); 3.倘若oldname与newname指向同一文件，函数不做任何更改而成功返回。为了成功调用rename，调用进程必须具有包含文件olename和newname的目录的写和执行权限。rename调用成功时返回0，否则返回-1.
int main(int argc, const char * argv[])
{
    
    FILE * fp;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("open %s failed.\n", argv[1]);
        return 1;
    }
    
    
    fclose(fp);
    
    FILE * filep;
    char szBuf[100];
    
    /*将屏幕标准输出的内容重定向到文件“/tmp/1”*/
    if ((filep = freopen("/tmp/1", "w+a", stderr)) == NULL) {
        printf("stderr -- /tmp/1 failed.\n");
        return 1;
    }
    
    /*stderr 已经输出重定向，所有错误输出内容都将写入文件“tmp/1”*/
    fputs("I like unix. You are sb\n", stderr);
    
    /*关闭文件*/
    fclose(filep);
    
    /*将标准输入由键盘输入更改为从文件/tmp/1中读入*/
    if ((filep = freopen("/tmp/1", "r", stdin)) == NULL) {
        printf("stdin -- /tmp/1 failed.\n");
        return 1;
    }
    
    memset(szBuf, 0, sizeof(szBuf));
    
    /*stdin已经重定向，所有内容都将从/tmp/1中读取*/
    fgets(szBuf, sizeof(szBuf), filep);
    printf("szBuf = [%s]", szBuf);
    
    /*关闭文件*/
    fclose(filep);
    
    remove("/tmp/1");
    
    
    
    return 0;
}

