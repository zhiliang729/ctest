#include <comlib.h>
#include <stdarg.h>
#include <stdio.h>
int PrintLog(FILE* pfile, const char * pformat, ...)
{
	va_list _va_list;
	TIMESTRU timestru;
	char szBuf[MAXBUF];
	int nLen;
	if (pformat == NULL || pfile == NULL) return -1; 	/* 判断指针是否正确*/
	timestru = GetTime();
	nLen = sprintf(szBuf, " %04d.%02d.%02d %02d:%02d:%02d [%d]: ",
		timestru.nYear, timestru.nMon, timestru.nDay,
		timestru.nHour, timestru.nMin, timestru.nSec, getpid());
		
	va_start(_va_list, pformat); 				/* 初始化变长参数列表 */
	nLen += vsprintf(szBuf+nLen, pformat, _va_list); 	/* 传递变长参数 */
	va_end(_va_list); 					/* 结束使用变长参数列表 */
	
	nLen += sprintf(szBuf + nLen, "\n");			/* 添加换行符 */
	if (fputs(szBuf, pfile) != EOF && fflush(pfile) != EOF) return 0;/*  输出并刷新文件流 */
	return -2;						/* 错误返回 */
}

int PrintTraceLog(const char* pformat, ...)
{
 	va_list _va_list;
	char szBuf[MAXBUF];
 	FILE *fp;
 	int ret;
	if (pformat == NULL) return -1; 	/* 判断指针是否正确*/
		
 	va_start(_va_list, pformat);		  
 	vsprintf(szBuf, pformat, _va_list);     
 	va_end(_va_list);                      

 	if ((fp = fopen(TRACE_FILE, "a")) != NULL)
 	{
 		ret = PrintLog(fp, szBuf);	/* 写日志文件 */
 		fclose(fp);			/* 关闭日志文件 */
	 	return(ret);
 	}
 	return -1;
}

int PrintHexLog(FILE* pfile, void * pData, int nSize)
{
	char cLine[16], *pcData;
	char szBuf[MAXBUF + 1];
	int nPos, nLineSize, nLine, nLen, n;
	if (pfile == NULL || pData == NULL) return -1;
	
	pcData = (char*)pData;
	nLine = 1;
	nLen = sprintf(szBuf, "address[%08X] size[%d]\n", pData, nSize);
	for (nPos = 0; nPos < nSize; nLine++)
	{
		nLineSize = min(nSize - nPos, 16);		/* 防止最后一行数据不足 */
		memcpy(cLine, pcData + nPos, nLineSize);
		nPos += nLineSize;
		nLen += sprintf(szBuf + nLen, "[%02d]:  ", nLine);
		for (n = 0; n < nLineSize; n++)
		{
			if (n == 8) nLen += sprintf(szBuf + nLen, " ");/* 第8个字节后空格 */
			nLen += sprintf(szBuf + nLen, "%02X ", cLine[n] & 0x00FF);
		}
		for (n = nLineSize; n < 16; n++)	/* 最后一行补空格 */
		{
			if (n == 8) nLen += sprintf(szBuf + nLen, " ");
			nLen += sprintf(szBuf + nLen, "   ");
		}
		nLen += sprintf(szBuf + nLen, " :");
		for (n = 0; n < nLineSize; n++)		/* 以字符显示内存数据 */
		{
			if (!isprint(cLine[n])) cLine[n] = '.';	/* 以“.”显示非打印字符 */
			nLen += sprintf(szBuf + nLen, "%c", cLine[n]);
		}
		nLen += sprintf(szBuf + nLen, "\n");
	}
	PrintLog(pfile, szBuf);
	return 0;
}

int PrintTraceHexLog(void * pData, int nSize)
{
 	FILE *fp;
 	int ret;
 	if ((fp = fopen(TRACE_FILE, "a")) != NULL)
 	{
 		ret = PrintHexLog(fp, pData, nSize);	/* 写日志文件 */
 		fclose(fp);				/* 关闭日志文件 */
	 	return(ret);
 	}
 	return -1;
}

int Verify(int bStatus, const char* szBuf, const char* szFile, int nLine)
{
	FILE *fp;
	char szFileLine[128], szError[128];
	if (!bStatus)
	{
		memset(szFileLine, 0, sizeof(szFileLine));       
		memset(szError, 0, sizeof(szError));
		if (errno != 0) sprintf(szError, "\t> %0.64s\n", strerror(errno));
		if (szFile == NULL) strcpy(szFileLine, "\t> Invalid file name");
		else sprintf(szFileLine, "\t> In line %d file %0.32s", nLine, szFile);
		if (szBuf == NULL)  szBuf = "";
		fp = fopen(TRACE_FILE, "a");
		if (fp != NULL)
		{
			PrintLog(fp, "%s[%d]\n%s%s", szBuf, getpid(), szError, szFileLine);
			fclose(fp);
		}
		errno = 0;
	}
	return bStatus;
}
