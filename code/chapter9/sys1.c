#include <unistd.h>
#include <stdio.h>
void main()
{
	char cmd[] = {"/bin/ls -l /u"};
	system(cmd);
}

