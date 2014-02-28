#include <comlib.h>

#define ProcessName "monitor1"
#define ShellName "ps -ef|grep monitor1"
int GetProcess1(char *pHost)
{
	int all = 0, off;
	char szLine[256], szFmt[] = "%s|PROC|%s|%d|1|%d|%s\n";
	FILE *pfile;
	char szUID[20], szPID[20], szPPID[20], szC[20], szSTIME[20], szTTY[20], szTIME[20], szCMD[40];	
	if ((pfile = popen(ShellName, "r")) == NULL)		/* 以管道形式运行进程 */
	{											/* 错误提示 */
		printf(szFmt, ProcessName, pHost, 0, 1, "error:shell");
	}
	else
	{									/* 读取shell进程的每一行输出 */
		memset(szLine, 0, sizeof(szLine));		
		while (fgets(szLine, sizeof(szLine), pfile) != NULL)
		{
			sscanf(szLine, "%s%s%s%s%s%s%s%s", szUID, szPID, szPPID, szC, szSTIME, szTTY, szTIME, szCMD);
			if (strlen(szCMD) >= strlen(ProcessName))
			{
				off = strlen(szCMD)-strlen(ProcessName);
				if (strcmp(szCMD+off, ProcessName) == 0) all++;
			}
       		}
       		pclose(pfile);					/* 关闭管道 */
		if (all > 0) printf(szFmt, pHost, ProcessName, all, 0, "OK");
		else printf(szFmt, pHost, ProcessName, all, 1, "No Process");
	}    
}
int main(int argc, char **argv)
{
	GetProcess1(argv[1]);
	return 0;
}
