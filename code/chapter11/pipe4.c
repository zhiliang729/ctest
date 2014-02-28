#include <stdio.h>
void main()
{
	FILE *out, *in;
	char buf[255];
	if ((out = popen("grep init", "w")) == NULL)
	{
		fprintf(stderr, "error!\n");
		return;
	}
	if ((in = popen("ps -ef", "r")) == NULL)
	{
		fprintf(stderr, "error!\n");
		return;
	}
	while (fgets(buf, sizeof(buf), in)) 
		fputs(buf, out);
	pclose(out);
	pclose(in);
}
