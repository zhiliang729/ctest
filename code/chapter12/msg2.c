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
	int ret;
	/* 打开（或创建）消息队列 */
	if ((msid = msgget(0x1234, 0666|IPC_CREAT)) < 0) 
	{
		fprintf(stderr, "open msg %X failed.\n", 0x1234);
		return ;
	}
	while(strncmp(buf.ctext, "exit", 4))
	{
		/* 置空消息缓冲区 */
		memset(&buf, 0, sizeof(buf));	
		/* 接收消息 */
		while ((ret = msgrcv(msid, &buf, sizeof(buf.ctext), 0, 0)) < 0)
		{
			if (errno == EINTR) continue;
			return;
		}
		fprintf(stderr, "Msg : Type=%d, Len=%d, Text:%s", buf.mtype, ret, buf.ctext);
	}
}
