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
	/* �򿪣��򴴽�����Ϣ���� */
	if ((msid = msgget(0x1234, 0666|IPC_CREAT)) < 0) 
	{
		fprintf(stderr, "open msg %X failed.\n", 0x1234);
		return ;
	}
	while(strncmp(buf.ctext, "exit", 4))
	{
		memset(&buf, 0, sizeof(buf));
		fprintf(stderr, "Please input:");
		/* �Ӽ���������Ϣ�������� */
		fgets(buf.ctext, sizeof(buf.ctext), stdin);
		/* ������Ϣ���� */
		buf.mtype = getpid();		
		/* ������Ϣ */
		while ((msgsnd(msid, &buf, strlen(buf.ctext), 0)) < 0)
		{
			if (errno == EINTR) continue;
			return;
		}
	}
}
