#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/errno.h>
extern int errno;
struct mymsgbuf 			/* 定义消息结构 */
{
	long	mtype;   		/* 消息类型 */
	char	ctext[100];		/* 消息数据 */
};
void main()
{
	struct mymsgbuf buf;	/* 申请消息缓冲 */
	int msid;
	/* 打开（或创建）消息队列 */
	if ((msid = msgget(0x1234, 0666|IPC_CREAT)) < 0) 
	{
		fprintf(stderr, "open msg %X failed.\n", 0x1234);
		return ;
	}
	while(strncmp(buf.ctext, "exit", 4))
	{
		memset(&buf, 0, sizeof(buf));
		fprintf(stderr, "Please input:");
		/* 从键盘输入消息数据内容 */
		fgets(buf.ctext, sizeof(buf.ctext), stdin);
		/* 设置消息类型 */
		buf.mtype = getpid();		
		/* 发送消息 */
		while ((msgsnd(msid, &buf, strlen(buf.ctext), 0)) < 0)
		{
			if (errno == EINTR) continue;
			return;
		}
	}
}
