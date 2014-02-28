
#include <comlib.h>

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

TIMESTRU GetOffsetTime(TIMESTRU timestru, int nOffsetSec)
{
	TIMESTRU timestruAfter;
	time_t ttime;
	struct tm when;
	
	memset(&when, 0, sizeof(when));
	when.tm_year = timestru.nYear - 1900;
	when.tm_mon  = timestru.nMon - 1;
	when.tm_mday = timestru.nDay;
	when.tm_hour = timestru.nHour;
	when.tm_min  = timestru.nMin;
	when.tm_sec  = timestru.nSec;
	
	ttime = mktime(&when) + nOffsetSec;
	
	when = *localtime(&ttime);
	
	timestruAfter.nYear = when.tm_year + 1900;
	timestruAfter.nMon = when.tm_mon + 1;
	timestruAfter.nDay = when.tm_mday;
	timestruAfter.nHour = when.tm_hour;
	timestruAfter.nMin = when.tm_min;
	timestruAfter.nSec = when.tm_sec;
	
	return timestruAfter;
}

/* End of this file */
