#include <comlib.h>

int main(int argc, char *argv[])
{
	int nSock, nSize, i;
	char szBuf[1024];
	if (argc != 2) return 1;
	if (CreateUdpSock(&nSock, atoi(argv[1])) != 0) return 1;
	while (1)
	{
		memset(szBuf, 0, sizeof(szBuf));
		nSize = sizeof(szBuf);
		RecvMsgByUdp(nSock, szBuf, &nSize);
		fprintf(stderr, "Get[%s]\n", szBuf);
	}
}
