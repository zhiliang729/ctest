#include <comlib.h>
int main()
{
	int ret, i = 0;
	char szBuf[100];
	while (1)
	{
		sprintf(szBuf, "��%d�η���", i);	/* ��� */
		ret = SendMsgByUdp(szBuf, strlen(szBuf), "127.0.0.1", 9999); /* ���� */
		if (ret == 0) printf("Send UDP Success:%s\n", szBuf);
		else printf("Send UDP Failed:%s\n", szBuf);
		sleep(1);
		i++;		/* i�����͵Ĵ��� */
	}
}
