//
//  main.c
//  ConcurrentProgramming
//
//  Created by zhangliang on 14-4-10.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>

int main1(int argc, const char * argv[])
{
    int i;
	fprintf(stderr, "argc=[%d]\n", argc);
	for (i=0; i<argc; i++)
	{
		fprintf(stderr, "argv[%d]=[%s]\n\n", i, argv[i]);
	}
    return 0;
}

#include<unistd.h>

void main2(int argc, const char * argv[])
{
    printf("pid=[%d], gid=[%d], ppid=[%d]\n", getpid(), getpgrp(), getppid());
	printf("uid=[%d], euid=[%d], gid=[%d], egid=[%d]\n", getuid(), geteuid(), getgid(), getegid());
}

extern char * * environ;
void main3(int argc, const char * argv[])
{
    char * * p = environ;
    while (* p) {
        fprintf(stderr, "%s\n", * p);
        p++;
    }
}

#include <stdlib.h>

void main4(int argc, const char * argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
        fprintf(stderr, "%s = %s \n", argv[i], getenv(argv[i]));
    }
}

void main5(int argc, const char * argv[])
{
    pid_t pid;
    int i = 0;
    printf("888888888888888888888888888888888\n");
    fprintf(stderr, "Process Begin. pid = [%d], i = [%d]\n", getpid(), i);
    if ((pid = fork()) > 0) {
        fprintf(stderr, "Parent pid = [%d], child pid = [%d]\n", getpid(), pid);
    }else if(pid == 0){
        fprintf(stderr, "child pid = [%d]   啊  [%d]\n", getpid(), pid);
        i++;
    }else{
        fprintf(stderr, "Fork failed.\n");
    }
    fprintf(stderr, "Process End. pid = [%d], i = [%d]\n", getpid(), i);
}

void main6(int argc, const char * argv[])
{
    fprintf(stderr, "--begin--\n");
    execl("/bin/ls", "-a", 0);
    fprintf(stderr, "--end--\n");
}

void main7(int argc, const char * argv[])
{//fork----exec模式
    pid_t pid;
    if ((pid = fork()) == 0) {//子进程
        main6(argc, argv);
    }else if(pid > 0){//父进程
        fprintf(stderr, "fork child pid = [%d]\n", pid);
    }else//调用失败
        fprintf(stderr, "Fork failed.\n");
}

void main8(int argc, const char * argv[])
{
    //vfork----exec模式
    //大多数情况下，fork--exec模型再创建子进程后立即执行新的程序，并不使用父进程的数据，因此unix提供了一个更快的函数vfork，原型为
    //#include <unistd.h>
    //pid_t vfork();
    
    

    //system模式    阻塞调用它的进程，并执行string中的shell命令，它可以理解为vfork-exec模型的简易表达方式
//    if (vfork() == 0) {
//        execl("/bin/bash", "-C", string, 0);
//        exit(0);
//    }
    
    
    char cmd[] = {"/bin/ls -l ./"};
    system(cmd);
}

void main9(int argc, const char * argv[])
{//进程休眠
    //#inlcude <unistd.h>
    //unsigned int sleep(unsigned int seconds);
    //进程调用函数sleep后休眠seconds秒，直到时间结束或收到不可忽略信号为止。
    
    //进程终止
    //#include <stdlib.h>
    //void exit (int status);
    //进程调用函数exit终止自己的运行，并且释放所有的系统资源。参数status的低8位记载了进程的终止状态，进程终止后将这个状态返回给它的父进程
    
    pid_t i, j;
    int status;
    printf("sizeof(int) = %lu\n", sizeof(int));
    if ((i = fork()) == 0) {
        fprintf(stderr, "child begin. pid = [%d]\n", getpid());
        sleep(5);
        fprintf(stderr, "child end. pid = [%d]\n", getpid());
        exit(10);
    }
    j = wait(&status);
    fprintf(stderr, "child pid = [%d], exit pid[%d], status[%d]\n", i, j, status);
    
    printf("10<<8   : %d", 10<<8);
    
    //在unix中存在大量的后台程序，利用父子进程的同步控制可以轻松地判断这些后台进程的运行状态，并且在进程非正常退出时如core-dump等自动重启。
}

/*
 本处设计一个进程并发监控程序运行的例子，父进程首先fork两个子进程，然后监控子进程的状态，一旦发现有子进程终止则重启子进程。
 */
#include <signal.h>
void main10(int argc, const char * argv[])
{
    pid_t pid1, pid2, pid3;
    int status;
    while (1) {/*利用循环来重启*/
        if ((pid1 = fork()) < 0) {
            exit(1);
        }else if (pid1 == 0){/*子进程1*/
            sleep(30);
            exit(0);
        }
        
        
        if ((pid2 = fork()) < 0) {
            exit(1);
        }else if (pid2 == 0){/*子进程2*/
            sleep(40);
            exit(0);
        }
        
        /*------------父进程---------------*/
        fprintf(stderr, "Fork child pid = [%d]     [%d]\n", pid1, pid2 );
        pid3 = wait(&status);/*并发，监控子进程状态*/
        kill(pid1, SIGTERM);/*杀死子进程1*/
        kill(pid2, SIGTERM);/*杀死子进程2*/
        fprintf(stderr, "Kill childpid = [%d]      [%d], exit pid[%d], status = [%d] \n", pid1, pid2, pid3, status);
        pid3 = wait(&status);/*两个子进程结束，故必须要wait两次*/
        sleep(10);
    }
}

void main11(int argc, const char * argv[])
{//僵死进程
    /*僵死进程是已经终止，但没有从进程表中清除的进程   产生僵死进程的例子*/
    pid_t pid1;
    if ((pid1 = fork()) == 0) {
        printf("child[%d]\n", getpid());
        exit(0);
    }
    
    
    /*wait（）  1*/
    printf("parent [%d]\n", getpid());
    sleep(60);
    
    
    
    /*
     
     子进程并没有如期消失， 而是转化为僵死进程继续占用系统进程表资源。如果系统存在大量的僵死进程，就会过多占用进程表空间，影响新进程的产生。对于僵死进程，不能奢望通过kill命令杀死之，因为它已经“死”了， 不再接收任何系统信号。
     当子进程终止时，它释放资源，并发送SIGCHLD信号通知父进程。父进程接收SIGCHLD信号，调用wait返回子进程的状态，并且释放系统进程表资源。故如果子进程先于父进程终止，而父进程没有调用wait接收子进程信息，则子进程将转化为僵死进程，直到其父进程结束。
        一旦直到僵死进程的原因，我们就可以采用如下方法预防僵死进程：
        1.wait法，父进程主动调用（或者接收到SIGC(H)LD信号后调用）wait接收子进程的死亡报告，释放子进程占用的系统进程表资源。如将1打开
        2.托管法。 如果父进程先于子进程而死，则它的所有子进程转由进程init(mac os x 上1为launchd， 0为kernel_task)领养，则它所有子进程的父进程id号变为1（进程init的标识为1）。当子进程结束时init为其释放进程表资源。（******实现托管法的技巧是调用fork两次，让子进程退出，子子进程的父进程更改为init，这样父进程就不必理会子子进程的处理了。例子如下main12（））
        3.忽略SIGC(H)LD信号。 当父进程忽略SIGC(H)LD信号后，即使不执行wait，子进程结束时也不会产生僵死进程。
        4.捕获SIGC(H）LD信号。 当父进程捕获SIGC(H)LD信号，并在捕获函数代码中等待（wait）子进程。
     */
}


void main12(int argc, const char * argv[])
{/*托管给init进程，两次fork*/
    pid_t pid1;
    if ((pid1 = fork()) == 0) {/*第一次fork*/
        printf("pid1 : [%d]  ,   child[%d]  parent1:[%d]\n",  pid1, getpid(), getppid());
        if ((pid1 = fork()) == 0) {/*第二次fork*/
            printf("pid1 : [%d]  , 33   child[%d]    parent1:[%d]\n", pid1, getpid(), getppid());
            sleep(20);
            exit(0);/*第二次创建的子进程退出*/
        }
        exit(0);/*第一次创建的子进程退出*/
    }
    
    waitpid(pid1, 0, 0);
    printf("parent[%d]     parent:[%d]\n", getpid(), getppid());
    sleep(60);
    
//    Upon successful completion, fork() returns a value of 0 to the child
//    process and returns the process ID of the child process to the parent
//    process.  Otherwise, a value of -1 is returned to the parent process, no
//    child process is created, and the global variable errno is set to indi-
//    cate the error.
    
//    Getpid() returns the process ID of the calling process.  The ID is guar-
//    anteed to be unique and is useful for constructing temporary file names.
//        
//    Getppid() returns the process ID of the parent of the calling process.
}

void main13(int argc, const char * argv[])
{
    /*守护进程
     所谓守护进程（又名精灵进程）是一种形象的说法，它是一个在后台长期运行的进程，它们独立于控制终端，周期性地执行某项任务，或者阻塞直到事件发生，摸摸地守护着计算机系统的正常运行。在unix应用中，大部分socket通信服务程序都以守护进程方式执行。
     
     守护进程编程模型
     完成一个守护进程的编写至少包括以下几项：
     1.后台运行
        后台运行的最大特点就是不再接收终端输入，托管法可以实现这一点
        pid_t pid1;
        pid1 = fork();
        if(pid > 0) exit(0);//父进程退出
        //子进程继续运行
        父进程结束，shell重新接管终端控制权，子进程移交init托管。
     2.独立于终端控制
        在后台进程的基础上，脱离原来shell的进程组和session期（session是一个或多个进程组的集合，每个session独占一个终端），自立门户为新进程组的会话组长进程，与原终端脱离关系。自立新进程组的函数原型为：pid_t setsid();
        函数setsid创建一个新的session和进程组，并以调用进程的id号来设置新成立的进程组id。函数调用成功时，返回创建的进程组id号，否则返回-1。
     3.清除文件创建掩码（umask）
        umask（0）；
     4.处理信号
        为了预防父进程不等待子进程结束而导致子进程僵死，必须忽略或者处理SIGCHLD信号。其中忽略SIGCHLD信号的方法为：
        signal（SIGCHLD, SIG_IGN）；
     
     //守护进程独立于控制终端，它们一般以文件日志的方式进行信息输出。
     */
}

int main(int argc, const char * argv[])
{
//    main1(argc, argv);
//    main2(argc, argv);
//    main3(argc, argv);
//    main4(argc, argv);
//    main5(argc, argv);
//    
//    printf("999999999999999999999999999\n");
//    main7(argc, argv);
//    
//    
//    printf("999999999999999999999999999\n");
//    main8(argc, argv);
    
//    printf("777777777777777777777777777\n");
//    main9(argc, argv);
    
//    main10(argc, argv);
    
//    main11(argc, argv);
    
    main12(argc, argv);
    
    return 0;
}

/*
 NAME
 wait, wait3, wait4, waitpid -- wait for process termination
 
 SYNOPSIS
 #include <sys/wait.h>
 
 pid_t
 wait(int *stat_loc);
 
 pid_t
 wait3(int *stat_loc, int options, struct rusage *rusage);
 
 pid_t
 wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);
 
 pid_t
 waitpid(pid_t pid, int *stat_loc, int options);
 
 DESCRIPTION
 The wait() function suspends execution of its calling process until
 stat_loc information is available for a terminated child process, or a
 signal is received.  On return from a successful wait() call, the
 stat_loc area contains termination information about the process that
 exited as defined below.
 
 The wait4() call provides a more general interface for programs that need
 to wait for certain child processes, that need resource utilization sta-
 tistics accumulated by child processes, or that require options.  The
 other wait functions are implemented using wait4().
 
 The pid parameter specifies the set of child processes for which to wait.
 If pid is -1, the call waits for any child process.  If pid is 0, the
 call waits for any child process in the process group of the caller.  If
 pid is greater than zero, the call waits for the process with process id
 pid.  If pid is less than -1, the call waits for any process whose
 process group id equals the absolute value of pid.
 
 The stat_loc parameter is defined below.  The options parameter contains
 the bitwise OR of any of the following options.  The WNOHANG option is
 used to indicate that the call should not block if there are no processes
 that wish to report status.  If the WUNTRACED option is set, children of
 the current process that are stopped due to a SIGTTIN, SIGTTOU, SIGTSTP,
 or SIGSTOP signal also have their status reported.
 
 If rusage is non-zero, a summary of the resources used by the terminated
 process and all its children is returned (this information is currently
 not available for stopped processes).
 
 When the WNOHANG option is specified and no processes wish to report sta-
 tus, wait4() returns a process id of 0.
 
 The waitpid() call is identical to wait4() with an rusage value of zero.
 The older wait3() call is the same as wait4() with a pid value of -1.
 
 The following macros may be used to test the manner of exit of the
 process.  One of the first three macros will evaluate to a non-zero
 (true) value:
 WIFEXITED(status)
 True if the process terminated normally by a call to _exit(2) or
 exit(3).
 
 WIFSIGNALED(status)
 True if the process terminated due to receipt of a signal.
 
 WIFSTOPPED(status)
 True if the process has not terminated, but has stopped and can
 be restarted.  This macro can be true only if the wait call spec-
 ified the WUNTRACED option or if the child process is being
 traced (see ptrace(2)).
 
 Depending on the values of those macros, the following macros produce the
 remaining status information about the child process:
 
 WEXITSTATUS(status)
 If WIFEXITED(status) is true, evaluates to the low-order 8 bits
 of the argument passed to _exit(2) or exit(3) by the child.
 
 WTERMSIG(status)
 If WIFSIGNALED(status) is true, evaluates to the number of the
 signal that caused the termination of the process.
 
 WCOREDUMP(status)
 If WIFSIGNALED(status) is true, evaluates as true if the termina-
 tion of the process was accompanied by the creation of a core
 file containing an image of the process when the signal was
 received.
 
 WSTOPSIG(status)
 If WIFSTOPPED(status) is true, evaluates to the number of the
 signal that caused the process to stop.
 
 NOTES
 See sigaction(2) for a list of termination signals.  A status of 0 indi-
 cates normal termination.
 
 If a parent process terminates without waiting for all of its child pro-
 cesses to terminate, the remaining child processes are assigned the par-
 ent process 1 ID (the init process ID).
 
 If a signal is caught while any of the wait() calls is pending, the call
 may be interrupted or restarted when the signal-catching routine returns,
 depending on the options in effect for the signal; see intro(2), System
 call restart.
 
 RETURN VALUES
 If wait() returns due to a stopped or terminated child process, the
 process ID of the child is returned to the calling process.  Otherwise, a
 value of -1 is returned and errno is set to indicate the error.
 
 If wait3(), wait4(), or waitpid() returns due to a stopped or terminated
 child process, the process ID of the child is returned to the calling
 process.  If there are no children not previously awaited, -1 is returned
 with errno set to [ECHILD].  Otherwise, if WNOHANG is specified and there
 are no stopped or exited children, 0 is returned.  If an error is
 detected or a caught signal aborts the call, a value of -1 is returned
 and errno is set to indicate the error.
 
 ERRORS
 The wait() system call will fail and return immediately if:
 
 [ECHILD]           The calling process has no existing unwaited-for child
 processes.
 
 [EFAULT]           The status or rusage argument points to an illegal
 address (may not be detected before the exit of a
 child process).
 
 [EINVAL]           Invalid or undefined flags are passed in the options
 argument.
 
 The wait3() and waitpid() calls will fail and return immediately if:
 
 [ECHILD]           The process specified by pid does not exist or is not
 a child of the calling process, or the process group
 specified by pid does not exist or does not have any
 member process that is a child of the calling process.
 
 The waitpid() call will fail and return immediately if:
 
 [EINVAL]           The options argument is not valid.
 
 Any of these calls will fail and return immediately if:
 
 [EINTR]            The call is interrupted by a caught signal or the sig-
 nal does not have the SA_RESTART flag set.
 
 STANDARDS
 The wait() and waitpid() functions are defined by POSIX; wait3() and
 wait4() are not specified by POSIX.  The WCOREDUMP() macro and the abil-
 ity to restart a pending wait() call are extensions to the POSIX inter-
 face.
 
 LEGACY SYNOPSIS
 #include <sys/types.h>
 #include <sys/wait.h>
 
 The include file <sys/types.h> is necessary.
 
 SEE ALSO
 sigaction(2), exit(3), compat(5)
 
 HISTORY
 A wait() function call appeared in Version 6 AT&T UNIX.
 
 如果在调用 waitpid()时子进程已经结束,则 waitpid()会立即
 返回子进程结束状态值。 子进程的结束状态值会由参数 status 返回,
 而子进程的进程识别码也会一起返回。如果不在意结束状态值,则
 参数 status 可以设成 NULL。参数 pid 为欲等待的子进程识别码,
 其他数值意义如下:
 pid<-1 等待进程组识别码为 pid 绝对值的任何子进程。
 pid=-1 等待任何子进程,相当于 wait()。
 pid=0 等待进程组识别码与目前进程相同的任何子进程。
 pid>0 等待任何子进程识别码为 pid 的子进程。
 参数options提供了一些额外的选项来控制waitpid，参数 option 可以为 0 或可以用"|"运算符把它们连接起来使用，比如：
 ret=waitpid(-1,NULL,WNOHANG | WUNTRACED);
 如果我们不想使用它们，也可以把options设为0，如：
 ret=waitpid(-1,NULL,0);
 WNOHANG 若pid指定的子进程没有结束，则waitpid()函数返回0，不予以等待。若结束，则返回该子进程的ID。
 WUNTRACED 若子进程进入暂停状态，则马上返回，但子进程的结束状态不予以理会。WIFSTOPPED(status)宏确定返回值是否对应与一个暂停子进程。
 子进程的结束状态返回后存于 status,底下有几个宏可判别结束情况:
 WIFEXITED(status)如果若为正常结束子进程返回的状态，则为真；对于这种情况可执行WEXITSTATUS(status)，取子进程传给exit或_eixt的低8位。
 WEXITSTATUS(status)取得子进程 exit()返回的结束代码,一般会先用 WIFEXITED 来判断是否正常结束才能使用此宏。
 WIFSIGNALED(status)若为异常结束子进程返回的状态，则为真；对于这种情况可执行WTERMSIG(status)，取使子进程结束的信号编号。
 WTERMSIG(status) 取得子进程因信号而中止的信号代码,一般会先用 WIFSIGNALED 来判断后才使用此宏。
 WIFSTOPPED(status) 若为当前暂停子进程返回的状态，则为真；对于这种情况可执行WSTOPSIG(status)，取使子进程暂停的信号编号。
 WSTOPSIG(status) 取得引发子进程暂停的信号代码,一般会先用 WIFSTOPPED 来判断后才使用此宏。
 如果执行成功则返回子进程识别码(PID) ,如果有错误发生则返回
 返回值-1。失败原因存于 errno 中。
 */

