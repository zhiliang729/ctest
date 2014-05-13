#include <comlib.h>
void main()
{
	TIMESTRU now, yes, tom;
	now = GetTime();
	yes = GetOffsetTime(now, 0 - 24*60*60);
	tom = GetOffsetTime(now, 24*60*60);
	printf("now[%04d%02d%02d], yes[%04d%02d%02d], tom[%04d%02d%02d]\n",
		now.nYear, now.nMon, now.nDay, 
		yes.nYear, yes.nMon, yes.nDay, 
		tom.nYear, tom.nMon, tom.nDay);		
}
