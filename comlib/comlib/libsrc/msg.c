//
//  msg.c
//  comlib
//
//  Created by zhangliang on 14-4-22.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

#define SIZEMAXMSGSIZE 2048
typedef struct structMSGSTRU
{
    long nType;
    char cText[SIZEMAXMSGSIZE];
    
} MSGSTRU;
typedef MSGSTRU * PMSGSTRU;


/*阻塞方式消息接收*/
//本函数从标识号为nPid的消息队列中接受消息，其中指针pText指向了接收消息数据的缓冲区，该缓冲区最大长度通过参数pSize传入，参数pType制定了接收消息的类型。与系统调用msgrcv相比，本函数内置了消息的定义、缓冲区准备、消息接收和结果判断等过程。函数成功时返回0，并将接收到消息数据的长度和消息的类型分别通过参数pSize和参数pType传回，否则函数返回非0值。   当系统调用msgrcv返回-1且errno被置为EINTR时，意味着在消息接收过程中产生了信号中断，此时应该重新接收消息，否则系统发生异常，函数返回非0值。参数pType 传入预读取的消息类型值或范围，返回时嫉妒去的消息类型值，因此该参数的传入值与回传值不一定相等，当且仅当pType传入正整数时，回传值不变。例如消息队列当前首位消息类型为5时，如果pType传入值为0，则传出值为5.
int ReadMsg(int nPid, void * pText, int * pSize, int * pType)
{
    //申请消息缓冲
    MSGSTRU sMsg;
    int n;
    ASSERT(pText != NULL && pSize != NULL && pType != NULL && nPid > 0 && *pSize > 0);//容错判断
    //准备接收消息缓冲区
    memset(sMsg, 0, sizeof(MSGSTRU));//清空缓冲
    memset(pText, 0, *pSize);
    //消息接收
    while ((n = msgrcv(nPid, &sMsg, *pSize, *pType, !IPC_NOWAIT)) < 0) {
        //消息接收结果判断
        if (errno == EINTR) {//信号中断，继续接收
            continue;
        }
        ASSERT(0);//异常发生，函数退出
    }
    
    //消息回传
    memcpy(pText, sMsg.cText, n);//回传消息数据
    *pSize = n;//回传消息长度
    *pText = sMsg.nType;//回传消息类型
    return 0;
}

//阻塞方式消息发送
//本函数向标识号为nPid的消息队列发送消息，其中待发送的消息数据存储在内存缓冲pText处，消息数据长度为nSize字节，消息类型为nType。与系统调用msgsnd相比，本函数内置了消息的定义、组包、发送和结果判断等过程。函数成功时返回0，否则返回非0值。  当系统调用msgsnd返回-1且errno被置为EINTR时，意味着在消息发送过程中产生了信号中断，此时应该重新发送消息，否则系统发生异常，函数返回非0值。
int WriteMsg(int nPid, void * pText, int nSize, int nType)
{
    //申请消息缓冲
    MSGSTRU sMsg;
    int n;
    ASSERT(pText != NULL && nPid > 0 && nSize > 0 && nSize <= SIZEMAXMSGSIZE && nType > 0);//容错判断
    //消息组包
    memset(&sMsg, 0, sizeof(sMsg));//清空缓冲区
    sMsg.nType = nType;//消息类型
    memcpy(sMsg.cText, pText, nSize);//拷贝消息数据到ctext中。
    //消息发送
    while ((n = msgsnd(nPid, &sMsg, nSize, !IPC_NOWAIT)) < 0) {//阻塞方式发送
        //消息发送结果判断
        if (errno == EINTR) {
            continue;//信号中断，继续发送
        }
        ASSERT(0);//异常发生，退出函数
    }
    return 0;
}

/*定时方式消息接收  它从标识号为nPid的消息队列中接收消息，接收的消息数据写入指针pText指向的内存缓冲区，参数pSize传入缓冲区最大长度，并回传实际接收到消息数据的长度，参数pText传入待接收消息的类型，并回传实际接收到消息的类型， 消息队列阻塞时，进程将被挂起，直到阻塞条件消失或者阻塞nTimeout秒后函数返回。*/
int ReadMsgExt(int nPid, void * pText, int * pSize, int * pType, int nTimeout)
{
    MSGSTRU sMsg;
    int n;
    ASSERT(pText != NULL && pSize != NULL && pType != NULL && nPid > 0 && *pSize > 0);
    memset(pText, 0, *pSize);
    memset(&sMsg, 0, sizeof(MSGSTRU));
    nFlag = 0;
    setjmp(env);
    if (nFlag != 0) {
        return 3;//超时，返回3
    }
    alarm(nTimeout);
    signal(SIGALRM, OnTimeout);
    while ((n = msgrcv(nPid, &sMsg, *pSize, *pType, !IPC_NOWAIT)) < 0) {
        if (errno == EINTR && nFlag == 0) {
            continue;//中断，继续
        }else if (errno == EINTR && nFlag != 0){
            return 3;//超时，返回3
        }
        ASSERT(0);
    }
    
    signal(SIGALRM, SIG_IGN);
    alarm(0);
    memcpy(pText, sMsg.cText, n);
    *pSize = n;
    if (*pType != 0) {
        *pType = sMsg.nType;
    }
    return 0;
}

//定时方式消息发送   向标识号为nPid的消息队列发送消息，其中待发送的消息数据存储在内存缓冲pText处，消息数据长度为nSize字节，消息类型为nType。当消息队列阻塞时，进程将被挂起，直到阻塞条件消失或者阻塞nTimeout秒后函数返回。
static int nFlag = 0;
jmp_buf env;

static void OnTimeout(int nSignal)
{
	signal(nSignal, SIG_IGN);
	nFlag = 1;
    longjmp(env, 1);
	return;
}

int WriteMsgExt(int nPid, void * pText, int nSize, int nType, int nTimeout)
{
    MSGSTRU sMsg;
    int n;
    ASSERT(pText != NULL && nPid > 0 && nSize > 0 && nSize <= SIZEMAXMSGSIZE && nType > 0);//容错判断
    memset(&sMsg, 0, sizeof(sMsg));
    sMsg.nType = nType;
    memcpy(sMsg.cText, pText, nSize);
    nFlag = 0;
    setjmp(env);
    if (nFlag != 0) {
        return 3;//超时，返回3
    }
    alarm(nTimeout);//定时
    signal(SIGALRM, OnTimeout);
    while ((n = msgsnd(nPid, &sMsg, nSize, !IPC_NOWAIT)) < 0) {
        if (errno == EINTR && nFlag == 0) {//中断，继续
            continue;
        }else if(errno == EINTR && nFlag != 0){
            return 3;//超时，返回3
        }
        ASSERT(0);
    }
    signal(SIGALRM, SIG_IGN);
    alarm(0);//取消定时
    ASSERT(n != nSize);
    return 0;
}

//判断消息队列中是否存在消息   消息队列的接收是一锤子买卖，一旦调用msgrcv成功，消息就会从队列中消失，函数只判断消息队列中是否存在符合要求的消息，而不实际接收消息数据。 nPid制定了消息队列的标识号，参数pType制定了欲读取消息的类型值或范围。如果队列中存在满足条件的消息，函数返回0并回传该消息的类型，如果不存在则返回-1.      本技巧的关键是以非阻塞和非错误方式接受长度为0的消息，此时一旦队列中存在满足条件的消息，必定造成缓冲区溢出，内核自动阶段0字节信息数据返回，从而造成队列被访问而消息又未删除的现象。
int CheckMsgData(int nPid, int * pType)
{
    MSGSTRU sMsg;
    int n;
    ASSERT(pType != NULL && nPid > 0);
    //消息接收
    while ((n = msgrcv(nPid, &sMsg, 0, *pType, IPC_NOWAIT | MSG_NOERROR)) < 0) {
        //消息接收结果判断
        if (errno == EINTR) {
            continue;//信号中断
        }
        if (errno == EAGAIN) {
            return 1;//信号中断
        }
        ASSERT(0);//异常发生，返回-1
    }
    *pType = sMsg.nType;//回传消息类型
    return 0;
}

