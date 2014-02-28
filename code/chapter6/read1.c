#include <unistd.h>
void main()
{       
        char buf[11];
        printf("%d", read(0, buf, sizeof(buf))); 
}
