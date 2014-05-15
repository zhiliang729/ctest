#include <stdio.h>
void main()
{
	FILE *fp;
	/* 数组数据，读者可以更改数组长度和元素内容 */
	int narray[10] = {3131, 45453, 534534, 422, 332, 322, 11, 223, 66,9};
	/* 写入数组元素的个数，请不要操作数组长度*/
	int all = 6;
	if ((fp = fopen("array.dat", "wb")) == NULL)
	{
		printf("open file array failed.\n");
		return;
	}
	fwrite(&all, sizeof(int), 1, fp);
	/* 实际写入1*sizeof(int)=4个字符 */
	fwrite(narray, sizeof(int), all, fp);
	/* 实际写入all*sizeof(int)=24个字符 */
	fclose(fp);
}
