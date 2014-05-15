#include <unistd.h>
#include <stdio.h>
void main()
{
	char buf[255];
	fprintf(stderr, "pwd = [%s]\n", getcwd(buf, sizeof(buf)));
	chdir("../");
	fprintf(stderr, "pwd = [%s]\n", getcwd(buf, sizeof(buf)));	
}

