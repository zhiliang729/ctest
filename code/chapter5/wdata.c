#include <stdio.h>
void main()
{
	FILE *fp;
	/* �������ݣ����߿��Ը������鳤�Ⱥ�Ԫ������ */
	int narray[10] = {3131, 45453, 534534, 422, 332, 322, 11, 223, 66,9};
	/* д������Ԫ�صĸ������벻Ҫ�������鳤��*/
	int all = 6;
	if ((fp = fopen("array.dat", "wb")) == NULL)
	{
		printf("open file array failed.\n");
		return;
	}
	fwrite(&all, sizeof(int), 1, fp);
	/* ʵ��д��1*sizeof(int)=4���ַ� */
	fwrite(narray, sizeof(int), all, fp);
	/* ʵ��д��all*sizeof(int)=24���ַ� */
	fclose(fp);
}
