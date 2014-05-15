#include <time.h>
main()
{
	time_t start, finish;
	time(&start);
	sleep(6);
	time(&finish);
	printf("start=[%d], finish=[%d], difftime=[%f]\n", start, finish, difftime(finish, start));
}
