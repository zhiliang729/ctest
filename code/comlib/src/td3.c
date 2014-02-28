#include <comlib.h>
void main()
{
	char c;
	int size = 1, i;
	i = ReadFileTime(0, &c, &size, 10);
	if (i == 0) printf("%d\n", c); 
	else if (i == 1) printf("Timeout\n");
	else printf("Error\n");
}
