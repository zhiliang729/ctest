#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(int argc, char **argv)
{
	struct hostent *host;
	if (argc != 2) return 1;
	if ((host = gethostbyname(argv[1])) == NULL) 
	{
		herror("gethostbyname");	
		return 2;
	}
	printf("Host name:%s\n", host->h_name);
	printf("Host IP :%s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
	return 0;
}
