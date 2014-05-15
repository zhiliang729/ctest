#include <stdio.h>
void main()
{
	FILE *fpr, *fpw;
	char buf[1024], *p1, *p2;
	/* ���´�Դ�ļ� */
	if ((fpr = fopen("/etc/passwd", "r")) == NULL)
	{
		printf("open file /etc/passwd failed.\n");
		return;
	}
	/* ���´�Ŀ���ļ� */
	if ((fpw = fopen("./copynameid.txt", "w")) == NULL)
	{
		printf("open file ./copynameid.txt failed.\n");
		fclose(fpr);
		return;
	}
	while (fgets(buf, sizeof(buf), fpr) != NULL)
	{
		/* p1ָ���һ��":"��p2ָ���2��":" */
		if ((p1 = strstr(buf, ":")) == NULL) break;
		if ((p2 = strstr(p1+1, ":")) == NULL) break;
		p1++; p2++;
		/* p1ָ��ڶ����������ֶΣ�p2ָ����������û�ID�ֶ� */
		/* ���´����ƶ��ַ������ݣ���ID�ֶε������ƶ����û����ֶκ� */
		while (*p2 != ':')
		{
			*p1 = *p2;
			p1++; p2++;
		}
		*p1 = 0;
		/* ��Ļ��� */
		puts(buf);
		/* �ļ���� */
		fputs(buf, fpw);
		fputs("\n", fpw);
		/* ����ڴ� */
		memset(buf, 0, sizeof(buf));
	}
	fclose(fpr);
	fclose(fpw);
}
