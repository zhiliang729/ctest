#include <stdio.h>
void main()
{
	FILE *fp;
	char szBuf[100];
	/* 将屏幕标准输出的内容重定向到文件"/tmp/1" */
	if ((fp = freopen("/tmp/1", "w", stderr)) == NULL)
	{ 
		printf("stderr -- /tmp/1 failed.\n");
		return;
	}
	/* stderr已经输出重定向，所有错误输出内容都将写入文件“/tmp/1”中 */
	fputs("I Like Unix.\n", stderr);
	/* 关闭文件 */
	fclose(fp);
	/* 将标准输入由键盘输入更改为从文件"/tmp/2"中读入 */
	if ((fp = freopen("/tmp/1", "r", stdin)) == NULL)
	{ 
		printf("stdin -- /tmp/1 failed.\n");
		return;
	}
	memset(szBuf, 0, sizeof(szBuf));
	/* stdin已经输入重定向，所有容都将写入文件“/tmp/1”中 */
	fgets(szBuf, sizeof(szBuf), stdin);
	printf("szBuf=[%s]", szBuf);
	/* 关闭文件 */
	fclose(fp);
}

