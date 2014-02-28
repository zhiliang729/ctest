#include <stdio.h>
#include <dirent.h>
void main(int argc, char **argv)
{
	DIR *pdir;
	struct dirent *pent;
	if (argc != 2) return;
	if ((pdir = opendir(argv[1])) == NULL) 
	{
		fprintf(stderr, "open dir failed.\n");
		return;
	}
	while (1)
	{
		pent = readdir(pdir);
		if (pent == NULL) break;
		fprintf(stderr, "%5d %s\n", pent->d_ino, pent->d_name);
	}
	closedir(pdir);
}
