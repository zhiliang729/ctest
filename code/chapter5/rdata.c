#include <stdio.h>
void main()
{
	FILE *fp;
	int narray[10];
	int all = 6, i;
	if ((fp = fopen("array.dat", "rb")) == NULL)
	{
		printf("open file array failed.\n");
		return;
	}
	/* ��ȡ�ļ��д洢�ĵ�һ����������ȡ�洢�����Ԫ���� */
	fread(&all, sizeof(int), 1, fp);
	/* ��ȡ�ļ��д洢������Ԫ�� */
	fread(narray, sizeof(int), all, fp);
	printf("all=%d\n", all); 
	for (i=0; i<all; i++)
	{
		printf("[%d]=%d    ", i, narray[i]); 
	}		
	printf("\n"); 
	fclose(fp);
}
