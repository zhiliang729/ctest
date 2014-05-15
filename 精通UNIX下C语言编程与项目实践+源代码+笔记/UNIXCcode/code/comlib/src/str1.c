#include <comlib.h>
char szCestr[] = "19999999999999999999朱云翔  1234567.89";
int nCeLen[4] = {1, 19, 6, 12};
char buf[10];
char *nCeAddr[4] = {buf, buf + 1, buf + 1 + 6, buf + 1 + 6 + 13};
int main()
{
	char *nCeAddr[4];
	int nIndex;
	char szAccno[20];
	char szName[10];
	double fAmt;
	STRRESVAR stuStr;
	stuStr.nAmount = 4;		/* 域总数 */
	stuStr.nFlag = 0;		/* 0:定长域；1：分隔域 */
	stuStr.filedlen = nCeLen;	/* 每个域的长度 */
	stuStr.nCompartlen = 0;		/* 分隔符号的长度 */
	stuStr.szCompart[0] = 0;	/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;	/* 指向每个域首的指针 */
	strrespre(szCestr, &stuStr);
	
	strresvalue(szCestr, &stuStr, 0, &nIndex, STRINT);
	strresvalue(szCestr, &stuStr, 1, szAccno, STRSTR);
	strresvalue(szCestr, &stuStr, 2, szName, STRSTR);
	strresvalue(szCestr, &stuStr, 3, &fAmt, STRDOUBLE);
	
	PrintLog(stderr, "id = %d", nIndex);
	PrintLog(stderr, "accno = %s", szAccno);
	PrintLog(stderr, "name = %s", szName);
	PrintLog(stderr, "amt = %.2f", fAmt);
}
	
	
