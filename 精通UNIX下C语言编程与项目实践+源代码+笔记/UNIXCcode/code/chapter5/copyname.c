#include <stdio.h>
void main()
{
	FILE *fpr, *fpw;
	int c = 0, f = 0;
	/* ���´�Դ�ļ� */
	if ((fpr = fopen("/etc/passwd", "r")) == NULL)
	{
		printf("open file /etc/passwd failed.\n");
		return;
	}
	/* ���´�Ŀ���ļ� */
	if ((fpw = fopen("./copyname.txt", "w")) == NULL)
	{
		printf("open file ./copyname.txt failed.\n");
		fclose(fpr);
		return;
	}
	while ((c = getc(fpr)) != EOF)
	{
		/* �ַ��Ѿ���ȡ����c�� */
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
