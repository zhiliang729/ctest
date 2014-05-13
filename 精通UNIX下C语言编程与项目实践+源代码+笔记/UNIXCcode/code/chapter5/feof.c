#include<stdio.h>
void main()
{
	FILE *fp;
	char buf[1024];
	if ((fp = fopen("/etc/passwd", "r")) == NULL)
	{
		printf("open file passwd failed.\n");
		return;
	}
	while (!feof(fp))
	{
		fgets(buf, sizeof(buf), fp);
		if (feof(fp)) break;
		fputs(buf, stderr);
	}
	fclose(fp);
}
