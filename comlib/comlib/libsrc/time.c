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

TIMESTRU GetTimeFromTM(struct tm when)
{
    TIMESTRU timestruAfter;
    timestruAfter.nYear = when.tm_year + 1900;
	timestruAfter.nMon = when.tm_mon + 1;
	timestruAfter.nDay = when.tm_mday;
	timestruAfter.nHour = when.tm_hour;
	timestruAfter.nMin = when.tm_min;
	timestruAfter.nSec = when.tm_sec;
    return timestruAfter;
}

TIMESTRU GetOffsetTime(TIMESTRU timestru, int nOffsetSec)
{
	TIMESTRU timestruAfter;
	time_t ttime;
	struct tm when;
	
	memset(&when, 0, sizeof(struct tm));
	when.tm_year = timestru.nYear - 1900;
	when.tm_mon  = timestru.nMon - 1;
	when.tm_mday = timestru.nDay;
	when.tm_hour = timestru.nHour;
	when.tm_min  = timestru.nMin;
	when.tm_sec  = timestru.nSec;
	
	ttime = mktime(&when) + nOffsetSec;/*将TIMESTRU先转换为本地时间，再将本地时间转换为系统时间，再加上时间差，得到增加或减少nOffsetSec秒后的时间对应的系统时间*/
	
	when = *localtime(&ttime);/*将系统时间转为本地时间*/
	
//	timestruAfter.nYear = when.tm_year + 1900;
//	timestruAfter.nMon = when.tm_mon + 1;
//	timestruAfter.nDay = when.tm_mday;
//	timestruAfter.nHour = when.tm_hour;
//	timestruAfter.nMin = when.tm_min;
//	timestruAfter.nSec = when.tm_sec;
    
    timestruAfter = GetTimeFromTM(when);
	
	return timestruAfter;
}
