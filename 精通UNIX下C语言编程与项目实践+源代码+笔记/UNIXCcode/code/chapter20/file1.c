#include <sys/types.h>
#include <sys/stat.h>

int GetFile1(char *pHost)
{
	struct stat info;
	char szFmt[] = "%s|FILE|config|%d|400|%d|%s\n";
	if (lstat("./config.ini", &info) != 0)
	{
		printf(szFmt, pHost, info.st_size, 1, "�ļ�������");
	}
	else if (info.st_size <= 400) 
	{
		printf(szFmt, pHost, info.st_size, 0, "�ļ�����");
	}
	else 
	{
		printf(szFmt, pHost, info.st_size, 2, "�ļ���С������Χ");
	}
	return 0;
}


int main(int argc, char **argv)
{
	GetFile1(argv[1]);
	return 0;
}
	
	

