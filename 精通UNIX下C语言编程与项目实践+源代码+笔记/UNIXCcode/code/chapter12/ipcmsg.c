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
	struct msqid_ds buf;		/* 申请消息队列结构缓冲区 */
	memset(&buf, 0, sizeof(buf));	/* 置空消息队列结构缓冲区 */
	msgctl(msgid, IPC_STAT, &buf);	/* 读取消息队列结构 */
	/* 转化消息队列结构并打印 */
	fprintf(stderr, "T     ID        KEY       MODE      OWNER      GROUP     CBYTES       QNUM     QBYTES\n");
	fprintf(stderr, "q %6d %#10.8x %10s %10d %10d %10d %10d %10d\n",
		msgid, 					/* 标识号 */
		buf.msg_perm.key, 			/* 关键字 */
		GetFileMode(buf.msg_perm.mode, resp), 	/* 访问权限 */
		buf.msg_perm.uid, 			/* 有限用户ID */
		buf.msg_perm.gid, 			/* 有限组ID */
		buf.msg_cbytes, 			/* 队列当前的消息字节数 */
		buf.msg_qnum, 				/* 队列当前的消息个数 */
		buf.msg_qbytes);			/* 队列可以容纳的最大字节数 */
}
void main(int argc, char *argv[])
{
	int msgid;
	if (argc != 2 && argc != 3) return;
	msgid = atoi(argv[1]);
	if (argc == 2)
	{	/* 查看消息队列信息 */
		StatMsg(msgid);
	}
	else if (argc == 3 && strcmp(argv[2], "c") == 0)
	{	/* 创建消息队列 */
		if (msgget(msgid, 0666|IPC_CREAT|IPC_EXCL) < 0) 		
			fprintf(stderr, "Create msg %#10.8x failed.\n", msgid);
		else fprintf(stderr, "Create msg %#10.8x success.\n", msgid);
	}
	else if (argc == 3 && strcmp(argv[2], "d") == 0)
	{	/* 删除消息队列 */
		if (msgctl(msgid, IPC_RMID, NULL) < 0)
			fprintf(stderr, "Delete msg %d failed.\n", msgid);
		else fprintf(stderr, "Delete msg %d success.\n", msgid);
	}	
}
