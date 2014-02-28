#include <fcntl.h>
void main()
{
	int cmd;
	printf("%d\n", fcntl(1, F_GETFD));	
	printf("%d\n", fcntl(1, F_SETFD));	
	printf("%d\n", fcntl(2, F_GETFD));	
	printf("%d\n", fcntl(2, F_SETFD));	

}

