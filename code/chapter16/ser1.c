#include <comlib.h>
int main(int argc, char *argv[])
{
	int i = 0;
	char szBuf[1024];
	if (argc != 4) return;
	while (1)			/* ѭ����ȡ */
	{
		sprintf(szBuf, "%d:%s", i++, argv[1]);	/* ��� */
		SendMsgByUdp(szBuf, strlen(szBuf), argv[2], atoi(argv[3]));	/* ���͵�ת������ */
		fprintf(stderr, "Send[%s][%d][%s]\n", argv[2], atoi(argv[3]), szBuf);
		sleep(5);				
	}
}
