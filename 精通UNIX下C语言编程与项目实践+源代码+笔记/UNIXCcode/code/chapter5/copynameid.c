#include <stdio.h>
void main()
{
	FILE *fpr, *fpw;
	char buf[1024], *p1, *p2;
	/* 以下打开源文件 */
	if ((fpr = fopen("/etc/passwd", "r")) == NULL)
	{
		printf("open file /etc/passwd failed.\n");
		return;
	}
	/* 以下打开目标文件 */
	if ((fpw = fopen("./copynameid.txt", "w")) == NULL)
	{
		printf("open file ./copynameid.txt failed.\n");
		fclose(fpr);
		return;
	}
	while (fgets(buf, sizeof(buf), fpr) != NULL)
	{
		/* p1指向第一个":"，p2指向第2个":" */
		if ((p1 = strstr(buf, ":")) == NULL) break;
		if ((p2 = strstr(p1+1, ":")) == NULL) break;
		p1++; p2++;
		/* p1指向第二个域密码字段，p2指向第三个域用户ID字段 */
		/* 以下代码移动字符串内容，将ID字段的内容移动到用户名字段后 */
		while (*p2 != ':')
		{
			*p1 = *p2;
			p1++; p2++;
		}
		*p1 = 0;
		/* 屏幕输出 */
		puts(buf);
		/* 文件输出 */
		fputs(buf, fpw);
		fputs("\n", fpw);
		/* 清除内存 */
		memset(buf, 0, sizeof(buf));
	}
	fclose(fpr);
	fclose(fpw);
}
