#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
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
int StatMsg(int msgid)
{
	char resp[20];
	struct msqid_ds buf;		/* ������Ϣ���нṹ������ */
	memset(&buf, 0, sizeof(buf));	/* �ÿ���Ϣ���нṹ������ */
	msgctl(msgid, IPC_STAT, &buf);	/* ��ȡ��Ϣ���нṹ */
	/* ת����Ϣ���нṹ����ӡ */
	fprintf(stderr, "T     ID        KEY       MODE      OWNER      GROUP     CBYTES       QNUM     QBYTES\n");
	fprintf(stderr, "q %6d %#10.8x %10s %10d %10d %10d %10d %10d\n",
		msgid, 					/* ��ʶ�� */
		buf.msg_perm.key, 			/* �ؼ��� */
		GetFileMode(buf.msg_perm.mode, resp), 	/* ����Ȩ�� */
		buf.msg_perm.uid, 			/* �����û�ID */
		buf.msg_perm.gid, 			/* ������ID */
		buf.msg_cbytes, 			/* ���е�ǰ����Ϣ�ֽ��� */
		buf.msg_qnum, 				/* ���е�ǰ����Ϣ���� */
		buf.msg_qbytes);			/* ���п������ɵ�����ֽ��� */
}
void main(int argc, char *argv[])
{
	int msgid;
	if (argc != 2 && argc != 3) return;
	msgid = atoi(argv[1]);
	if (argc == 2)
	{	/* �鿴��Ϣ������Ϣ */
		StatMsg(msgid);
	}
	else if (argc == 3 && strcmp(argv[2], "c") == 0)
	{	/* ������Ϣ���� */
		if (msgget(msgid, 0666|IPC_CREAT|IPC_EXCL) < 0) 		
			fprintf(stderr, "Create msg %#10.8x failed.\n", msgid);
		else fprintf(stderr, "Create msg %#10.8x success.\n", msgid);
	}
	else if (argc == 3 && strcmp(argv[2], "d") == 0)
	{	/* ɾ����Ϣ���� */
		if (msgctl(msgid, IPC_RMID, NULL) < 0)
			fprintf(stderr, "Delete msg %d failed.\n", msgid);
		else fprintf(stderr, "Delete msg %d success.\n", msgid);
	}	
}
