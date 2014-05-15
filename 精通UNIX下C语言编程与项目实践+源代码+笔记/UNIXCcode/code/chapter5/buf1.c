#include <stdio.h>
void main()
{
	printf("  1---1  ");
	/* fflush(stdout); */
	fprintf(stderr, "  2---2  ");
	printf("  3---3\n");
	fprintf(stderr, "  4---4\n");
}
