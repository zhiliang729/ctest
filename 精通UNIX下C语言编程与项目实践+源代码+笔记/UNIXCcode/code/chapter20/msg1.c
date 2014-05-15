#include <comlib.h>

int GetMsg1(char *pHost)
{
	char szBuf[256], szFmt[] = "%s|MSG|0x1234|%d|%d|%d|%s\n";
	struct msqid_ds buf;						/* 申请消息队列结构缓冲区 */
	int msgid, unused;
	float rate;
	memset(&buf, 0, sizeof(buf));				/* 置空消息队列结构缓冲区 */

	if ((msgid = msgget(0x1234, 0666)) == -1 || 
		msgctl(msgid, IPC_STAT, &buf) == -1)		/* 读取消息队列结构 */
	{
		printf(szFmt, pHost, 0, 0, 1, strerror(errno));
		return 0;
	}
	unused = buf.msg_qbytes - buf.msg_cbytes;
	rate = unused / buf.msg_qbytes;
	memset(szBuf, 0, sizeof(szBuf));
	sprintf(szBuf, "space: %d of %d available (%.2f%%).", unused, buf.msg_qbytes, rate*100);
	if (rate >= 0.9)
	{
		printf(szFmt, pHost, unused , buf.msg_qbytes, 2, szBuf);
	}
	else
	{
		printf(szFmt, pHost, unused , buf.msg_qbytes, 0, szBuf);
	}
}

int main(int argc, char **argv)
{
	GetMsg1(argv[1]);
	return 0;
}
