//
//  main.c
//  fileOperation--formatReadWrite
//
//  Created by zhangliang on 14-2-27.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <strings.h>

//标准编程函数库对文件流的输入输出操作非常灵活, 我们既可以采用所见即所得的方式, 以无格式方式读写文件, 又可以对输入输出数据进行转化, 以有格式方式读写文件.
/*
文件的无格式读写

无格式读写分三类: 按字符读写, 按行读写和按块读写.
字符读写函数族:
代码:
#include <stdio.h>
int getc(FILE *stream);
int getchar(void);
int fgetc(FILE *stream);
 
int putc(int c, FILE *stream);
 int putchar(int c);
int fputc(int c, FILE *stream);
 函数getc以unsigned char类型读取文件输入流stream中的一个字符，并将该无符号字符转化为整数返回，同时移动文件指针到下一个字符处。函数getchar实际上是关于getc的一个宏定义"getc(stdin)".
 函数fgetc的功能类似于getc，不同的是，它的执行速度远低于getc，因此getc常常被定义在宏中使用。
 当文件结束或错误时，这三个函数都将返回EOF，EOF为常数。特别注意，他们的返回值都是整形。 EOF一般都是定义为int型-1,在某些UNIX中，将EOF强行转换为字符型后的数值不再与原值相等，从而程序不能正确执行。
 字符输入函数族虽然每次读取一个字符，但是其返回值为整型，故在返回值变量中务必定义为int而不是char 或 unsigned char， 否则会读取到错误的信息。
 因此错误的文件结束判断代码如下：
 char c;//---char c是错误的---
.....
c=getc(FILENAME);
if(c==EOF)
....
而正确的文件结束判断代码是：
int c;//---char c是错误的---
.....
c=getc(FILENAME);
if(c==EOF)
....
 
 函数putc首先先将int型参数c自动转换为unsigned char类型，然后写入文件流stream中，同时移动文件指针到下一个字符处。函数putchar 实际上是关于putc的宏定义"putc(stdout)"。函数fputc在功能上与putc相仿，但是执行速度却远低于putc。调用成功时，这三个函数返回其写入的数值，即参数c的值，否则返回EOF。

 标准函数编程库提供了行读写函数，该类函数读取一行以换行符"/n"结束的数据，写入数据时自动输出换行符。
行读写函数族:
代码:
#include <stdio.h>
char *gets(char *s);
char *fgets(char *s, int n, FILE *stream);
 函数gets从标准输入流(stdin)中读取一串字符存储到参数s所指向的内存空间中，文件结束或者错误发生时返回NULL，否则将返回参数s所指向的内存地址。
 函数fgets中键入了防溢出控制，它从文件流stream中读取一串字符到参数s所指向的内存空间，但读取数据的长度(包括换行符"/n")不能超过 n-1,。参数n代表了字符串s的最大存储空间。倘若待读入的实际数据长度包括("/n")超过了n-1，函数将截取该n个字符返回，剩余的字符将在下一 次fgets调用时读入。
 两个函数都把读取的字符信息存入字符串s中，并且自动增加字符串结束符"0",这也是fgets一次性最多只能读入n-1个字符的原因(第n个字符需要存储结束符"0")。函数调用成功时返回参数s的值，即指向输入的字符信息，否则返回空指针NULL。
 
 
 函数fgets，gets调用成功时返回值就是指针参数s的值，这样的函数设计方彪了后续处理过程。比如完成了“从文件stream中读取一串字符，再显示出来”两种方的调用为：
 //不利用fgets函数的返回值
 char s[1024];
 ...
 fgets(s,sizeof(s),STREAM);//fgets调用模板
 puts(s);
 .....
 
 
 //利用fgets函数的返回值
 char s[1024];
 ...
 puts(fgets(s,sizof(s),STREAM);
 ....
 出于程序的可移植性考虑，程序中数据类型长度，字符数组长度和结构体长度等数据一般都不直接指定，而是采用sizeof的形式由编译程序决定。
 
int puts(const char *s);
int fputs(const char *s, FILE *stream);
 参数s指向一串以字符串结束符"0"结尾的字符；函数puts把该字符串(不包括结束符"0")写入到标准输出流stdout中，并自动输出换行符"\n"；函数fputs字符串s(不包括结束符"0")写入文件流stream中，但不再输出换行符"\n"。
 两函数都不输出字符串末的结束符，输出失败时，都返EOF。
 
 
 
块读写函数族:
 块读写函数，能够输入输出任何数量的字符，在操作二进制文件时常常使用，标准文件编程库中用于文件块输入输出的函数
代码:
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);
 函数fread从文件流stream中读入nitems个数据项存储到指针ptr所指向的内存中，每个数据项具有size字节大小，一次操作总共读入size*nitems个字节。
 函数fwrite将ptr的数据写入到stream中，每次可写入size*nitems个字符。参数nitems表示写入文件的数据项个数，参数size表示每个数据项具有的字节大小。
 注：在大多数Unix中，size_t被定义为无符号整形：
 typedef unsigned int size_t;
 fread&fwrite都不返回实际读写的字符个数，而返回的是实际读写的数据项数。成功时，返回值等于参数nitems值，否则返回值将小于nitems值。
 块I/O函数常应用于二进制文件读写中，操作比较灵活，既可以读写一个字符，也可以读写一个结构，还可以读写任意数据类型。
 
 //读写字符
char c;
....
fread(&c,sizeof(char),1,INSTREAM);
...
fwrite(&c,sizeof(char),1,OUTSTREAM);


//读写结构
typedef struct
{
    char c;
    int l;
}MYSTRUCT;
...
MYSTRUCT my;
...
fread(&my,sizeof(MYSTRUCT),1,INSTREAM);
...
fwrite(&my,sizeof(MYSTRUCT),1,OUTSTREAM);

//读写数组
double f[5];
...
fread(f,sizeof(double),5,INSTREAM);
...
fwrite(f,sizeof(double),5,OUTSTREAM);*/


/*
 (3)实例
 报文解析是Unix应用的一个重要内容。双方把约定的几个域通过某种排序和分割方式组合在一起，就成了报文。报文解析就是从报文中分解出各个域的数据，比如从银行的代收代付报文查找出账号域和资金域的内容。
 字符串报文是报文的重要分支，它以字符串为载体记录了域的数据。在字符串报文中，域与域之间最常见的2中分隔方式是固定长度分割和特殊字符(串)分割。前 者每个域占用固定宽度，解析时只是读取特定位置的数据即可。后者的域与域之间由固定的字符(串)连接，解析时需要计算固定字符(串)出现的次数，以决定域 的序号和内容。Unix中passwd文件就是由":"分割的字符串报文组合而成的。
 例子：一个使用字符读写函数解析报文的，程序读取文件的"/etc/passwd"中每一个字符，并将"用户名称"域(报文的第一个域)单独提出，存入文件"copyname.txt"中.
 */

int main1(int argc, const char * argv[])
{
    FILE * fpr, *fpw;
    
    int c = 0, f = 0;//c用来存储读取的字符，f作为标识符，用来判断当前域信息。dangf值为0时表明正在读取报文的第一个域，所有的字符均为“有效”数据。当读取到分隔符“:”时，意味着已经超出了“用户名称”域的范围，所有数据均需抛弃，直到读取到换行符“\n”，进入到下一个报文位置。
    
    /*以下打开源文件*/
    if ((fpr = fopen("/etc/passwd", "r")) == NULL) {
        printf("open file /etc/passwd failed.\n");
        return 1;
    }
    
    /*以下打开目标文件*/
    if ((fpw = fopen("/Users/zhangliang/Desktop/Copyname.txt", "w")) == NULL) {
        printf("open file /Users/zhangliang/Desktop/Copyname.txt failed.\n");
        fclose(fpr);//如果程序需要同时打开两个或两个以上文件，当后一个文件的打开操作发生错误，而不得不退出函数时，务必关闭前面已经打开的文件。
        return 1;
    }
    
    
    while ((c = getc(fpr)) != EOF) {
        /*字符已经读取到c中*/
        if (f == 0) {
            
            if (c != ':') {//读取文件直到遇到“:”
//                putchar(putc(c, fpw));/*先将c输出到fpw，再输出到stdout*/
                putc(c, fpw);
            }else{//遇到“:”则将标识置为1
                f = 1;
            }
        }else if(c == '\n'){//若标识为1，且c为换行符则，重新置为0，并输出换行符，否则，跳过（即越过第一个":"以后的该行内容）
            f = 0;
//            putchar(putc(c, fpw));
            putc(c, fpw);
        }
    }
    
    /*关闭文件*/
    fclose(fpr);
    fclose(fpw);
    return 0;
}


//解析报文文件的另一种方法是先读入一行数据，再通过字符串函数分解。本处设计了一个使用行读写解析报文的例子，程序按行读取文件"/etc /passwd"，并将"用户名称"域提取出来(报文的第一个域)单独提取出来，存入件"copyname1.txt"中
int main2(int argc, const char * argv[])
{
    FILE * fpr, * fpw;
    char buf[1024], * p1, * p2;
    
    /*以下是打开源文件*/
    if ((fpr = fopen("/etc/passwd", "r")) == NULL) {
        printf("open file /etc/passwd failed.\n");
        return 1;
    }
    
    /*以下是打开目标文件*/
    if ((fpw = fopen("/Users/zhangliang/Desktop/Copyname1.txt", "w")) == NULL) {
        printf("open /Users/zhangliang/Desktop/Copyname1.txt failed.\n");
        fclose(fpr);
        return 1;
    }
    
    memset(buf, 0, sizeof(buf));
    while (fgets(buf, sizeof(buf), fpr) != NULL) {//读入一行数据，经过转换后分别再文件和屏幕上输出，由于函数fputs并不自动补‘\n’，所以文件输出过程中单独输出了换行符。
        /*p1 指向第一个“：”， p2 指向第2个“：”*/
        if ((p1 = strstr(buf, ":")) == NULL) {
            continue;
        }
        
        
        /*
         #include <string.h>
         
         char * strstr(char * s1, char * s2);
         
         strstr函数查找字符串s2在字符串s1中第一次出现的位置。如果s2是空指针，则返回指针s1的值，如果字符串s2没有在s1中出现，则返回空指针，否则，返回值指向s2在s1中出现的首地址*/
        if ((p2 = strstr(p1 + 1, ":")) == NULL) {
            continue;
        }
        
        p1++;
        p2++;
        /*p1指向第二个域密码字段，p2指向第三个域用户ID字段*/
        
        /*以下代码移动字符串内容，将ID字段的内容移动到用户名字段后*/
        while ( *p2 != ':') {
            *p1 = *p2;
            p1++; p2++;
        }
        
        *p1 = 0;//插入字符结束符
        
        /*屏幕输出*/
        puts(buf);
        
        /*文件输出*/
        fputs(buf, fpw);
        fputs("\n", fpw);
        
        /*清除内存*/
        memset(buf, 0, sizeof(buf));//自建的缓冲区，请务必先清空后使用
    }
    fclose(fpr);
    fclose(fpw);
    
    return 0;
}

/*
块读写函数经常操作二进制文件，保留内存信息或永久存储数据信息等。比如在软件中涉及一个文件型数据库，用以存取历史交易明细。
本处设计了一个存取数据信息的实例，通过文件"array.dat"存储和读取一个整形数组，其中文件起始记录数组的长度，随后是数组的数据信息。*/
/*数据信息存储*/
int main3(int argc, const char * argv[])
{
    FILE * fp;
    
    /*数组数据*/
    int narray[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2};
    
    /*写入数组元素的个数，请不要操作数组长度*/
    int all = 6;
    
    if ((fp = fopen("/Users/zhangliang/Desktop/array.dat", "wb")) == NULL) {
        printf("open file array failed.\n");
        return 1;
    }
    
    /*实际写入1*sizeof（int）个字符*/
    fwrite(&all, sizeof(int), 1, fp);
    
    /*实际写入all*sizeof（int）个字符*/
    fwrite(narray, sizeof(int), all, fp);
    
    fclose(fp);
    return 0;
}

/*数据信息恢复*/

int main4(int argc, const char * argv[])
{
    FILE * fp;
    int narray[10];
    int all, i;
    
    if ((fp = fopen("/Users/zhangliang/Desktop/array.dat", "rb")) == NULL) {
        printf("open file array failed.\n");
        return 1;
    }
    
    /*读取文件中存储的第一个整数，获取存储数组的元素数*/
    fread(&all, sizeof(int), 1, fp);
    
    /*读取文件中存储的数组元素*/
    fread(narray, sizeof(int), all, fp);
    printf("all = %d\n", all);
    
    for (i = 0; i < all; i++) {
        printf("[%d] = %d   ", i, narray[i]);
    }
    
    printf("\n");
    
    fclose(fp);
    return 0;
}



/*
2)文件的格式化读写
文件的格式化读写具有数据格式自动转换和文件流输入输出两个过程。输入时，先将文件流中的字符串转化为二进制数据，再存入内存中；输出时，先将二进制数据转化为字符串数据，再输出。
 数据格式化读写时能够自动转换的数据格式有：数据类型、精度、宽度、进制和标志等。
 文件格式化输出函数族：
 int printf(const char *format, [arg,] ...)
int fprintf(FILE *stream, const char *format, [arg,]...)
int sprintf(char *s, const char *format, [arg,]...)
sprintf输出结果到字符串s中，同时在字符串末尾自动加上字符结束符'\0'。这三个函数调用成功时返回实际输出的字符数，否则返回一个负数
 
 
 c语言中转移字符串的一般形式为
 %【标志】【宽度】【.精度】类型
 
 【】内为可选项
 
 常见类型：
 d,i 以十进制形式输出带符号整数
 u 以十进制形式输出无符号整数
 f 以小数形式输出单、双精度实数
 e,E 以指数形式输出单、双精度实数
 g,G 以%f%e中较短的输出宽度输出单、双精度实数
 x,X 以十六进制形式输出无符号整数,前者输出"0123456789abcdef"，后者输出"0123456789ABCDEF"
 o 以八进制形式输出无符号整数
 c 输出单个字符
 s 输出字符串
 p 以指针形式输出
 标志：
 - 左对齐输出，右边填空格
 + 带符号(正号或负号)输出
 空格 输出正数时加上空格，输出负数时加上负号
 # 整型的八进制或十六进制转换时分别输出前缀"0"或"0x"；浮点数转换时，省去多余小数
 
 
 
 
 
 文件格式化输入函数族：
int scanf(const char *format, [pointer,]...)
int fscanf(FILE *stream, const char *format, [pointer,]...)
int sscanf(const char *s, const char *format, [pointer,]...)
输入函数能自动过滤输入流中的空格、制表等符号。函数调用成功时返回读入值的参数个数，否则返回EOF。
 
 输入函数使用格式控制字符串format转换输入字符串为指定的数据类型，与输出函数相比，在形式上有两大不同点：
 1.输入函数的变长参数必须全部为指针
 2.输入函数能自动过滤输入流中的空格、制表符等符号。
 
 */





/*
 3):函数的变长参数定义及在本函数内使用
 文件的格式化参数都支持变长参数。定义时，变长参数列表通过省略号"..."表示，因此，具有变长参数列表的函数定义格式如下：
 type 函数名(参数1,参数2,参数n,...);
 其中type为函数的返回值类型，参数1~n为定长参数，...代表变长参数，...必须定义在参数的最右端。如下例：
 int printf(const char * format,...);
 int mysum(...);
 1)变长参数的使用
 Unix的变长参数通过va_list对象实现，定义在文件"stdarg.h"中，变长参数的应用模板代码如下：
 #include<stdarg.h>
 function(parmN,...)
 va_list pvar;
 .........
 va_start(pvar,parmN);
 while()
 {
 ........
 f=va_arg(pvar,type);
 ........
 }
 va_end(pvar);
 
 
 1 step. va_list pvar
 申明va_list数据类型变量pvar，该变量访问变长参数列表中的参数
 2 step. va_start(pvar,parmN)
 宏va_start初始化变长参数列表。pvar是va_list型变量，在step1定义，记载列表中的参数信息。parmN是省略号"..."前的一个参数名，va_start根据此参数，判断参数列表的起始位置。
 3 step: va_arg(pvar,type)
 获取变长参数列表中参数的值。pvar是step定义的va_list型变量，type为参数值的类型，也是红va_arg返回数值的类型，如：
 va_arg(pvar,int);
 va_arg(pvar,float);
 宏va_arg执行完毕后自动更改对象pvar，将其指向下一个参数。
 4 step：va_end(pvar)
 关闭本次对变长参数列表的访问。
 */


/*
 设计函数mysum，计算输入参数的和并返回结果
 */

#include <stdarg.h>
int mysum(int i, ...)/*参数i表明变长参数的个数*/
{
    int r = 0, j = 0;
    va_list pvar;
    va_start(pvar, i);
    
    for (j = 0; j < i; j++) {
        r += va_arg(pvar, int);
    }
    va_end(pvar);
    return r;
}

int main5(int argc, const char * argv[])
{
    printf("sum(1,4) = %d \n", mysum(1,4));
    printf("sum(3, 2, 4, 8) = %d \n", mysum(3, 2, 4, 8));
    return 0;
}


/*
 4)变长参数的传递，将变长参数作为参数列表传递给其它函数。
 变长参数传递的函数族如下：
 #include<stdarg.h>
 int vprintf(const char *format,va_list ap);
 int vfprintf(FILE *stream,const char *format,va_list ap);
 int vsprintf(char *str,const char *format,va_list ap);
 这些函数完全等价于格式化函数，只是在形式上采用固定参数替代变长参数列表，这样描述的函数更加紧凑，这些函数常应用于变长参数内部的功能实现。
 
 实现：设计函数"int PrintLog(FILE *stream,const char *pformat,...)"，它按照字符串format的内容，控制后继参数的数量和格式，并在文件流stream中输出。
 */

int printLog(FILE * pfile, const char * pformat, ...)
{
    va_list pvar;
    char szbuf[1024];
    if (pformat == NULL || pfile == NULL) {//对于指针类型的参数，最好在参数入口处判断其是否为空，以免空指针引用错误
        return -1;
    }
    va_start(pvar, pformat);//初始化变长参数列表
    vsprintf(szbuf, pformat, pvar);//传递变长参数
    va_end(pvar);//结束使用变长参数列表
    
    fputs(szbuf, pfile);//输出到文件流
    return 0;
}

int main6(int argc, const char * argv[])
{
    int a = 10;
    int * p = &a;
    printLog(stderr, "[%s] [%s] [%d] [%c]\n", "This", "is", 5, 's');
    printLog(stdout, "Error [%p] [%.2f] [%x]\n", p, 3.123, 100);
    return 0;
}


/*
 4：文件读写位置的定位
 在实际应用中，我们常常只需要读取文件中的一小段内容，或者写入一小段内容到文件中，使用文件的读写定位功能可以避免读写不必要的的数据段。例如某文件由一系列固定大小记录块组成，当访问其第n块记录时，可以先将文件指针移动到第n块记录的起始位置，再访问1个记录块大小的数据，并不需要从文件头开始读取n个记录块。
 标准文件变成库中用于定位文件读写为止的函数如下：
 #include<stdio.h>
 int fseek(FILE *stream,long int offset,int whence);
 void rewind(FILE *stream);
 long int ftell(FILE *stream);
 其中fseek改变文件流stream中的访问位置，参数whence表示文件定位的方式，fseek中提供了三种定位方式，参数offset表明了定位的偏移量，其值可正可负，与whence一起确定访问文件的最终定位。若调用成功则返回0，否则返回非0值
 whence     含义              文件定位于
 SEEK_SET   从文件头开始定位    0+offset
 SEEK_CUR   从当前位置开始定位   当前位置+offset
 SEEK_END   从文件尾开始定位    文件末+offset
 
 rewind重置流stream，将文件流定位于文件开始处，相当于执行了以下操作：
 void fseek(stream,0L,SEEK_SET);
 
 函数ftell获取文件流的当前位置，调用成功时将该值返回，否则返回-1。
 
 注：
 大多数系统中，可以通过以下代码获取文件的长度(len)
 long len;
 fseek(stream,0,SEEK_END);
 len=ftell(stream);
 */

/*
 本处设计了一个文件读写定位的例子，代码如下，其中seekwrite将整数值narray写入文件第"rec*sizeof(int)"处；函数seekread从文件的第"rec*sizeof(int)"位置中读取整数值存入narray.
 如果定位超过了文件的最大长度且执行了写入操作，文件长度将延长到当前位置，中间部分自动补0；
 */

void seekWrite(FILE * fp, int *narray, int sec)
{
    fseek(fp, sec * sizeof(int), SEEK_SET);
    fwrite(narray, sizeof(int), 1, fp);
}

void seekRead(FILE * fp, int * narray, int sec)
{
    fseek(fp, sec * sizeof(int), SEEK_SET);
    fread(narray, sizeof(int), 1, fp);
}

int main7(int argc, const char * argv[])
{
    FILE * fp;
    int narray[2] = {100, 200}, narrayr[2];
    if ((fp = fopen("/Users/zhangliang/Desktop/see.dat", "w+b")) == NULL) {
        printf("open file seek.dat failed.\n");
        return -1;
    }
    
    seekWrite(fp, &narray[0], 1);
    seekWrite(fp, &narray[1], 10);
//    seekRead(fp, &narrayr[0], 1);
    seekRead(fp, narrayr, 1);
//    seekRead(fp, &narrayr[1], 10);
    seekRead(fp, narrayr+1, 10);
    
    printf("now array[0] = %d, array[1] = %d\n", narrayr[0], narrayr[1]);
    fclose(fp);
    return 0;
}

/*
 5:文件的状态
 每一个流对象内部都保持了两个指示状态：错误指示状态和文件结束状态，函数ferror和feof分别检查这两个状态，函数strerror显示错误的提示信息。
 (1)文件的错误与结束状态
 #include<stdio.h>
 int ferror(FILE *stream);
 int feof(FILE *stream);
 void clearerr(FILE *stream);
 当文件I/O发生错误时，调用ferror函数将返回非0值，否则返回0值。当文件结束时，调用feof函数返回非0值，否则返回0值，函数clearerr清除文件错误标志和EOF标志。
 */

/*实例：
 读取文件"/etc/passwd"，当文件结束时自动退出。
 
 while (!feof(fp)) {//文件未结束
    //读文件函数
    if (feof(fp)){
        break;
    }
 }
 
 */

int main8(int argc, const char * argv[])
{
    FILE * fp;
    char buf[1024];
    if ((fp = fopen("/etc/passwd", "r")) == NULL) {
        printf("open file /etc/passwd failed.\n");
        return -1;
    }
    
    while (!feof(fp)) {//文件未结束
        fgets(buf, sizeof(buf), fp);
        if (feof(fp)) {
            break;
        }
        fputs(buf, stderr);
//        FILE * fpw;
//        if ((fpw = fopen("/Users/zhangliang/Desktop/password", "w+")) == NULL ) {
//            printf("error !\n");
//            fclose(fp);
//            return -1;
//        }
//        
//        fwrite(buf, sizeof(buf), 1, fpw);
//        fclose(fpw);
    }
    fclose(fp);
    return 0;
}

/*
(2)文件的错误信息
错误状态指示器仅能判断错误是否发生，不能明确错误的内容，标准文件编程库用于明确错误信息的函数如下：
extern int errno;
#include<string.h>
char *strerror(int errnum);
其中外部变量errno代表了发生错误的代码，函数strerror获取第errnum号错误的详细信息。
 在错误发生后立马调用语句strerror（error）可以获得当前的错误提示信息。
*/


/*
6：文件的缓冲
所谓文件写缓冲，是指文件流在执行输出操作时，并不立刻将数据写入文件，而是先把数据累计到缓冲区，再以块为单位批量输出到文件中，同理，文件读缓冲是指 文件流在执行输入操作时，以块为单位读取文件内容，多余的数据存储在内存中。如果下次读操作的内容刚好在同一块中，则可以直接返回结果，避免一次输入操 作。通过缓冲技术，可以减少低级I/O函数read和write函数的调用次数，从而大大提高软件执行效率。
1)缓冲模式
标准文件编程库采用FILE类型描述文件流，与低级I/O函数相比，最大的特性就是应用及增加了缓冲功能(低级I/O函数只使用了文件系统自带的缓冲功能)，文件的输入输出以"缓冲块"为单位批量完成，并且根据"缓冲块"大小，提供了三种缓冲模式。
(1)全缓冲(_IOFBF)：缓存快大小是全部缓存区。输出时，只有当缓冲区满，才将内容写入文件中。输入时，每次从文件读入数据到缓存区满时为止。一般读写普通磁盘文件采用全缓冲模式。
(2)行缓冲(_IOLBF)：缓存快大小不确定，当I/O操作遇到换行符‘\n’时结束。行缓存常用于交互式比如调用fgets函数从标准输入流stdin中输入字符，当且仅当客户输入回车换行时，函数才返回。
(3)无缓冲(_IONBF)：缓存块大小为0，所有I/O立刻执行，字符被逐个地写入文件或从文件读出。为了便于错误的及时提示，标准错误流stderr采用了无缓冲模式；
 
 2)缓冲函数
 #include<stdio.h>
 void setbuf(FILE *stream,char *buf);
 int setvbuf(FILE *stream,char *buf,int type,size_t size);
 int fflush(FILE *stream);
 setbuf设置文件流stream的缓冲区，参数buf指向一个大小为BUFSIZ的内存块，调用成功后，文件流stream使用该内存块作为新的缓冲区。倘若buf是空指针NULL，文件流stream的缓冲将被完全关闭。缓冲区内存块的定义一般为：
 char buf[BUFSIZ]; ---其中BUFSIZ是stdio.h中的常数，代表缓冲区的大小，常为256的整数倍。
 setvbuf设置了文件流stream的缓冲区和缓冲模式，缓冲模式由参数type确定.
 取值如下：_IOFBF （Full全缓冲)  _IOLBF(Line行缓冲)  _IONBF (No无缓冲)
 参数buf和size只有当缓冲模式设置为全缓冲或行缓冲时有效，此时它们分别指定了缓冲区的位置和大小。函数setvbuf成功调用时返回0，否则返回非0值。
 
 系统将根据文件流的类型自动确定全缓冲或行缓冲模式，引出缓冲设置函数setbuf和setvbuf的有效调用时机：文件流stream被打开但尚未执行读写操作前。
 
 任何时候，都可以使用fflush刷新缓冲区，并将缓冲区的内容强制输出到文件中，参数stream指明了更新的文件流，当其值为NULL时，系统将刷新全部文件流的缓冲区。fflush(fp) 调用成功时，返回0，调用失败返回EOF，并且全局变量errno被设置来指示错误。
 Upon successful completion 0 is returned.  Otherwise, EOF is returned and
 the global variable errno is set to indicate the error.
 */


/*
 实例：
 本处设计了一个缓冲显示的实例：
 */

int main9(int argc, const char * argv[])
{//由于stdout是行缓冲，而stderr是无缓冲，故字符串  2---2  在  1---1  前显示，又由于printf("  3---3  \n");向stdout输出了换行符，故  3---3  在  4---4前输出
    printf("  1---1  ");
    /*fflush(stdout);*/
    
    fprintf(stderr, "  2---2  ");
    
    printf("  3---3  ");//printf("  3---3  \n");
    
    fprintf(stderr, "  4---4\n");
    return 0;
}

int main(int argc, const char * argv[])
{
    main1(argc, argv);
    printf("\n");
    main2(argc, argv);
    printf("\n");
    main3(argc, argv);
    printf("\n");
    main4(argc, argv);
    printf("\n");
    main5(argc, argv);
    printf("\n");
    main6(argc, argv);
    printf("\n");
    main7(argc, argv);
    printf("\n");
    main8(argc, argv);
    printf("\n");
    main9(argc, argv);
    printf("\n");
    return 0;
}

