#include <comlib.h>
char szCestr[] = "0019999999999999999999朱云翔  1234567.89";
int nCeLen[4] = {3, 19, 6, 12};		/* 直接获取固定长度报文中每个数据域的长度 */
int main()
{
	int nIndex;
	char *nCeAddr[4], szAccno[20], szName[10];
	double fAmt;
	STRRESVAR stuStr;
	stuStr.nAmount = 4;				/* 数据域总数 */
	stuStr.nFlag = 0;				/* 固定长度报文 */
	stuStr.filedlen = nCeLen;		/* 每个数据域的长度 */
	stuStr.nCompartlen = 0;			/* 分隔字符串长度为0 */
	stuStr.szCompart[0] = 0;		/* 无分隔字符串 */
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
