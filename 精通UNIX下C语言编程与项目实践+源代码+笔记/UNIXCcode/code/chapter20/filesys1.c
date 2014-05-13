#include <comlib.h>
#include  <sys/vfs.h>

int GetFilesysValue(char *pHost, char *pPath)
{
	char szFmt[] = "%s|FILE|%s|%d|%d|%d|%s\n", szRate1[30], szRate2[30];
	struct statfs buf;
	float rused, rinode;
	int nalarm;
	if (statfs(pPath, &buf) == 0)
	{
		rused = (double)buf.f_bfree / buf.f_blocks;
		rinode = (double)buf.f_ffree / buf.f_files;
		sprintf(szRate1, "block : %.2f%% available", rused*100);
		sprintf(szRate2, "inode : %.2f%% available", rinode*100);
		if (rused >= 0.15) nalarm = 0;
		else if (rused >= 0.5) nalarm = 2;
		else nalarm = 1;
		printf(szFmt, pHost, pPath, buf.f_bfree, buf.f_blocks, nalarm, szRate1);
		if (rinode >= 0.15) nalarm = 0;
		else if (rinode >= 0.5) nalarm = 2;
		else nalarm = 1;
		printf(szFmt, pHost, pPath, buf.f_ffree, buf.f_files, nalarm, szRate2);
	}
	return 0;
}


int GetFilesys1(char *pHost)
{
	GetFilesysValue(pHost, "/u");
	GetFilesysValue(pHost, "/");
	return 0; 
}
int main(int argc, char **argv)
{
	GetFilesys1(argv[1]);
	return 0;
}
