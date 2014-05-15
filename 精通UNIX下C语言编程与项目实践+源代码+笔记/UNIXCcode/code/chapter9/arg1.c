#include <stdio.h>
void main(int argc, char * argv[])
{
	int i;
	fprintf(stderr, "argc=[%d]\n", argc);
	for (i=0; i<argc; i++)
	{
		fprintf(stderr, "argv[%d]=[%s]\n", i, argv[i]);
	}
}
