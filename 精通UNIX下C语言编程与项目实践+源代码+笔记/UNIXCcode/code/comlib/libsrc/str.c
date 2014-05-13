#include <comlib.h>


char * TrimString(char* szDest)
{
	int n, nLen;
	if (szDest != NULL)
	{
		/* ―――― 以下删除字符串右边的空格 ―――― */
		for (n = strlen(szDest); n > 0; n--)
		{
			if (!isblank(szDest[n - 1]))  break;
		}
		szDest[n] = '\0'; 			/* 将右边最靠左的一个空格替换为0既可 */
		/* ―――― 以下删除字符串左边的空格 ―――― */
		nLen = strlen(szDest);
		for (n = 0; n < nLen; n++)
		{
			if (!isblank(szDest[n])) break;
		}
		strcpy(szDest, szDest + n);	/* 从左边第一个非空格起向前移动到串首既可*/
	}
	return szDest;
}


int strrespre(char *buf, PSTRRESVAR pStrstu)
{
	int i = 0, j = 0;
	char *p1, *p2;
	ASSERT(buf != NULL && pStrstu != NULL && pStrstu->filedlen != NULL);
	ASSERT(pStrstu->filedaddr != NULL);
	if (pStrstu->nFlag == 0)				/* 固定长度报文 */
	{
		for (i=0; i<pStrstu->nAmount; i++)
		{
			pStrstu->filedaddr[i] = buf + j;
			j = j + pStrstu->filedlen[i] + pStrstu->nCompartlen;
		}
	}
	else										/* 固定分隔报文 */
	{
		p1 = buf;
		p2 = p1;
		while (p1 != NULL && i<= pStrstu->nAmount)
		{
			pStrstu->filedaddr[i] = p1;			/* p1指向数据域首地址 */
			p2 = strstr(p1, pStrstu->szCompart);
			if (p2 == NULL)						/* 最后一个数据域 */
			{
				pStrstu->filedlen[i] = strlen(p1);
				p1 = NULL;
			}
			else 								/* 非最后一个数据域 */
			{
				pStrstu->filedlen[i] = p2 - p1;	/* 数据域长度 */
				p1 = p2 + pStrstu->nCompartlen;	/* p1指向下一个数据域首地址 */
			}
			i++;								/* 当前解析的数据域编号 */
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
	/* 拷贝数据域原始信息 */
	strncpy(szStrbuf, Strstu.filedaddr[nIndex],Strstu.filedlen[nIndex]);
	TrimString(szStrbuf);		/* 清除原始数据首尾的空格 */
	/* 转化数据类型并回传数据信息 */
	switch (nType)
	{
		case STRINT :			/* 整型 */
			*(int *)pValue = atoi(szStrbuf);
			break;
		case STRDOUBLE:			/* 浮点型 */
			*(double *)pValue = atoi(szStrbuf);
			break;
		default:			/* 缺省为字符型 */
			strcpy((char *)pValue, szStrbuf);
			break;
	}
	return 0;
}

