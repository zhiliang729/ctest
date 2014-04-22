//
//  main.c
//  pipe
//
//  Created by zhangliang on 14-4-21.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

/*
    进程与人一样，都需要交流。unix为进程提供了丰富的交流方式，其中公开的交流方式有信号量、消息队列、共享内存、有名管道和文件等，秘密的信息仅限于交流双方知道的交流方式有信号通信、无名管道通信和socket通信等。
 
    管道是Unix操作系统最强大和最具特色的性能之一，从功能上讲，管道机制提供了一种在各进程之间大量传输信息的通道；而从本质上讲，管道是一个特殊的文件。管道是以Unix文件系统为基础而设计的，它具有对普通文件操作的一般特性，如用于普通文件的打开（OPEN）、读（READ）、写（WRITE）、关闭（CLOSE）等系统调用同样可以作用于管道， 管道是一种队列类型的数据结构，它的数据从一端输入，另一端输出。管道最常用是连接两个进程的输入输出，即把一个进程的输出编程另一个进程的输入。shell中存在专门的管道运算符“|”，例如shell命令：ps -ef | grep init  命令ps -ef分析当前运行的全部进程，并将结果打印到屏幕上，进程grep init从输入的字符串中查找包含字符init的字串，并打印结果，这两个命令通过管道符连接起来后就成了一个新的应用：查找正在运用的、命名中包含字符串init的进程。
    Unix操作系统下的管道分为无名管道（PIPE）和有名管道（FIFO）两种，它们在不同的领域下有效地完成了相关进程之间的数据共享和信息交换，从而保证了进程间的协同工作，前者在父子进程中流行，后者由于可以独立成磁盘文件而存在，因而能够被无血缘关系的进程所共享。与Unix系统提供的其他进程通信机制，如信号（SIGNAL）、消息传送（MESSAGE PASSING）、共享内存（SHARED MEMORY）等相比，管道机制具有如下三个突出的特点：
        （1）发送进程能以比较简单的方式，源源不断地把产生的消息以自然流的方式写入管道，而不需要考虑对每次传送的信息长度的限制。
        （2）接收进程能在适当的时机从管道按需提取信息，同样也不必以固定的消息长度为单位进行。
        （3）发送和接收进程都能以一定的方式了解对方进程是否存在，并且可以通过管道的现存消息情况（管道空、管道有信息、管道满）等相互协调动作。
    常见的无名管道流有单向管道流的模型和双向管道流的模型两种。而单向管道流又可分为从父进程向子进程的管道和从子进程到父进程的管道等两种。
 
    无名管道（PIPE）
    无名管道是一个特殊的打开文件，只能在程序中由系统调用PIPE（P）生成。其中参数P的定义为：
        INT P[2]；
    无名管道由一个在基本文件系统存储设备上的INODE，一个与其相连的内存INODE，两个打开文件控制块（分别对应管道的信息发送端和信息接收端）及其所属进程的描述信息来标识，在系统执行PIPE（P）命令行之后生成。在P[0]中返回管道的读通道打开文件描述符；在P[1]中返回管道的写通道打开文件描述符。从结构上看，无名管道没有文件路径名，不占用文件目录项，因此文件目录结构中的链表不适用于这种文件，它只是存在于打开文件结构中的一个临时文件，随其所依附的进程的生存而生存，当进程终止时，无名管道也随之消亡。占用两个文件描述符，不能被非血缘关系的进程共享，一般应用在父子进程中。
 
 
    有名管道（FIFO）
    无名管道存在着如下两个严重的缺点：
        第一、无名管道只能用于连接具有共同祖先的进程；
        第二、无名管道是依附进程而临时存在的，并不是永久存在于系统中。
    为了弥补无名管道的不足，伴随Unix SYSTEM III的出台推出了一种无名管道的变种——有名管道，它常被称为FIFO。有名管道除继承了无名管道的所有特性优点之外，还摒弃了无名管道的两个缺点。首先，FIFO是一种永久性的机构，它具有普通的Unix系统文件名。在系统下可利用MKNOD命令建立永久的管道，除非刻意删除它，否则它将一直保持在系统中。其次，正是由于有名管道以文件名来标识，所以只要事先约定某一特定文件名，那样所有知道该约定的服务进程，不论它们之间是否有亲属关系，都可以方便地利用管道进行通信。
 
 
 */

/*
 1.无名管道的建立
    unix中一切皆为文件，管道也是文件的一种，称为管道文件。当系统创建一个管道时，它返回两个文件描述符：一个文件以只写打开，作为管道的输入端；另一个文件以只读打开，作为管道的输出端。在unix中，采用函数pipe创建无名管道，其原型为
        #include <unistd.h>
        int pipe(int fildes[2]);    1是输入， 0是输出
        函数pipe调用成功时返回0，否则返回-1.
    在系统执行pipe（fildes）命令行之后生成。在fildes[0]中返回管道读通道打开文件描述符；在fildes[1]中返回管道的写通道打开文件描述符。
 */

/*
 2 单向管道流模型
    管道的两端（输入端和输出端）被一个进程控制没有意义，如果管道的两端分别控制在不同的进程中，这两个进程之间就能通信。输入端写入，输出端接收。
 */

//从父进程流向子进程的管道      在父进程创建无名管道并产生子进程后，父子进程均拥有管道两端的访问权。此时关闭父进程的管道输出端、关闭子进程的管道输入端，就形成一个父进程到子进程的管道流，数据由父进程写入，从子进程读出。创建从父进程到子进程的管道过程：
//1. 创建管道，返回无名管道的两个文件描述符fildes[0]和fildes[1].
//      int fildes[2];
//      pipe(fildes);
//2. 创建子进程，子进程继承无名管道文件描述符
//3. 父进程关闭管道的输出端，即关闭只读文件描述符fields[0].
//      close(fildes[0]);//关闭输出
//4. 子进程关闭管道的输入端，即关闭只写文件描述符fildes[1].
//      close(fildes[1]);//关闭输入
// 如此，则为从父进程到子进程的管道， 子进程流向父进程的管道与上面只有3，4步相反。
#include <stdio.h>
#include <stdlib.h>

/*创建一个管道，父进程向管道写入一行字符，子进程读取数据并打印到屏幕上*/
#include <unistd.h>
#include <string.h>
void main1(int argc, const char * argv[])
{
    int fildes[2];
    pid_t pid;
    ssize_t j;
    char buf[256];
    if (pipe(fildes) < 0) {//创建管道
        fprintf(stderr, "pipe error!\n");
        return;
    }
    
    if ((pid = fork()) < 0) {//创建子进程
        fprintf(stderr, "fork error\n");
        return;
    }
    
    if (pid == 0) {//子进程
        close(fildes[1]);/*子进程关闭管道输入*/
        memset(buf, 0, sizeof(buf));
        j = read(fildes[0], buf, sizeof(buf));/*读管道*/
        fprintf(stderr, "[child] buf = [%s] len[%ld]\n", buf, j);
        return;
    }
    
    
                                            /*父进程*/
    close(fildes[0]);                       /*父进程关闭管道输出*/
    write(fildes[1], "Hello!", strlen("Hello!"));/*写管道*/
    write(fildes[1], "world!", strlen("world!"));/*写管道*/
    
    /*
     
     父进程分别向管道中写入字符串，子进程一次性从管道中读出并答应这些数据
     经验：在进程的通信中，我们无法判断每次通信中报文的字节数，即无法对数据流进行自动拆分，从而发生了上面子进程一次性读取父进程两次通信的报文的情况。为了能正常拆分发送报文，通常方法有：
            1.固定长度。 发送进程每次写入固定字节的数据，接收进程每次读取固定字节的内容，报文中多余部分填充空格或者填充0，根据填充的位置，本方法又可分为左对齐和右对齐两种。比如定义固定长度为10，为了传输字符串“Hello！”， 则左对齐报文应该为“Hello！”“”， 右对齐报文为“”“Hello！”，其中“”为空格或者ascii码的0；
            2.显式长度。 每条报文由“长度域”和“数据域”组成，“长度域”大小固定，存储了“数据域”的长度，分为字符串型和整型两种，“数据域”是传输的实际报文数据。接收进程先获取“长度域”数据，转换为“数据域”的长度，再读取相应的长度的信息即为“数据域”内容。比如为了传输“hello！”，报文写为”0006Hello！“。
            3.短连接。 每当进程间需要通信时，创建一个通信线路，发送一条报文后立即废弃这条通信线路。这种方法在socket通信中很常用。
     */
}


/*
 3. 双向管道流模型
    管道是进程之间的一种单相交流方法，要实现进程间的双向交流，就必须通过两个管道来完成。创立双向管道的过程：
        1.创建管道，返回两个无名管道文件描述符对fildes1和fildes2.
            int fildes1[2], fildes2[2];
            pipe(fildes1);
            pipe(fildes2);
        2.创建子进程，子进程中继承两个管道
        3.父进程关闭管道1的输出端fildes1[0], close(fildes1[0]);
        4.子进程关闭管道1的输入端fildes1[1], close(fildes1[1]);
        5.父进程关闭管道2的输入端fildes2[1], close(fildes2[1]);
        6.子进程关闭管道2的输出端fildes2[0], close(fildes2[0]);
 */

//本处设计一个父子进程间双向管道通信的实例，父进程首先向子进程传送两次数据，再接收子进程传送过来的两次数据，为了能够正确拆分数据流，从父进程流向子进程的管道1采用“固定长度“方法传送数据，从子进程流向父进程的管道2采用”显式长度“方法回传数据
//1固定长度    管道输入时固定写入len个字符，管道输出时也固定读取len个字符，采用了左对齐方式，多余部分填充ASCII码0，“固定长度”数据的管道操作方法：

void WriteG(int fd, char *str, int len)/*写入固定长度报文*/
{
    char buf[255];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", str);
    write(fd, str, len);/*管道输入*/
}

char * ReadG(int fd, int len)/*读取固定长度报文, 记得去free内存*/
{
    int bufSize = 255;
    char *buf;
    buf = malloc(bufSize);
    memset(buf, 0, bufSize);
    read(fd, buf, len);/*管道输出*/
    return buf;/*返回管道输出数据*/
}

//2 显式长度    显式长度报文的“长度域”可分为整型和字符串型两种，以4字节“长度域”传书数据“Hello！”为例，整型长度域报文为：0x06， 0x00, 0x00, 0x00, "Hello!" 出于兼容性考虑，一般采用网络字节顺序的整型。字符串型长度域报文为：“0006Hello！”
//本例采用“4字节字符串“+“数据”的格式传送报文，输入时先写入数据长度再写入数据内容   显式长度输入操作
void WriteC(int fd, char * str)
{
    char buf[255];
    sprintf(buf, "%04lu%s", strlen(str), str);/*报文头增加报文长度*/
    write(fd, buf, strlen(buf));
}

//管道的输出操作分为两步：1.读入4字节，转化为整型长度。2.读入数据，字节数为步骤1中的整型数。
char * ReadC(int fd)//记得free内存
{
    int bufSize = 255;
    char *buf;
    buf = malloc(bufSize);
    ssize_t i;
    memset(buf, 0, bufSize);
    read(fd, buf, 4);/*读入长度域*/
    i = atoi(buf);/*转化长度域为整型   长度域采用字符串类型存储，函数atoi将之转换为整型，并作为参数供函数read调用*/
    read(fd, buf, i);/*读入后续报文*/
    return buf;/*返回输入的报文*/
}

//父子进程双向通信实例
void main2(int argc, const char * argv[])
{
    int fildes1[2], fildes2[2];
    pid_t pid;
    char buf[255];
    memset(buf, 0, sizeof(buf));
    if (pipe(fildes1) < 0 || pipe(fildes2) < 0) {//创建管道
        fprintf(stderr, "pipe error!\n");
        return;
    }
    
    if ((pid = fork()) < 0) {//创建子进程
        fprintf(stderr, "fork error!\n");
        return;
    }
    
    if (pid == 0) {//子进程
        close(fildes1[1]);
        close(fildes2[0]);
        char * bb = ReadG(fildes1[0], 10);
        strcpy(buf, bb);//读取管道数据
        free(bb);
        fprintf(stderr, "[child] buf=[%s]\n", buf);
        WriteC(fildes2[1], buf);//回传父进程
        
        bb = ReadG(fildes1[0], 10);
        strcpy(buf, bb);//读取管道数据
        free(bb);
        
        fprintf(stderr, "[child] buf=[%s]\n", buf);
        WriteC(fildes2[1], buf);//回传父进程
        return;
    }
    
    //父进程
    close(fildes1[0]);
    close(fildes2[1]);
    WriteG(fildes1[1], "hello!", 10);/*固定长度输入*/
    WriteG(fildes1[1], "world!", 10);/*固定长度输入*/
    
    char * bc = ReadC(fildes2[0]);
    fprintf(stderr, "[father] buf=[%s]\n", bc);/*显式输出*/
    free(bc);
    
    bc = ReadC(fildes2[0]);
    fprintf(stderr, "[father] buf=[%s]\n", bc);/*显式输出*/
    free(bc);
}


/*
 4. 连接标准I/O的管道模型
    管道在shell中最常见的应用是连接不同进程的输入输出，比如使用a进程的输出变成b进程的输入等。
    例1 分别重定向标准输入、标准输出、标准错误输出到文件描述符fd1， fd2， fd3中。 
        复制文件描述符fd1到文件描述符0中即可重定向标准输入，其他类似
        dup2(fd1, 0); dup2(fd2, 1); dup2(fd3, 2); 当执行完dup2（fd1, 0）后，文件描述符0就对应到了fd1所对应的文件中，而标准输出函数，printf，puts等任然向描述符0中写入内容，从而达到了重定向的效果。
    1.模型
        使用管道将父进程标准输入连接到子进程标准输入的方法。(父进程流向子进程)
            1.创建管道，返回两个无名管道文件描述符fildes[0]和fildes[1].
            2.创建子进程，子进程中继承无名管道文件描述符。
            3.父进程关闭管道的输出端，即关闭只读文件描述符fildes[0].
            4.父进程将标准输出（stdout, 文件描述符1）重定向为文件描述符fildes[1].
            5.子进程关闭管道的输入端，即关闭只写文件描述符fildes[1].
            6.子进程将标准输入（stdin, 文件描述符0）重定向为fildes[0]
 */
//设计一个降幅进程标准输出流连接到子进程标准输入流的管道，父进程向stdout输出“Hello”直接转移到子进程的stdin，由子进程gets（buf）语句获取。
#include <stdio.h>
void main3(int argc, const char * argv[])
{
    int fildes[2];
    pid_t pid;
    char buf[255];
    signal(SIGPIPE, SIG_IGN);
    if (pipe(fildes) < 0 || (pid = fork()) < 0) {//创建管道和子进程
        fprintf(stderr, "error!\n");
        return;
    }
    
    if (pid == 0) {//子进程
        close(fildes[1]);
        dup2(fildes[0],STDIN_FILENO);//重定向标准输入到fildes[0]中
        close(fildes[0]);//关闭fildes[0],即重定向之后，关闭原来的文件描述符
        gets(buf);//读入输入，其实就是读取父进程输出
        fprintf(stderr, "child:[%s]\n", buf);
        return;
    }
    
    //父进程
    close(fildes[0]);
    dup2(fildes[1], STDOUT_FILENO);//重定向标准输出到fildes[1]中
    close(fildes[1]);//关闭原来的
    char bb[255];
    scanf("%s", bb);
    
    printf("Hello, World!");//输出函数输出到标准输出的内容被重定向到fildes[1],通过管道传道子进程的输入端。
    puts(bb);//输出，同时增加子进程的输入信息
    
    return;
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
    main3(argc, argv);
    return 0;
}


//NAME
//pipe -- create descriptor pair for interprocess communication
//
//SYNOPSIS
//#include <unistd.h>
//
//int
//pipe(int fildes[2]);
//
//DESCRIPTION
//The pipe() function creates a pipe (an object that allows unidirectional data flow) and
//allocates a pair of file descriptors.  The first descriptor connects to the read end of
//the pipe; the second connects to the write end.
//
//Data written to fildes[1] appears on (i.e., can be read from) fildes[0].  This allows
//the output of one program to be sent to another program: the source's standard output is
//set up to be the write end of the pipe; the sink's standard input is set up to be the
//read end of the pipe.  The pipe itself persists until all of its associated descriptors
//are closed.
//
//A pipe whose read or write end has been closed is considered widowed.  Writing on such a
//pipe causes the writing process to receive a SIGPIPE signal.  Widowing a pipe is the
//only way to deliver end-of-file to a reader: after the reader consumes any buffered
//data, reading a widowed pipe returns a zero count.
//
//The generation of the SIGPIPE signal can be suppressed using the F_SETNOSIGPIPE fcntl
//command.
//
//RETURN VALUES
//On successful creation of the pipe, zero is returned. Otherwise, a value of -1 is
//returned and the variable errno set to indicate the error.
//
//ERRORS
//The pipe() call will fail if:
//
//[EFAULT]           The fildes buffer is in an invalid area of the process's address
//space.
//
//[EMFILE]           Too many descriptors are active.
//
//[ENFILE]           The system file table is full.
//
//SEE ALSO
//sh(1), fork(2), read(2), socketpair(2), fcntl(2), write(2)


