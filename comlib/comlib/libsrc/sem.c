//
//  sem.c
//  comlib
//
//  Created by zhangliang on 14-4-23.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*信号量阻塞操作  信号量的阻塞P操作（信号值减少）、释放V操作(信号值增加)和Z（信号值判断）操作功能  本函数通过封装系统调用semop来操作标识号为nSid，序号为nIndex的信号量，函数Semop将信号量的取值增加nVal。当nVal>0时，函数等同于V操作；当nVal<0时，函数的效果是信号量取值减小，等同于信号量P操作；当nVal=0时，函数等同于Z操作。简化了结构sembuf的设置过程，并增加了错误信息的判断  函数成功时返回0，信号中断时返回1，否则返回其他值*/
int Semop(int nSid, int nIndex , int nVal)
{
    struct sembuf sb;
}
int SemopNowait(int nSid, int nIndex , int nVal);
int SemopTime(int nSid, int nIndex, int nVal, int nTimeOut);
