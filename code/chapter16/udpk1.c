#include <comlib.h>
int main()
{
	int ret, i = 0;
	char szBuf[100];
	while (1)
	{
		sprintf(szBuf, "第%d次发送", i);	/* 组包 */
		ret = SendMsgByUdp(szBuf, strlen(szBuf), "127.0.0.1", 9999); /* 发送 */
		if (ret == 0) printf("Send UDP Success:%s\n", szBuf);
		else printf("Send UDP Failed:%s\n", szBuf);
		sleep(1);
		i++;		/* i代表发送的次数 */
	}
}
