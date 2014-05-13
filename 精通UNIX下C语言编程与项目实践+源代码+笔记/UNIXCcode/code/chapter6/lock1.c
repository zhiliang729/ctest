#include <stdio.h>
#include <fcntl.h>
/* 查看文件从start开始len字节内的锁情况 */
void SeeLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type=F_WRLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	if (fcntl(fd, F_GETLK, &arg) == -1) fprintf(stderr, "See Lock failed.\n");
	else if (arg.l_type == F_UNLCK) fprintf(stderr, "No Lock From %d To %d\n", start, len);
	else if (arg.l_type == F_WRLCK) fprintf(stderr, "Write Lock From %d To %d, id=%d\n", start, len, arg.l_pid);
	else if (arg.l_type == F_RDLCK) fprintf(stderr, "Read Lock From %d To %d, id=%d\n", start, len, arg.l_pid);
}
/* 在文件从start开始len字节内的申请读锁，阻塞模式 */
void GetReadLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type=F_RDLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	if (fcntl(fd, F_SETLKW, &arg) == -1) fprintf(stderr, "[%d] Set Read Lock failed.\n", getpid());
	else fprintf(stderr, "[%d] Set Read Lock From %d To %d\n", getpid(), start, len);
}
/* 在文件从start开始len字节内的申请写锁，阻塞模式 */
void GetWriteLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type=F_WRLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	if (fcntl(fd, F_SETLKW, &arg) == -1) fprintf(stderr, "[%d] Set Write Lock failed.\n", getpid());
	else fprintf(stderr, "[%d] Set Write Lock From %d To %d\n", getpid(), start, len);
}
/* 释放文件从start开始len字节内的锁 */
void ReleaseLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type=F_UNLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	if (fcntl(fd, F_SETLKW, &arg) == -1) fprintf(stderr, "[%d] UnLock failed.\n", getpid());
	else fprintf(stderr, "[%d] UnLock From %d To %d\n", getpid(), start, len);
}

void main()
{
	int fd;
	struct flock arg;
	if ((fd = open("/tmp/tlock1", O_RDWR| O_CREAT, 0755)) <0)
	{
		fprintf(stderr, "open file failed.\n");
		return;
	}
	SeeLock(fd, 0, 10);		
	GetReadLock(fd, 0, 10);		/* 申请读锁 */
	SeeLock(fd, 11, 20);		
	GetWriteLock(fd, 11, 20);	/* 申请写锁 */
	sleep(30);		
	ReleaseLock(fd, 0, 10);		/* 释放锁 */
	ReleaseLock(fd, 11, 20);	/* 释放锁 */
}
	
