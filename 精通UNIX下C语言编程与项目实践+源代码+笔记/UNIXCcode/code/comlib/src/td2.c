#include <comlib.h>
void main()
{
	char buf[11];
	int size = sizeof(buf);
	ReadFile(0, buf, &size); 
	printf("%d", size); 
}
