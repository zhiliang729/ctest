#include <unistd.h>
void main()
{       
        char buf[10] = "abcchina";
        printf("%d", write(1, buf, sizeof(buf))); 
}
