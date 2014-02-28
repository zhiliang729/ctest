//
//  time.c
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

TIMESTRU GetTime()
{
    TIMESTRU timestru;
    
    time_t time1;
    struct tm * when;
    
    time(&time1);
    when = localtime(&time1);
    
    timestru.nYear = when->tm_year + 1900;
    timestru.nMon = when->tm_mon + 1;
    timestru.nDay = when->tm_mday;
    
    timestru.nHour = when->tm_hour;
    timestru.nMin = when->tm_min;
    timestru.nSec = when->tm_sec;
    
    return timestru;
}