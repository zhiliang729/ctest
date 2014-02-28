#include <comlib.h>

int GetConfigStrValue(char *buf, void *pValue, int nType)
{
	char *nCeAddr[2];
	int nCeLen[2];
	STRRESVAR stuStr;
	stuStr.nAmount = 2;				/* 数据域总数 */
	stuStr.nFlag = 1;				/* 固定分隔 */
	stuStr.filedlen = nCeLen;		/* 每个域的长度 */
	stuStr.nCompartlen = 1;			/* 分隔符号的长度 */
	strcpy(stuStr.szCompart, "=");	/* 分隔字符串 */
	stuStr.filedaddr = nCeAddr;		/* 指向每个域首的指针 */
	strrespre(buf, &stuStr);		/* 解析字符串报文 */
	ASSERT(strresvalue(buf, stuStr, 1, pValue, nType) == 0);
	return 0;	
}

int GetConfigValue(char *szPath, char *szRoot, char *szName, void *pValue, int nType)
{
	FILE *fp;
	int nFlag = 0;							/* 进入数据域的标志 */
	char buf[1024], szRootExt[100], szNameExt[100];
	ASSERT( (fp = fopen(szPath, "r")) != NULL);
	sprintf(szRootExt, "[%s]", szRoot);	/* 数据域“[szRoot]”格式 */
	sprintf(szNameExt, "%s=", szName);		/* 数据项“szName=“格式 */
	while (feof(fp) == 0)					
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), fp);		/* 读取文件中的每一行字符串 */
		if (buf[0] == '#') continue;		/* 注解行，跳过 */
											/* 未进入数据域时，只查询数据域行 */
		if (nFlag == 0 && buf[0] != '[') continue;
											/* 未进入数据域，当前处于数据域行 */
		else if (nFlag == 0 && buf[0] == '[') 
		{									/* 匹配当前数据域是否为所求域 */
			if (strncmp(buf, szRootExt, strlen(szRootExt)) == 0) nFlag = 1;
		}
		else if (nFlag == 1 && buf[0] == '[')
		{									
			break;							/* 数据域遍历完毕 */
		}
		else 								/* 遍历数据域中的配置项 */
		{									/* 匹配当前配置项是否为所求项 */
			if (strncmp(buf, szNameExt, strlen(szNameExt)) == 0)
			{								/* 从配置项中分解配置取值 */
				ASSERT(GetConfigStrValue(buf, pValue, nType) == 0);
				fclose(fp);
				return 0;
			}
		}
	}
	fclose(fp);
	return -1;
}

