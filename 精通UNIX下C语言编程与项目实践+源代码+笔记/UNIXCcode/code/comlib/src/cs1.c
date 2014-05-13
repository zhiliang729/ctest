#include <comlib.h>
int main()
{
	int nSock;
	char buf[100];
	ConnectSock(&nSock, 80, "218.30.66.101");
	LocateNativeAddr(nSock, buf);
	printf(buf);
}
