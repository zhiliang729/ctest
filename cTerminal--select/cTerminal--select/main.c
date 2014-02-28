//
//  main.c
//  cTerminal--select
//
//  Created by zhangliang on 13-11-11.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

/*
 
 select()—I/O多工机制
 
 头文件：#include <sys/time.h>   
        #include <sys/types.h>   
        #include <unistd.h>
 
 定义函数：int select(int n, fd_set * readfds, 
                    fd_set * writefds, fd_set * exceptfds, 
                    struct timeval * timeout);
 
 函数说明：select()用来等待文件描述词状态的改变. 参数n 代表最大的文件描述词加1, 
    参数readfds、writefds 和exceptfds 称为描述词组, 是用来回传该描述词的读, 写或例外的状况. 
    下面的宏提供了处理这三种描述词组的方式：
        FD_CLR(inr fd, fd_set* set); 用来清除描述词组set 中相关fd 的位
        FD_ISSET(int fd, fd_set *set); 用来测试描述词组set 中相关fd 的位是否为真
        FD_SET(int fd, fd_set*set); 用来设置描述词组set 中相关fd 的位
        FD_ZERO(fd_set *set); 用来清除描述词组set 的全部位
    参数timeout为结构timeval, 用来设置select()的等待时间, 其结构定义如下：
        struct timeval
        {
            time_t tv_sec;
            time_t tv_usec;
        };
 
 返回值：如果参数timeout 设为NULL 则表示select ()没有timeout.

 错误代码：
    执行成功则返回已改变状态的文件描述词个数,
    如果返回0 代表在描述词状态改变前已超过timeout 时间, 
    当有错误发生时则返回-1, 错误原因存于errno, 
    此时参数readfds, writefds, exceptfds 和timeout的值变成不可预测。
    EBADF 文件描述词为无效的或该文件已关闭
    EINTR 此调用被信号所中断
    EINVAL 参数n 为负值.
    ENOMEM 核心内存不足
 
 
 
 
 select 在socket中的应用
        Select在Socket编程中还是比较重要的，可是对于初学Socket的人来说都不太爱用Select写
    程序，他们只是习惯写诸如connect、 accept、recv或recvfrom这样的阻塞程序（所谓阻塞方式
    block，顾名思义，就是进程或是线程执行到这些函数时必须等待某个事件的发生，如果事件没有
    发生，进程或线程就被阻塞，函数不能立即返回）。可是使用Select就可以完成非阻塞（所谓非
    阻塞方式non-block，就是进程或线程执行此函数时不必非要等待事件的发生，一旦执行肯定返
    回，以返回值的不同来反映函数的执行情况，如果事件发生则与阻塞方式相同，若事件没有发生则
    返回一个代码来告知事件未发生，而进程或线程继续执行，所以效率较高）方式工作的程序，它能
    够监视我们需要监视的文件描述符的变化情况——读写或是异常。下面详细介绍一下！
函数格式:
        Select的函数格式(我所说的是Unix系统下的伯克利socket编程，和windows下的有区别，
    一会儿说明)：
        int select(int maxfdp,fd_set *readfds,fd_set *writefds,
                    fd_set *errorfds,struct timeval *timeout);
        先说明两个结构体：
        第一，struct fd_set可以理解为一个集合，这个集合中存放的是文件描述符(file 
    descriptor)，即文件句柄，这可以是我们所说的普通意义的文件，当然Unix下任何设备、管道、
    FIFO等都是文件形式，全部包括在内，所以毫无疑问一个socket就是一个文件，socket句柄就是一个
    文件描述符。fd_set集合可以通过一些宏由人为来操作，比如清空集合 FD_ZERO(fd_set *). 将一
    个给定的文件描述符加入集合之中FD_SET(int ,fd_set *). 将一个给定的文件描述符从集合中删除FD_CLR(int ,fd_set*). 检查集合中指定的文件描述符是否可以读写FD_ISSET(int ,fd_set*)。
    一会儿举例说明。
        第二，struct timeval是一个大家常用的结构，用来代表时间值，有两个成员，一个是秒数，另
    一个是毫秒数。具体解释select的参数：
        int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，
    不能错！在Windows中这个参数的值无所谓，可以设置不正确。
        fd_set *readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视
    这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，如果这个集合中有一个文件
    可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则根据timeout参数
    再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表
    示不关心任何文件的读变化。
        fd_set *writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视
    这些文件描述符的写变化的，即我们关心是否可以向这些文件中写入数据了，如果这个集合中有一个文件
    可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则根据timeout参数
    再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表
    示不关心任何文件的写变化。
        fd_set *errorfds同上面两个参数的意图，用来监视文件错误异常。
        struct timeval* timeout是select的超时时间，这个参数至关重要，它可以使select处于
    三种状态，第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监
    视文件描述符集合中某个文件描述符发生变化为止；第二，若将时间值设为0秒0毫秒，就变成一个纯粹的
    非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正
    值；第三，timeout的值大于0，这就是等待的超时时间，即 select在timeout时间内阻塞，超时时
    间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。
 
 返回值：
        负值：select错误 正值：某些文件可读写或出错 0：等待超时，没有可读写或错误的文件
        
 
    在有了select后可以写出像样的网络程序来！举个简单的例子，就是从网络上接受数据写入一个文件中。
 
 
        select()的机制中提供一fd_set的数据结构，实际上是一long类型的数组，
    每一个数组元素都能与一打开的文件句柄（不管是Socket句柄,还是其他
    文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成，
    当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执
    行了select()的进程哪一Socket或文件可读，
 #include <sys/types.h>
 #include <sys/times.h>
 #include <sys/select.h>
 
 int select(nfds, readfds, writefds, exceptfds, timeout)
 int nfds;
 fd_set *readfds, *writefds, *exceptfds;
 struct timeval *timeout;
 
 ndfs：select监视的文件句柄数，视进程中打开的文件数而定,一般设为呢要监视各文件
 中的最大文件号加一。
 readfds：select监视的可读文件句柄集合。
 writefds: select监视的可写文件句柄集合。
 exceptfds：select监视的异常文件句柄集合。
 timeout：本次select()的超时结束时间。（见/usr/sys/select.h，
 可精确至百万分之一秒！）
 
 当readfds或writefds中映象的文件可读或可写或超时，本次select()
 就结束返回。程序员利用一组系统提供的宏在select()结束时便可判
 断哪一文件可读或可写。对Socket编程特别有用的就是readfds。
 几只相关的宏解释如下：
 
 FD_ZERO(fd_set *fdset)：清空fdset与所有文件句柄的联系。
 FD_SET(int fd, fd_set *fdset)：建立文件句柄fd与fdset的联系。
 FD_CLR(int fd, fd_set *fdset)：清除文件句柄fd与fdset的联系。
 FD_ISSET(int fd, fdset *fdset)：检查fdset联系的文件句柄fd是否
 可读写，>0表示可读写。
 （关于fd_set及相关宏的定义见/usr/include/sys/types.h）
 
 这样，你的socket只需在有东东读的时候才读入，大致如下：
 
 ...
 int sockfd;
 fd_set fdR;
 struct timeval timeout = ..;
 ...
 for(;;) {
 FD_ZERO(&fdR);
 FD_SET(sockfd, &fdR);
 switch (select(sockfd + 1, &fdR, NULL, &timeout)) {
 case -1:
 error handled by u;
 case 0:
 timeout hanled by u;
 default:
 if (FD_ISSET(sockfd)) {
 now u read or recv something;
// if sockfd is father and
// server socket, u can now
// accept()
}
}
}

所以一个FD_ISSET(sockfd)就相当通知了sockfd可读。
至于struct timeval在此的功能，请man select。不同的timeval设置
使使select()表现出超时结束、无超时阻塞和轮询三种特性。由于
timeval可精确至百万分之一秒，所以Windows的SetTimer()根本不算
什么。你可以用select()做一个超级时钟。

FD_ACCEPT的实现？依然如上，因为客户方socket请求连接时，会发送
连接请求报文，此时select()当然会结束，FD_ISSET(sockfd)当然大
于零，因为有报文可读嘛！至于这方面的应用，主要在于服务方的父
Socket，你若不喜欢主动accept()，可改为如上机制来accept()。

至于FD_CLOSE的实现及处理，颇费了一堆cpu处理时间，未完待续。

--
讨论关于利用select()检测对方Socket关闭的问题：

仍然是本地Socket有东东可读，因为对方Socket关闭时，会发一个关闭连接
通知报文，会马上被select()检测到的。关于TCP的连接（三次握手）和关
闭（二次握手）机制，敬请参考有关TCP/IP的书籍。

不知是什么原因，UNIX好象没有提供通知进程关于Socket或Pipe对方关闭的
信号，也可能是cpu所知有限。总之，当对方关闭，一执行recv()或read()，
马上回返回-1，此时全局变量errno的值是115，相应的sys_errlist[errno]
为"Connect refused"（请参考/usr/include/sys/errno.h）。所以，在上
篇的for(;;)...select()程序块中，当有东西可读时，一定要检查recv()或
read()的返回值，返回-1时要作出关断本地Socket的处理，否则select()会
一直认为有东西读，其结果曾几令cpu伤心欲断针脚。不信你可以试试：不检
查recv()返回结果，且将收到的东东（实际没收到）写至标准输出...
在有名管道的编程中也有类似问题出现。具体处理详见拙作：发布一个有用
的Socket客户方原码。

至于主动写Socket时对方突然关闭的处理则可以简单地捕捉信号SIGPIPE并作
出相应关断本地Socket等等的处理。SIGPIPE的解释是：写入无读者方的管道。
在此不作赘述，请详man signal。

以上是cpu在作tcp/ip数据传输实验积累的经验，若有错漏，请狂炮击之。

唉，昨天在hacker区被一帮孙子轰得差点儿没短路。ren cpu(奔腾的心) z80

补充关于select在异步(非阻塞)connect中的应用,刚开始搞socket编程的时候
我一直都用阻塞式的connect,非阻塞connect的问题是由于当时搞proxy scan
而提出的呵呵
通过在网上与网友们的交流及查找相关FAQ,总算知道了怎么解决这一问题.同样
用select可以很好地解决这一问题.大致过程是这样的:

1.将打开的socket设为非阻塞的,可以用fcntl(socket, F_SETFL, O_NDELAY)完
成(有的系统用FNEDLAY也可).

2.发connect调用,这时返回-1,但是errno被设为EINPROGRESS,意即connect仍旧
在进行还没有完成.

3.将打开的socket设进被监视的可写(注意不是可读)文件集合用select进行监视,
如果可写,用
getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, sizeof(int));
来得到error的值,如果为零,则connect成功.

在许多unix版本的proxyscan程序你都可以看到类似的过程,另外在solaris精华
区->编程技巧中有一个通用的带超时参数的connect模块.
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>

int main(int argc, const char * argv[])
{
    int sock;
    FILE *fp;
    struct fd_set fds;
    struct timeval timeout = {3, 0};//select 等待3秒， 3秒轮询，要非阻塞就设置0
    char buffer[256] = {0};//256字节的接受缓存区
    
    /* 假定已经建立UDP连接，具体过程不写，简单，当然TCP也同理，主机ip和port都已经给定，要写的文件已经打开
     
     sock=socket(...);
     
     bind(...);
     
     fp=fopen(...); */
    
    while (1) {
        FD_ZERO(&fds);//每次循环都要调用清空集合，否则不恩能够检测描述符变化
        FD_SET(sock, &fds);//添加描述符
        FD_SET(fp, &fds);
        
        int maxfdp = sock>fp?sock+1:fp+1;//描述符最大值加1
        switch (select(maxfdp, &fds, &fds, NULL, &timeout)) {//select使用
            case -1:
                exit(-1);//select 错误，退出程序
                break;
            case 0:
                break;//再次轮询
            default:
                if (FD_ISSET(sock, &fds)) {//测试sock是否可读，即是否网络上有数据
                    recvfrom(sock, buffer, 256, 0, NULL, NULL);//接受网络数据
                    if (FD_ISSET(fp, &fds)) {
                        fwrite(fp, buffer, 0, NULL);//写入文件
                        bzero(buffer, sizeof(buffer));//buffer清空
                    }
                }
                break;
        }
    }
    return 0;
}

