#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(int argc, char **argv)
{
	struct servent *serv;
	if (argc != 2) return 1;
	if ((serv = getservbyname(argv[1], "tcp")) == NULL) 
	{
		herror("getservbyname");	
		return 2;
	}
	printf("Serv name:%s\n", serv->s_name);
	printf("Serv Port :%d\n", ntohs(serv->s_port));
	return 0;
}
