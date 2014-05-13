#include <comlib.h>
//#include  <sys/vfs.h>
struct  statfs {
        short   f_fstyp;        /* File system type */
        long    f_bsize;        /* Block size */
        long    f_frsize;       /* Fragment size (if supported) */
        unsigned long   f_blocks; /* Total number of blocks on file system */
        unsigned long   f_bfree;  /* Total number of free blocks */
        unsigned long   f_files;  /* Total number of file nodes (inodes) */
        unsigned long   f_ffree;  /* Total number of free file nodes */
        char    f_fname[6];     /* Volume name */
        char    f_fpack[6];     /* Pack name */
};
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
