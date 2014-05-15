#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
extern int errno;
void main(int argc, char *argv[])
{
	FILE *fp;
	char buf[255];
	/* 创建管道，如果已存在则跳过 */
	if (mkfifo("myfifo", S_IFIFO|0666) < 0 && errno != EEXIST) return;
	while(1)
	{
		if ((fp = fopen("myfifo", "w")) == NULL) return;
		printf("please input:");
		gets(buf);
		fputs(buf, fp);
		fputs("\n", fp);
		if (strncmp(buf, "quit", 4) == 0 || strncmp(buf, "exit", 4) == 0) break;
		fclose(fp);
	}
}
