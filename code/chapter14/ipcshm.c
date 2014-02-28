#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.\n", b);  \
		return;\
	}\
	else \
		fprintf(stderr, "%s success.\n", b);  
char * GetFileMode(mode_t st_mode, char *resp)
{
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
	resp[9] = 0;
	return resp;
}
int StatShm(int shmid)
{
	char resp[20];
	struct shmid_ds buf;
	memset(&buf, 0, sizeof(buf));	/* �ÿ���Ϣ���нṹ������ */
	shmctl(shmid, IPC_STAT, &buf);
	
	/* ת�������ڴ�ṹ����ӡ */
	fprintf(stderr, "T     ID        KEY       MODE      OWNER      GROUP     NATTCH       SEGSZ      CPID       LPID\n");
	fprintf(stderr, "m %6d %#10.8x %10s %10d %10d %10d %10d %10d %10d\n",
		shmid, 					/* ��ʶ�� */
		buf.shm_perm.key, 			/* �ؼ��� */
		GetFileMode(buf.shm_perm.mode, resp), 	/* ����Ȩ�� */
		buf.shm_perm.uid, 			/* �����û�ID */
		buf.shm_perm.gid, 			/* ������ID */
		buf.shm_nattch,				/* �����ڴ������� */
		buf.shm_segsz, 				/* �����ڴ��ֽ��� */
		buf.shm_cpid, 				/* ���������ڴ���û���ID��	*/
		buf.shm_lpid);				/* ���һ�β��������ڴ���û�ID */
}

void main(int argc, char *argv[])
{
	int shmid, size;
	if (argc != 3) return;
	shmid = atoi(argv[1]);
	if (strcmp(argv[2], "v") == 0)
	{	/* ��ѯ�����ڴ� */
		StatShm(shmid);
	}
	else if (strcmp(argv[2], "d") == 0)
	{	/* ɾ�������ڴ� */
		VERIFYERR(shmctl(shmid, IPC_RMID, NULL) < 0, "Delete Shm");
	}	
	else
	{
		size = atoi(argv[2]);
		VERIFYERR(shmget(shmid, size, 0666|IPC_CREAT|IPC_EXCL) < 0, "Create Shm");
	}
}

