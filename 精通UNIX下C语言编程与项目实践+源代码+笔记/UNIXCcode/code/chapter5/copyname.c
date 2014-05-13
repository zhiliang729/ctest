#include <stdio.h>
void main()
{
	FILE *fpr, *fpw;
	int c = 0, f = 0;
	/* 以下打开源文件 */
	if ((fpr = fopen("/etc/passwd", "r")) == NULL)
	{
		printf("open file /etc/passwd failed.\n");
		return;
	}
	/* 以下打开目标文件 */
	if ((fpw = fopen("./copyname.txt", "w")) == NULL)
	{
		printf("open file ./copyname.txt failed.\n");
		fclose(fpr);
		return;
	}
	while ((c = getc(fpr)) != EOF)
	{
		/* 字符已经读取到了c中 */
		if (f == 0) 
		{
			if (c != ':') putchar(putc(c, fpw));
			else f = 1;
		}
		else if (c == '\n') 
		{
			f = 0;
			putchar(putc(c, fpw));
		}
	}
	fclose(fpr);
	fclose(fpw);
}
