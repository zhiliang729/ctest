#include <stdio.h>
#include<stdlib.h>
void main(int argc, char **argv)
{
	int i;
	for (i=1; i<argc; i++)
	{
		fprintf(stderr, "%s=%s\n", argv[i], getenv(argv[i]));
	}
}
