#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
int GetFileType(mode_t st_mode, char *resp)
{
	if (resp == NULL) return 0;
	if (S_ISDIR(st_mode)) resp[0] = 'd';	/* 测试是否为目录文件 */
	else if (S_ISCHR(st_mode))  resp[0]='c';  	/* 测试是否为字符文件 */
	else if (S_ISBLK(st_mode))  resp[0]='b';  	/* 测试是否为块文件 */
	else if (S_ISREG(st_mode))  resp[0]='-';  	/* 测试是否为普通文件 */
	else if (S_ISFIFO(st_mode)) resp[0]='p'; 	/* 测试是否为管道文件 */
	else if (S_ISLNK(st_mode))  resp[0]='l'; 	  	/* 测试是否为符合链接 */
	else resp[0] = ' ';
	return 1;
}

int GetFileMode(mode_t st_mode, char *resp)
{
	if (resp == NULL) return 0;
	memset(resp, '-', 9);
	if (st_mode & S_IRUSR) resp[0] = 'r';
	if (st_mode & S_IWUSR) resp[1] = 'w';
	if (st_mode & S_IXUSR) resp[2] = 'x';
	if (st_mode & S_IRGRP) resp[3] = 'r';
	if (st_mode & S_IWGRP) resp[4] = 'w';
	if (st_mode & S_IXGRP) resp[5] = 'x';
	if (st_mode & S_IROTH) resp[6] = 'r';
	if (st_mode & S_IWOTH) resp[7] = 'w';
	if (st_mode & S_IXOTH) resp[8] = 'x';
	return 9;
}

int GetFileOtherAttr(struct stat info, char *resp)
{
	struct tm * mtime;
	if (resp == NULL) return 0;
	mtime = localtime(&info.st_mtime);
	return (sprintf(resp, " %3d %6d %6d %11d %04d%02d%02d", 
		info.st_nlink, info.st_uid, info.st_gid, info.st_size,
		mtime->tm_year+1900, mtime->tm_mon+1, mtime->tm_mday));
} 

void main(int argc, char **argv)
{
	struct stat info;
	char buf[100], *p = buf;
	if (argc != 2) 
	{
		fprintf(stderr, "ls1 filename\n");
		return ;
	}
	memset(buf, 0, sizeof(buf));
	if (lstat(argv[1], &info) == 0)
	{
		p += GetFileType(info.st_mode, p);
		p += GetFileMode(info.st_mode, p);
		p += GetFileOtherAttr(info, p);
		printf("%s %s\n", buf, argv[1]);
	}
	else fprintf(stderr, "open file failed.\n");
}
		
	

