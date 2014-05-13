#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
void main(int argc, char *argv[])
{
	FILE *fp;
	char buf[255];
	while(1)
	{
		if ((fp = fopen("myfifo", "r")) == NULL) return;	/* 打开管道 */
		fgets(buf, sizeof(buf), fp);			/* 读取管道 */
		printf("gets:[%s]", buf);			
		if (strncmp(buf, "quit", 4) == 0 || strncmp(buf, "exit", 4) == 0) break;
		fclose(fp);	/* 关闭管道 */
	}
}
