#include <stdio.h>
#include<sys/types.h>
#include<sys/times.h>
#include<sys/select.h>
void main()
{
	struct timeval timeout;
	fd_set readfds;
	int i;
	char c;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	i = select(1, &readfds, NULL, NULL, &timeout);
	if (i >0)
	{
		read(0, &c, 1);
		printf("%d\n", c);
	}
	else if (i == 0) printf("timeout\n");
	else printf("error\n");
}

