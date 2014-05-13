#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/errno.h>
extern int errno;
struct mymsgbuf 			/* ������Ϣ�ṹ */
{
	long	mtype;   		/* ��Ϣ���� */
	char	ctext[100];		/* ��Ϣ���� */
};
void main()
{
	struct mymsgbuf buf;	/* ������Ϣ���� */
	int msid;
	int ret;
	/* �򿪣��򴴽�����Ϣ���� */
	if ((msid = msgget(0x1234, 0666|IPC_CREAT)) < 0) 
	{
		fprintf(stderr, "open msg %X failed.\n", 0x1234);
		return ;
	}
	while(strncmp(buf.ctext, "exit", 4))
	{
		/* �ÿ���Ϣ������ */
		memset(&buf, 0, sizeof(buf));	
		/* ������Ϣ */
		while ((ret = msgrcv(msid, &buf, sizeof(buf.ctext), 0, 0)) < 0)
		{
			if (errno == EINTR) continue;
			return;
		}
		fprintf(stderr, "Msg : Type=%d, Len=%d, Text:%s", buf.mtype, ret, buf.ctext);
	}
}
