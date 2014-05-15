#include <unistd.h>
#include <stdio.h>
void main(int argc, char **argv)
{
	if (argc != 2) return;
	fprintf(stderr, "delete dir [%s] respcode=[%d]\n", argv[1], rmdir(argv[1]));
}

