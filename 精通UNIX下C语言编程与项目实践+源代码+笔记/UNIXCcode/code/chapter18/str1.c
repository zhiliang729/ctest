#include <comlib.h>
char szCestr[] = "0019999999999999999999������  1234567.89";
int nCeLen[4] = {3, 19, 6, 12};		/* ֱ�ӻ�ȡ�̶����ȱ�����ÿ��������ĳ��� */
int main()
{
	int nIndex;
	char *nCeAddr[4], szAccno[20], szName[10];
	double fAmt;
	STRRESVAR stuStr;
	stuStr.nAmount = 4;				/* ���������� */
	stuStr.nFlag = 0;				/* �̶����ȱ��� */
	stuStr.filedlen = nCeLen;		/* ÿ��������ĳ��� */
	stuStr.nCompartlen = 0;			/* �ָ��ַ�������Ϊ0 */
	stuStr.szCompart[0] = 0;		/* �޷ָ��ַ��� */
	stuStr.filedaddr = nCeAddr;		/* Ϊ�洢��������׵�ַ���仺���� */
	strrespre(szCestr, &stuStr);	/* ׼�����Ľ����ṹ */
	/* ��ȡ����������Ϣ */
	strresvalue(szCestr, stuStr, 0, &nIndex, STRINT);
	strresvalue(szCestr, stuStr, 1, szAccno, STRSTR);
	strresvalue(szCestr, stuStr, 2, szName, STRSTR);
	strresvalue(szCestr, stuStr, 3, &fAmt, STRDOUBLE);
	/* ��ӡ����������Ϣ */
	PrintLog(stdout, "id = %d", nIndex);
	PrintLog(stdout, "accno = %s", szAccno);
	PrintLog(stdout, "name = %s", szName);
	PrintLog(stdout, "amt = %.2f", fAmt);
}
