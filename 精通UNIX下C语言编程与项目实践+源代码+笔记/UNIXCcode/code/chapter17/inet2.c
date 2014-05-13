#include <stdio.h>
int main()
{
	char buf[1024];
	printf("Welcome!\n");
	fflush(stdout);
	while (1)
	{
		fgets(buf, sizeof(buf), stdin);
		printf("Input:%s", buf);
		fflush(stdout);
		if (strncmp(buf, "exit", 4) == 0) break;
	}
}
