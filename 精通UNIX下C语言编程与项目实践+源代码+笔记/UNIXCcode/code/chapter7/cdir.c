#include <unistd.h>
#include <stdio.h>
void main(int argc, char **argv)
{
	if (argc != 2) return;
	fprintf(stderr, "create dir [%s] respcode=[%d]\n", argv[1], mkdir(argv[1] , 0777));
}

