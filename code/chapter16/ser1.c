#include <comlib.h>
int main(int argc, char *argv[])
{
	int i = 0;
	char szBuf[1024];
	if (argc != 4) return;
	while (1)			/* 循环提取 */
	{
		sprintf(szBuf, "%d:%s", i++, argv[1]);	/* 组包 */
		SendMsgByUdp(szBuf, strlen(szBuf), argv[2], atoi(argv[3]));	/* 发送到转发进程 */
		fprintf(stderr, "Send[%s][%d][%s]\n", argv[2], atoi(argv[3]), szBuf);
		sleep(5);				
	}
}
