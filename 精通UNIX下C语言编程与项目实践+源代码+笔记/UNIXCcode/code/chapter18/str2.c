#include <comlib.h>
char szCestr[] = "1|!9999999999999999999|!������|!1234567.89";
int main()
{
	int nIndex, nCeLen[4];			/* �޷�ֱ�ӻ�ȡÿ��������ĳ��� */
	char *nCeAddr[4], szAccno[20], szName[10];
	double fAmt;
	STRRESVAR stuStr;
	stuStr.nAmount = 4;				/* ������ */
	stuStr.nFlag = 1;				/* 0:������1���ָ��� */
	stuStr.filedlen = nCeLen;		/* Ϊ�洢������ĳ��ȷ��仺���� */
	stuStr.nCompartlen = 2;			/* �ָ��ַ����ĳ���Ϊ2 */
	strcpy(stuStr.szCompart, "|!");	/* �ָ��ַ��� */
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
