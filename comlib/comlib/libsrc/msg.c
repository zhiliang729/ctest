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


