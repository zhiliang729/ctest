#include <comlib.h>

int optname[] = 	/* ѡ������ */
{
	SO_DEBUG,	SO_REUSEADDR,	SO_REUSEPORT,	SO_KEEPALIVE,
	SO_DONTROUTE,	SO_BROADCAST,	SO_OOBINLINE,	SO_SNDBUF,	
	SO_RCVBUF,	SO_SNDLOWAT,	SO_RCVLOWAT,	SO_TYPE,	
	SO_ERROR
};
char optstr[][20] = 	/* ѡ����������ӡ��ʾ�� */
{
	"SO_DEBUG",	"SO_REUSEADDR",	"SO_REUSEPORT",	"SO_KEEPALIVE",
	"SO_DONTROUTE",	"SO_BROADCAST",	"SO_OOBINLINE",	"SO_SNDBUF",	
	"SO_RCVBUF",	"SO_SNDLOWAT",	"SO_RCVLOWAT",	"SO_TYPE",	
	"SO_ERROR"
};
int main()
{
	int nSock;
	int i, result, nSize;
	struct linger ling;		/* ѡ��SO_LINGER�������ݽṹ */
	struct timeval val1, val2;	/* ���͡����ճ�ʱʱ�����ݽṹ */
	ASSERT((nSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);	/* ����TCP�׽��������� */
	for (i=0; i<sizeof(optname)/sizeof(4); i++)
	{
		result = -1;
		nSize = sizeof(result);
		/* ��ȡ�׽���ѡ��ȡֵ��result�� */
		getsockopt(nSock, SOL_SOCKET, optname[i], &result, &nSize);
		printf("%s:%d\n", optstr[i], result);
	}
	/*----------------��ȡ�׽���ѡ��SO_LINGERȡֵ��linger�ṹling��-------- */
	nSize = sizeof(ling);
	getsockopt(nSock, SOL_SOCKET, SO_LINGER, &ling, &nSize);
	printf("SO_LINGER:%d.%d\n", ling.l_onoff, ling.l_linger);
	/*----------------��ȡ�׽���ѡ��SO_SNDTIMEOȡֵ��timeval�ṹval1��-------- */
	nSize = sizeof(val1);
	getsockopt(nSock, SOL_SOCKET, SO_SNDTIMEO, &val1, &nSize);
	printf("SO_SNDTIMEO:%d.%d\n", val1.tv_sec, val1.tv_usec);
	/*----------------��ȡ�׽���ѡ��SO_RCVTIMEOȡֵ��timeval�ṹval2��-------- */
	nSize = sizeof(val2);
	getsockopt(nSock, SOL_SOCKET, SO_RCVTIMEO, &val2, &nSize);
	printf("SO_RCVTIMEO:%d.%d\n", val2.tv_sec, val2.tv_usec);
}

