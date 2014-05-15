#include <comlib.h>


char * TrimString(char* szDest)
{
	int n, nLen;
	if (szDest != NULL)
	{
		/* �D�D�D�D ����ɾ���ַ����ұߵĿո� �D�D�D�D */
		for (n = strlen(szDest); n > 0; n--)
		{
			if (!isblank(szDest[n - 1]))  break;
		}
		szDest[n] = '\0'; 			/* ���ұ�����һ���ո��滻Ϊ0�ȿ� */
		/* �D�D�D�D ����ɾ���ַ�����ߵĿո� �D�D�D�D */
		nLen = strlen(szDest);
		for (n = 0; n < nLen; n++)
		{
			if (!isblank(szDest[n])) break;
		}
		strcpy(szDest, szDest + n);	/* ����ߵ�һ���ǿո�����ǰ�ƶ������׼ȿ�*/
	}
	return szDest;
}


int strrespre(char *buf, PSTRRESVAR pStrstu)
{
	int i = 0, j = 0;
	char *p1, *p2;
	ASSERT(buf != NULL && pStrstu != NULL && pStrstu->filedlen != NULL);
	ASSERT(pStrstu->filedaddr != NULL);
	if (pStrstu->nFlag == 0)				/* �̶����ȱ��� */
	{
		for (i=0; i<pStrstu->nAmount; i++)
		{
			pStrstu->filedaddr[i] = buf + j;
			j = j + pStrstu->filedlen[i] + pStrstu->nCompartlen;
		}
	}
	else										/* �̶��ָ����� */
	{
		p1 = buf;
		p2 = p1;
		while (p1 != NULL && i<= pStrstu->nAmount)
		{
			pStrstu->filedaddr[i] = p1;			/* p1ָ���������׵�ַ */
			p2 = strstr(p1, pStrstu->szCompart);
			if (p2 == NULL)						/* ���һ�������� */
			{
				pStrstu->filedlen[i] = strlen(p1);
				p1 = NULL;
			}
			else 								/* �����һ�������� */
			{
				pStrstu->filedlen[i] = p2 - p1;	/* �����򳤶� */
				p1 = p2 + pStrstu->nCompartlen;	/* p1ָ����һ���������׵�ַ */
			}
			i++;								/* ��ǰ�������������� */
		}
	}
	return 0;
}

int strresvalue(char *buf, STRRESVAR Strstu, int nIndex, void *pValue, int nType)
{
	char szStrbuf[1024];
	ASSERT(buf != NULL && pValue != NULL);
	ASSERT(Strstu.filedlen != NULL && Strstu.filedaddr != NULL);
	ASSERT(nIndex >=0 && nIndex < Strstu.nAmount);
	memset(szStrbuf, 0, sizeof(szStrbuf));
	/* ����������ԭʼ��Ϣ */
	strncpy(szStrbuf, Strstu.filedaddr[nIndex],Strstu.filedlen[nIndex]);
	TrimString(szStrbuf);		/* ���ԭʼ������β�Ŀո� */
	/* ת���������Ͳ��ش�������Ϣ */
	switch (nType)
	{
		case STRINT :			/* ���� */
			*(int *)pValue = atoi(szStrbuf);
			break;
		case STRDOUBLE:			/* ������ */
			*(double *)pValue = atoi(szStrbuf);
			break;
		default:			/* ȱʡΪ�ַ��� */
			strcpy((char *)pValue, szStrbuf);
			break;
	}
	return 0;
}

