#include <stdio.h>
/* д���ļ���rec������ */
void seekwrite(FILE *fp, int narray, int rec)
{
	fseek(fp, sizeof(int)*rec, SEEK_SET);
	fwrite(&narray, sizeof(int), 1, fp);
}
/* д���ļ���rec������ */
void seekread(FILE *fp, int *narray, int rec)
{
	fseek(fp, sizeof(int)*rec, SEEK_SET);
	fread(narray, sizeof(int), 1, fp);
}
void main()
{
	FILE *fp;
	int narray[2] = {100, 200}, narrayr[2];
	if ((fp = fopen("seek.dat", "w+b")) == NULL)
	{
		printf("open file seek.dat failed.\n");
		return;
	}
	seekwrite(fp, narray[0], 1); seekwrite(fp, narray[1], 10);
	seekread(fp, narrayr, 1); seekread(fp, narrayr+1, 10);
	printf("now array[0]=%d, array[1]=%d\n", narrayr[0], narrayr[1]);
	fclose(fp);
}

