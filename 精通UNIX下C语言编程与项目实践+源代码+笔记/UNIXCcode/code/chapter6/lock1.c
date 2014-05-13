#include <stdio.h>
#include <fcntl.h>
/* �鿴�ļ���start��ʼlen�ֽ��ڵ������ */
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
/* ���ļ���start��ʼlen�ֽ��ڵ��������������ģʽ */
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
/* ���ļ���start��ʼlen�ֽ��ڵ�����д��������ģʽ */
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
/* �ͷ��ļ���start��ʼlen�ֽ��ڵ��� */
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
	GetReadLock(fd, 0, 10);		/* ������� */
	SeeLock(fd, 11, 20);		
	GetWriteLock(fd, 11, 20);	/* ����д�� */
	sleep(30);		
	ReleaseLock(fd, 0, 10);		/* �ͷ��� */
	ReleaseLock(fd, 11, 20);	/* �ͷ��� */
}
	
