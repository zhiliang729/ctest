//
//  main.c
//  signal
//
//  Created by zhangliang on 14-4-15.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

/*
    信号是传送给进程的事件通知，它可以完成进程间异步事件的通信，比如用户按“Ctrl + c” 组合键，unix内核将产生程序终止号
 SIGINT, 并通知进程组终止进程。unix为进城定义了一组事件，并分别将每个事件定义为“SIG”开头的信号宏，比如SIGALRM表示
 定时器信号等。
    1.信号产生
        导致信号产生的原因总体来说有三种可能：
            1.程序错误-----当硬件出现异常、除数为0或者软件非法访问等情况时产生。
            2.外部事件-----当定时器到达、用户按键中断或者进城调用abort等信号发送函数时产生。
            3.显式请求-----当进程调用kill、raise等信号发送函数或者用户执行shell命令kill传递信号时产生。
        每个进程都有一个域用来存储信号的接收情况，域中每个位bit对应一个专门信号。进程在接收信号时设置之，在处理信号时清除之。故进程可以并行接受不同的信号，但如无特殊处理机制（如信号阻塞等），不能记载统一信号在处理前的接收次数
    2.信号处理
        当进程收到信号时有如下三种处理方式。
            1.系统默认-----进程对信号的一般处理方式，由unix内核完成。系统对不同的信号采用不同的默认处理方式。
                方式                                      描述
                终止                                      进程退出
                忽略                                      丢弃信号
                core-dump                                 进程退出，并将进程在内存的数据和存储器状态以某种
                                                            特殊的格式转存到文件系统“core”文件中。本类信号在程序异常时发生，产生core文件供程序员参考
                挂起                                      进程进入睡眠态
                继续                                      唤醒睡眠中的进程。“挂起”和“继续”处理方式主要应用于
                                                        程序调试过程中，如进程单步调试等
            2.忽略信号-----信号接收后，立即丢弃。注意信号SIGSTOP和SIGKILL不能忽略，否则将会出现超级用户无法停止的进程，另外建议对某些信号不能忽略，比如硬件异常信号等。
            3.捕获信号-----进程接收信号，并且调用自定义的代码响应之。比如守护进程生成函数InitServer就调用ClearChild响应信号SIGCHLD。
    3.信号分类
            1.软硬件异常类信号------一般在程序执行了某些严重不合法的动作时发出。
                SIGABRT：   调用abort函数生成的信号。函数abort退出进程，与函数exit类似。
                SIGBUS/SIGSEGV： 硬件错误-非法地址访问。本信号在总线错误时发出。
                SIGEMT：    硬件错误  本信号在仿真器自陷时发出，一般由于操作系统不能正确模拟软件指令而产生。
                SIGFPE：      浮点计算错误。本信号在浮点运算异常时发出，如浮点溢出、除数为0等
                SIGILL：     CPU检测到某进程执行了非法硬件指令时发出。如程序本身出现代码错误、试图执行数据段或堆栈溢出等。
                SIGPWR：     本信号在缺少电力时发出。
                SIGSEGV:    "段违例"信号，在进程访问无效或无权限内存地址时发出。指示进程进行了无效的内存访问
                SIGSYS：     无效的系统调用。比如指示系统调用的参数类型错误等。
                SIGTRAP：    该信号由断点指令或其他trap陷阱指令产生，主要供调试程序时应用。
                SIGXCPU：    进程执行时间超过了分配给该进程的CPU时间，系统产生该信号并发送给该进程。
                SIGXFSZ：    本信号在进程扩大文件过程中超过文件最大长度的限制时发出。
            2.终止进程类信号-----本类信号能够终止进程的运行，部分信号生成core文件。
                SIGHUP：    用户在登录unix时，系统将为其分配终端。本信号在用户退出unix登录时发送。终端连接正常或非正常断开时发出，该终端上所有的进程无论是前台的还是后台的都将受到信号。信号SIGHUP的默认操作方式是终止进程，因此在终端断开连接后，系统将终止该终端上德所有前后台进程。这就是后台执行的进程在用户退出登陆后会消失的原因。但在shell中执行的守护进程不受终端控制。
                SIGINT：     用户键入中断命令一般是按下＜Ctrl+C 或者delete键＞组合键时，用户终端向正在运行中的由该终端启动的程序发出此信号。用于通知前台进程组终止进程。
                SIGKILL：    无条件终止进程。本信号不能被忽略、处理和阻塞。默认动作为终止进程。它向系统管理员提供了一种可以杀死任何进程的方法。
                SIGQUIT：        当用户键入退出命令通常是按下＜Ctrl+\＞组合键时产生该信号，用户终端向正在运行中的由该终端启动的程序（前台进程组）发出此信号。默认动作为终止进程并产生core文件。
                SIGTERM：        程序结束（terminate）信号，与SIGKILL不同的是，该信号可以被阻塞和处理。通常用来要求程序正常退出。执行Shell命令kill时，缺少产生这个信号。
            3.进程挂起类信号-----本类信号导致进程进入休眠状态，一般应用于程序调试和资源抢占中。
                SIGCONT：        激活处于休眠状态的进程，如果进程不处于休眠态将忽略信号。本信号不能被阻塞，但可以捕获，用于完成进程状态变化时执行特定的工作。比如vi在收到本信号后将刷新屏幕。让一个暂停的进程继续执行。
                SIGSTOP：        停止（stopped）进程的执行。注意它和SIGTERM以及SIGINT的区别：该进程还未结束，只是暂停执行。本信号不能被忽略、处理和阻塞。默认动作为暂停进程。
                SIGTSTP：        交互式挂起进程运行。停止进程的动作，但该信号可以被处理和忽略。按下＜Ctrl+Z＞组合键时发出这个信号。默认动作为暂停进程。
                SIGTTIN：        当后台进程要从用户终端读数据时，该终端中的所有进程会收到SIGTTIN信号。默认动作为暂停其他进程。
                SIGTTOU：        该信号类似于SIGTIN，在后台进程要向终端输出数据时产生。默认动作为暂停其他进程。
            4.定时器类信号------本类信号一般由函数alram和色titimer发出，用于指示定时器定时完毕。
                SIGALRM         本信号在alarm函数定义的定时器到期时发出.SIGALRM：定时器超时，超时的时间由系统调用alarm设置。
                SIGPROF         类似于SIGVTALRM，它在该进程占用的CPU时间还包括执行系统调用的时间到期时发出。
                SIGVTALRM：      虚拟时钟超时时产生该信号。类似于SIGALRM，但是它只计算该进程占用的CPU时间。比如函数setitimer设置的定时器到期时会产生此信号。
            5.其他信号-------包括预留信号、I/O信号等。
                SIGCHLD：        子程序结束时，父进程会收到这个信号。如果父进程没有处理这个信号，也没有等待（wait）子进程，子进程虽然终止，但会在内核进程表中占有表项，这时的子进程称为僵死进程。为了避免这种情况，父进程可以采取忽略本信号、捕捉本信号、wait子进程或者父进程先终止，由进程init自动接管子进程等方式。
                SIGPIPE：        Broken pipe：向一个没有读端的管道写数据。管道错误信号，常实用于进程间通信。比如在管道未打开或已关闭时读写管道，又如向已关闭的socket中写入数据
                SIGIO：          本信号在流设备文件描述符准备就绪，可以开始执行I/O操作时发出
                SIGURG:         本信号在进程出现“紧急”数据或者socket中接收了外带数据时发出。
                SIGUSR1：        预留给用户的自定义信号
                SIGUSR2:         预留给用户的自定义信号
                SIGWINCH:        本信号在终端（伪终端）的窗口大小改变时发出
    4.信号操作
        函数signal更改信号的默认处理方式，函数kill或raise可以向进程发送信号。//在实践应用中最常见的信号处理有：1.忽略SIGINT 等进程终止类信号，屏蔽用户终止进程；2.忽略或捕获子进程结束信号SIGCHLD，释放进程表项，预防僵死进程；3.捕获定时器信号，完成进程定时或并发操作；4.捕获自定义信号，完成进程特定操作。
        1.信号的忽略与捕获
            函数signal设置对信号的操作动作。它的原型如下：
                void	(*signal(int, void (*)(int)))(int);
            这是一个复杂的函数原型。分开写就是
                typddef void (* func)(int);
                func signal(int sig, func f);
            首先，定义函数指针类型func, 它是一个指向参数为整型（int）且无返回值的函数指针。然后定义函数signal为具有int类型和func类型参数、返回值为func类型的函数。
                函数signal更改进程对信号的处理方式（信号SIGKILL和SIGSTOP除外）.整型sig指定了要处理的信号，参数f指明了进程接收到信号sig后的处理动作，目前可取动作如下：
                    取值                 描述
                    SIG_DFL             恢复信号默认处理机制。
                    SIG_IGN             忽略信号处理
                    函数地址              调用信号捕获函数执行处理
            函数调用成功时，返回信号捕获函数的地址，即参数f的取值，否则返回SIG_ERR.参数sig取值非法是导致调用失败的常见原因：或者sig不是一个信号值，或者妄图更改信号SIGKILL或SIGSTOP的默认处理方式。//虽然参数f的类型是指向函数的指针，但它的取值SIG_IGN和SIG_DFL居然都是整型！其实指针在内存中表示为4字节字符，从本质上讲它可以用一个整型描述，而一般unix定义常数SIG_DFL值为0，常数SIG_IGN为1，不会与普通的函数地址发生冲突，这样就完全可以使用一个4字节字符来表示，它即表示整型又可描述函数地址。比如空指针null的取值就是整型0
        2.信号的显式发送 unix应用程序可以向进程发送任意信号，这些信号发送函数的原型如下：
            #include <sys/types.h>
            #include <signal.h>
 
            int kill(pid_t pid, int signo);
            int raise(int signo);
 
            函数kill发送信号signo到参数pid决定的特定进程中，其中pid的取值含义如下：
                取值                                  含义
                > 0                                 发送信号signo到进程pid中
                0                                   发送信号到signo到与调用进程同组进程中
                -1                                  发送信号signo到实际用户id等于调用进程的有效用户id的进程中
                <-1                                 发送信号signo到进程组id等于pid绝对值的进程中
 
                函数kill调用成功时返回0，否则返回-1；
                函数raise向调用进程自身发送信号signo，成功时返回0，否则返回-1.
 */
int usr1 = 0, usr2 = 0;
void func(int);
void main1(int argc, const char * argv[])
{
    signal(SIGUSR1, func);
    signal(SIGUSR2, func);
    
    for (; ;) {
        sleep(1);
    }
}

void func(int sig)
{
    if (sig == SIGUSR1) {
        usr1++;
    }
    
    if (sig == SIGUSR2) {
        usr2++;
    }
    
    fprintf(stderr, "SIGUSR1[%d]; SIGUSR2[%d]\n", usr1, usr2    );
    signal(SIGUSR1, func);
    signal(SIGUSR2, func);
}

#pragma mark - 发送和捕获SIGTERM 终止信号的例子
/*父进程创建一个子进程，并且向其发送SIGTERM信号：子进程捕获信号SIGTERM 后退出。*/
void childfunc(int sig)/*子进程捕获信号函数*/
{
    fprintf(stderr, "Get Sig\n");
    exit(6);//收到信号后自动退出
}

void main2(int argc, const char * argv[])
{
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "\npid = [%d]\n", pid);
        exit(1);
    }else if(pid == 0){/*子进程*/
        fprintf(stderr, "\n子进程pid = [%d]\n", pid);
        signal(SIGTERM, childfunc);/*捕获信号SIGTERM*/
        
//        for (;;)sleep(1);
        sleep(20);
        fprintf(stderr, "\nwill exit self !\n");
        exit(0);//20秒内没收到信号，则自动退出
    }
    
    fprintf(stderr, "Parent [%d] fork child pid = [%d]\n", getpid(), pid);
    
    sleep(10);
    kill(pid, SIGTERM);/*10秒后向子进程发送SIGTERM信号  可以在活动监视器中退出进程即为发送SIGTERM信号*/
    wait(&status);/*等待子进程结束*/
    fprintf(stderr, "kill child pid = [%d], exit status [%d]\n", pid, status>>8);
}


int main(int argc, const char * argv[])
{
//    //信号忽略实例   在100庙内，屏蔽SIGINT信号，即按下delete或ctrl+c键无效
//    signal(SIGINT, SIG_IGN);//屏蔽信号SIGINT
//    sleep(100);
    
    
    //信号捕获实例
//    main1(argc, argv);
    
    //发送和捕获SIGTERM 终止信号
    main2(argc, argv);
    return 0;
}

