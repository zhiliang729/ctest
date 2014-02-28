#include <comlib.h>
char szCestr[] = "1|!9999999999999999999|!朱云翔|!1234567.89";
int main()
{
	int nIndex, nCeLen[4];			/* 无法直接获取每个数据域的长度 */
	char *nCeAddr[4], szAccno[20], szName[10];
	double fAmt;
	STRRESVAR stuStr;
	stuStr.nAmount = 4;				/* 域总数 */
	stuStr.nFlag = 1;				/* 0:定长域；1：分隔域 */
	stuStr.filedlen = nCeLen;		/* 为存储数据域的长度分配缓冲区 */
	stuStr.nCompartlen = 2;			/* 分隔字符串的长度为2 */
	strcpy(stuStr.szCompart, "|!");	/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;		/* 为存储数据域的首地址分配缓冲区 */
	strrespre(szCestr, &stuStr);	/* 准备报文解析结构 */
	/* 获取报文数据信息 */
	strresvalue(szCestr, stuStr, 0, &nIndex, STRINT);
	strresvalue(szCestr, stuStr, 1, szAccno, STRSTR);
	strresvalue(szCestr, stuStr, 2, szName, STRSTR);
	strresvalue(szCestr, stuStr, 3, &fAmt, STRDOUBLE);
	/* 打印报文数据信息 */
	PrintLog(stdout, "id = %d", nIndex);
	PrintLog(stdout, "accno = %s", szAccno);
	PrintLog(stdout, "name = %s", szName);
	PrintLog(stdout, "amt = %.2f", fAmt);
}
