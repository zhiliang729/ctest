#include <stdio.h>
void main()
{
	FILE *fp;
	char szBuf[100];
	/* ����Ļ��׼����������ض����ļ�"/tmp/1" */
	if ((fp = freopen("/tmp/1", "w", stderr)) == NULL)
	{ 
		printf("stderr -- /tmp/1 failed.\n");
		return;
	}
	/* stderr�Ѿ�����ض������д���������ݶ���д���ļ���/tmp/1���� */
	fputs("I Like Unix.\n", stderr);
	/* �ر��ļ� */
	fclose(fp);
	/* ����׼�����ɼ����������Ϊ���ļ�"/tmp/2"�ж��� */
	if ((fp = freopen("/tmp/1", "r", stdin)) == NULL)
	{ 
		printf("stdin -- /tmp/1 failed.\n");
		return;
	}
	memset(szBuf, 0, sizeof(szBuf));
	/* stdin�Ѿ������ض��������ݶ���д���ļ���/tmp/1���� */
	fgets(szBuf, sizeof(szBuf), stdin);
	printf("szBuf=[%s]", szBuf);
	/* �ر��ļ� */
	fclose(fp);
}

