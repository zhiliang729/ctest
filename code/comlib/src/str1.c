#include <comlib.h>
char szCestr[] = "19999999999999999999������  1234567.89";
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
	stuStr.nAmount = 4;		/* ������ */
	stuStr.nFlag = 0;		/* 0:������1���ָ��� */
	stuStr.filedlen = nCeLen;	/* ÿ����ĳ��� */
	stuStr.nCompartlen = 0;		/* �ָ����ŵĳ��� */
	stuStr.szCompart[0] = 0;	/* �ָ��ַ��� */
	stuStr.filedaddr = nCeAddr;	/* ָ��ÿ�����׵�ָ�� */
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
	
	
