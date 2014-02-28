#include <time.h>                                               
#include <stdio.h>                                              
void main()                                                          
{                                                               
	struct tm when;
	time_t now;
	time(&now);                                             
	when = *localtime(&now);
	printf("now=[%d][%04d%02d%02d %02d:%02d:%02d]\n", now, 
		when.tm_year+1900, when.tm_mon+1, when.tm_mday, 
		when.tm_hour, 	when.tm_min, when.tm_sec);
}  

