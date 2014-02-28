#include <sys/types.h>
#include <sys/stat.h>

int GetFile1(char *pHost)
{
	struct stat info;
	char szFmt[] = "%s|FILE|config|%d|400|%d|%s\n";
	if (lstat("./config.ini", &info) != 0)
	{
		printf(szFmt, pHost, info.st_size, 1, "文件不存在");
	}
	else if (info.st_size <= 400) 
	{
		printf(szFmt, pHost, info.st_size, 0, "文件正常");
	}
	else 
	{
		printf(szFmt, pHost, info.st_size, 2, "文件大小超出范围");
	}
	return 0;
}

