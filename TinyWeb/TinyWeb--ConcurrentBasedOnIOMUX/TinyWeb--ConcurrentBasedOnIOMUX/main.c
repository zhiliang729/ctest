//
//  main.c
//  TinyWeb--ConcurrentBasedOnIOMUX
//
//  Created by zhangliang on 13-11-15.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t * rp);
int parse_uri(char * uri, char * filename, char *cgiargs);
void serve_static(int fd, char * filename, int filesize);
void get_filetype(char * filename, char *filetype);
void serve_dynamic(int fd, char * filename, char * cgiargs);
void clienterror(int fd, char * cause, char * errnum, char * shortmsg, char * longmsg);

void echo(int connfd);
void sigchld_handler(int sig);

void command(void);
/*
 假设要求你编写一个echo服务器，它也能对用户从标准输入键入的交互命令做出响应。在这种情况下，
 服务器必须响应两个相互独立的I/O事件：1）网络客户端发起连接请求，2）用户在键盘上键入命令行。
 我们先等待那个事件呢？没有那个选择是理想的。如果在accept中等待一个连接请求，我们就不能响应输入
 的命令。类似地，如果在read中等待一个输入命令，我们就不能响应任何连接请求。
 针对这种困境的一个解决办法就是I/O多路复用（I/O multiplexing）技术.基本的思路就是使用
 select函数，要求内核挂起进程，只有在一个或多个I/O事件发生后，才将控制返回给应用程序，就像下面
 的实例中一样：
 ·当集合{0，4}中任意描述符准备好读时返回。
 ·当集合{1，2，7}中任意描述符准备好写时返回。
 ·如果在等待一个I/O事件发生时过了152.13秒，就超时。
 select 是一个复杂的函数，有许多不同的使用场景。我们这里只讨论第一种场景：等待一组描述符准备好读。
 select函数：
 头文件包含
 #include <unistd.h>
 #include <sys/types.h>
 函数定义：
 int select(int n, fd_set *restrict fdset, fd_set *restrict NULL,
 fd_set *restrict NULL, struct timeval *restrict NULL);
 函数描述：
 返回已准备好的描述符的非零的个数，若出错则为-1.
 select函数处理类型为fd_set的集合，也叫做描述符集合。逻辑上，我们将描述符集合看
 成一个大小为n的位向量：bn-1, ..., b1, b0   每个位bk对应于描述符k。当且仅当bk=1，
 描述符k才表明是描述符集合的元素。只允许你对描述符集合做三件事：1）分配它们，2）将一个
 此种类型的变量赋值给另一个变量，3）用FD_ZERO、FD_SET、FD_CLR和FD_ISSET宏指令来修
 改和检查它们。针对我们的目的，select函数有两个输入：一个称为读集合的描述符集合（fdset）
 和该杜集合的基数（n）（实际上是任何描述符集合的最大基数）。select函数会一直阻塞，直到
 读集合中至少有一个描述符准备好可以读。当且仅当一个从该描述符读取一个字节的请求不会阻塞
 时，描述符k就表示准备好可以读了。作为一个副作用，select函数修改了参数fdset指向的
 fd_set，指明读集合中一个称为准备好集合（ready set）的子集，这个集合是由读集合中准备
 好可以读了的描述符组成的。函数返回的值指明了准备好集合的基数。注意，由于这个副作用，我们
 必须在每次调用select时都更新读集合。
 
 相关函数：处理描述符集合的宏
 FD_ZERO(fd_set *fdset);//clear all bits in fdset
 FD_CLR(int fd, fd_set * fdset);//clear bit fd in fdset
 FD_SET(int fd, fd_set * fdset);//Turn on bit fd in fdset
 FD_ISSET(int fd, fd_set * fdset);
 */

/*
    使用I/O多路复用的echo服务器。服务器使用select等待监听描述符上的连接请求
 和标准输入上的命令
    遗留问题：一旦它连接到某个客户端，就会连续会送输入航，直到客户端关闭这个连接中它的
 那一端。因此，如果你键入一个命令到标准输入，你讲不会得到响应，直到服务器和客户端之间结束。一个更
 好的方法是更细粒度的多路复用，服务器每次循环（至多）回送一个文本行。
 */

/*
 * 基于I/O多路复用的并发事件驱动服务器
 * I/O多路技术可以用作并发事件驱动（event-driven）程序的基础，在事件驱动中，流是作为某种事件
 * 的结果前进的。一般概念是将逻辑流模型化为状态机。不严格地说，一个状态机（state machine）就
 * 是一组状态（state）、输入时间（input event）和转移（transition）， 其中转移就是将状态和
 * 和输入事件映射到状态。每个转移都将一对（输入状态和输入事件）映射到一个输出状态。自循环（self-loop）
 * 是同一个输入和输出状态之间的转移。通常把状态机化成有向图，其中节点表示状态，
 */

int main(int argc, const char * * argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    fd_set read_set, ready_set;
    
    if (argc != 2) {
        fprintf(stderr, "usage:%s <port>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    
    listenfd = Open_listenfd(port);//打开一个监听描述符
    
    FD_ZERO(&read_set); //clear read set 创建一个空的读集合
    
    //定义由描述符0（标准输入）和监听描述符组成的读集合
    FD_SET(STDIN_FILENO, &read_set);//add stdin to read set
    FD_SET(listenfd, &read_set);//add listenfd to read set
    
    while (1) {
        ready_set = read_set;
        select(listenfd + 1, &ready_set, NULL, NULL, NULL);//select函数会一直阻塞，直到监听描述符或者标准输入准备好可以读
        
        //select 返回后判断哪个描述符准备好可以读了。
        if (FD_ISSET(STDIN_FILENO, &ready_set)) {
            command();//read command line from stdin
        }
        
        if (FD_ISSET(listenfd, &ready_set)) {
            connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);
            echo(connfd);//echo client input until EOF
            Close(connfd);
        }
    }
    return 0;
}

/*
 *doit 函数处理一个http事务。首先，我们读和解析请求行。
 *注意，我们使用rio_readlineb函数读取请求行
 *Tiny 只支持GET方法。如果客户端请求其他（比如POST），我们发送给
 *它一个错误信息，并返回到主程序，主程序随后关闭并等待下一个连接请求。
 *否则，我们读并且忽略任何请求包头
 *然后，我们将URI解析为一个文件名和一个可能为空的cgi参数字符串，
 *并且我们设置一个标志，表明请求的时静态内容还是动态内容。
 *如果文件在磁盘上不存在，我们立即发送一个错误信息给客户端并返回。
 *最后，如果请求的是静态内容，我们就验证该文件是一个普通文件，而我们是有读权限的
 *如果是这样，我们就向客户端提供静态内容。相似地，如果请求的是动态内容，我们就验证
 *该文件是可执行文件，如果是这样，我们就继续，并且提供动态内容。
 */
void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    
    //read request line and headers
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    }
    
    read_requesthdrs(&rio);
    
    //parse URI from GET request
    is_static = parse_uri(uri, filename, cgiargs);
    int res;
    if ((res = stat(filename, &sbuf)) < 0) {
        printf("stat result : %d   errno : %d\n\n", res, errno);
        clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
        return;
    }
    
    if (is_static) {//serve static content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename,"403", "Forbidden", "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, (int)sbuf.st_size);
    }else{//serve dynamic content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
    
    
}

/*
 *Tiny 不使用请求报头中德任何信息。它仅仅调用read_requesthdrs函数来读取并忽略这些报头
 *终止请求报头的空文本行是由回车和换行符对组成的，
 */
void read_requesthdrs(rio_t * rp)
{
    char buf[MAXLINE];
    
    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

/*
 *Tiny假设静态内容的主目录就是它的当前目录，而可执行文件的主目录是./cgi-bin.
 *任何包含字符串cgi-bin的uri都会被认为便是的是对动态内容的请求。
 *默认的文件名是./home.html.
 *parse_uri实现了这些策略。它将uri解析为一个文件名和一个可选的cgi参数字符串。
 *如果请求的时静态内容，我们将清除cgi参数串，然后将uri转换为一个相对的unix路径名，例如./index.html。
 *如果uri是用"/"结尾的，我们将把默认的文件名加在后面。
 *另一方面，如果请求的是动态内容，我们就会抽取出所有的cgi参数，并将uri剩下的部分转换为一个相对的unix文件名
 */
int parse_uri(char * uri, char * filename, char *cgiargs)
{
    printf("%s\n", uri);
    char *ptr;
    
    if (!strstr(uri, "cgi-bin")) {//static content
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (strlen(uri) == 1 && uri[0] == '/') {
            strcat(filename, "/gooagoo-doc/api/platform/index.html");
        }else if(strlen(uri) > 1){
            char straa[MAXLINE] = "./gooagoo-doc/api/platform/";
            filename++;
            strcat(straa, ++filename);
            strcpy(filename, straa);
            printf("load : %s", filename);
        }
        return 1;
    }else{//Dynamic content
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        }else{
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

/*
 *Tiny提供四种不同的静态内容：html文件、无格式的文本文件，以及编码为gif和jpeg格式的图片。
 *这些文件类型占据web上提供的绝大部分静态内容。
 *server_static函数发送一个http响应，其主题包含一个本地文件的内容。
 *首先，我们通过检查文件名的后缀来判断文件类型，并且发送响应行和响应报头给客户端。
 *注意用一个空行终止报头.接着，我们将被请求文件的内容拷贝到已连接描述符fd来发送响应主体。
 *这里的代码是比较微妙的，需要仔细研究。首先以读方式打开filename，并获得它的描述符。
 *接着Mmap函数将被请求文件映射到一个虚拟存储器空间。
 *调用Mmap将文件srcfd的前filesize个字节映射到一个从地址srcp开始的私有只读虚拟存储器区域。
 *一旦将文件映射到存储器，我们就不再需要它的描述符了，所以我们关闭这个文件。
 *执行这项任务失败将导致一种潜在的致命的存储器泄漏。rio_writen函数拷贝从srcp位置开始的filesize个字节
 *（它们当然已经被映射到了所请求的文件）到客户端的已连接描述符。最后Munmap释放了映射的虚拟存储器区域。
 *这对于避免潜在的致命的存储器泄漏是很重要的。
 */
void serve_static(int fd, char * filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
    
    //send response headers to client
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer:Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d \r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));
    
    //send response body to client
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}

/*
 *get_filetype - derive file type from file name
 */
void get_filetype(char * filename, char *filetype)
{
    if (strstr(filename, ".html")) {
        strcpy(filetype, "text/html");
    }else if (strstr(filename, ".gif")){
        strcpy(filetype, "image/gif");
    }else if (strstr(filename, ".jpg")){
        strcpy(filetype, "image/jpeg");
    }else{
        strcpy(filetype, "text/plain");
    }
}


/*
 *Tiny 通过派生一个子进程并在子进程的上下文中运行一个cgi程序，来提供各种类型的动态内容
 *serve_dynamic 函数一开始就向客户端发送一个表明成功的响应行，同时还包括带有信息的server报头。
 *cgi程序负责发送响应的剩余部分。注意，这并不像我们可能希望的那样健壮，
 *因为它没有考虑到cgi程序会遇到某些错误的可能性.在发送了响应的第一部分后，会派生一个新的子进程。
 *子进程用来自请求uri的cgi参数初始化QUERY_STRING环境变量。
 *注意，一个真正的服务器还会在此处设置其他的cgi环境变量。为了简短，我们省略了这一步。
 *还有，我们注意到solaris系统使用的时putenv函数，而不是setenv函数。
 *接下来，子进程重定向它的标准输出到已连接文件描述符，然后加载并运行cgi程序.
 *因为cgi程序运行在子进程的上下文中，它能够访问所有在调用execve函数之前就存在的打开文件和环境变量。
 *因此，cgi程序写到标准输出上的任何东西都将直接送到客户端进程，不会受到任何来自父进程的干涉。
 *期间，父进程阻塞在对wait的调用中，等待当子进程终止的时候，回收操作系统非配给子进程的资源。
 */
void serve_dynamic(int fd, char * filename, char * cgiargs)
{
    char buf[MAXLINE], *emptylist[] = {NULL};
    
    //return first part of http response
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server:Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    printf("filename:%s\n", filename);
    if (Fork() == 0) {//child
        //read server would set all cgi vars here
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);//redirect stdout to client
        Execve(filename, emptylist, environ);//run cgi program
    }
    Wait(NULL);//parent waits for and reaps child
}

/*
 *Tiny 缺乏一个实际服务器的许多错误处理特性。然而，它会检查一些明显的错误，并把它们报告给客户端。
 *clienterror函数发送一个http响应到客户端，在响应行中包含相应的状态码和状态消息，响应主体中包含一个html文件
 *向浏览器的用户解释这个错误。回想一下，html响应应该指明主体中内容的大小和类型。因此，我们选择创建html内容为
 *一个字符串，这样一来我们可以简单地确定它的大小。还有，请注意我们的输出使用的都是rio_writen函数
 */
void clienterror(int fd, char * cause, char * errnum, char * shortmsg, char * longmsg)
{
    char buf[MAXLINE], body[MAXBUF];
    
    //build the http response body
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffff"">\r\n", body);
    sprintf(body, "%s%s:%s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
    
    
    //print the http response
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

void echo(int connfd)
{
    doit(connfd);
}

void sigchld_handler(int sig)
{
    while (waitpid(-1, 0, WNOHANG) > 0) {
        ;
    }
    return;
}

void command(void)
{
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin)) {
        exit(0);//EOF
    }
    printf("%s", buf);//Process the input command
}

