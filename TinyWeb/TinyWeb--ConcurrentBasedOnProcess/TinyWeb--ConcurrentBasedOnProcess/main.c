//
//  main.c
//  TinyWeb--ConcurrentBasedOnProcess
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

/*
 基于进程的并发服务器
 
 1.几点重要说明：
    首先，通常服务器会运行很长时间，所以我们必须要包括一个SIGCHLD处理程序，来回收僵死
 （zombie）子进程的资源。因为当SIGCHLD处理程序执行时，SIGCHLD信号是阻塞的，而Unix信号是不排
 队的，所以SIGCHLD处理程序必须准备好回收多个僵死子进程的资源。
    其次，父子进程必须关闭它们各自的connfd拷贝。这对父进程而言尤为重要，它必须关闭它的已连接
 描述符，以避免存储器泄漏。
    最后，因为套接字的文件表表项中的引用计数，直到父子进程的connfd都关闭了，到客户端的连接才
 会终止
 
 
 2.关于进程的优劣
    对于在父、子进程间共享状态信息，进程有一个非常清晰的模型：共享文件表，但是不共享用户地址空间。
 进程有独立的地址空间既是优点也是缺点。这样一来，一个进程不可能不小心覆盖另一个进程的虚拟存储器，
 这就消除了许多令人迷惑的错误--这是一个明显的优点。
    另一方面，独立的地址空间使得进程共享状态信息变得更加空难。为了共享信息，它们必须使用显式的
 IPC（进程间通信）机制。基于进程的设计的另一个缺点是，它们往往比较慢，因为进程控制和IPC的开销很高。
 
 
 3.Unix IPC
    waitpid函数和Unix信号是基本的IPC机制，它们允许进程发送小消息到同一主机上的其他进程。
    套接字接口也是IPC的一种重要形式，它允许不同主机上的进程交换任意的字节流。然而，术语Unix IPC
 通常指的是所有允许进程和同一台主机上其他进程进行通信的技术。其中包括管道、先进先出（FIFO）、
 系统V共享存储器，以及系统V信号量（semaphore）。这些机制超出了我们的讨论范围。
 */
int main(int argc, const char * * argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    
    if (argc != 2) {
        fprintf(stderr, "usage:%s <port>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    
    Signal(SIGCHLD, sigchld_handler);
    
    listenfd = Open_listenfd(port);
    
    while (1) {
        connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
        if (Fork() == 0) {//父进程派生一个子进程来处理每个新的连接请求
            Close(listenfd);//child closes its listening socket (important!)
            echo(connfd);//child services client
            Close(connfd);//child colses connection with client
            exit(0);
        }
        Close(connfd);//parent closes connected socket (important!)
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

