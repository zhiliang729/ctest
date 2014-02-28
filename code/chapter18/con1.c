#include <comlib.h>

int main()
{
	int nNumber, i, nPort;
	char szRoot[100], szIp[100];
	ASSERT(GetConfigValue("./config.ini", "SERVER", "NUMBER", &nNumber, STRINT) == 0);
	PrintLog(stderr, "no=%d", nNumber);
	for (i=0; i<nNumber; i++)
	{
		sprintf(szRoot, "SERVER%02d", i+1);
		memset(szIp, 0, sizeof(szIp));
		GetConfigValue("./config.ini", szRoot, "IP", szIp, STRSTR);
		GetConfigValue("./config.ini", szRoot, "PORT", &nPort, STRINT);
		PrintLog(stderr, "IP=%s PORT=%d", szIp, nPort);
	}
	return 0;
}
