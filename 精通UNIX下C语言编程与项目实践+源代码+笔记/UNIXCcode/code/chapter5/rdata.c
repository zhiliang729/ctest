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
	/* 读取文件中存储的第一个整数，获取存储数组的元素数 */
	fread(&all, sizeof(int), 1, fp);
	/* 读取文件中存储的数组元素 */
	fread(narray, sizeof(int), all, fp);
	printf("all=%d\n", all); 
	for (i=0; i<all; i++)
	{
		printf("[%d]=%d    ", i, narray[i]); 
	}		
	printf("\n"); 
	fclose(fp);
}
