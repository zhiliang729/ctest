//
//  debug.c
//  comlib
//
//  Created by zhangliang on 14-2-28.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"


int PrintLog(FILE* pfile, const char * pformat, ...)
{
    va_list _va_list;
    
    char szBuf[BYTESIZE];
    int nLen;
    if (pformat == NULL || pfile == NULL) {/*判断指针是否正确*/
        return -1;
    }
    
    
    TIMESTRU timestru;
    timestru = GetTime();
    
    nLen = sprintf(szBuf, "$Time:%04d.%02d.%02d %02d:%02d:%02d  $Process:[%d] $Log: ",
                   timestru.nYear, timestru.nMon, timestru.nDay,
                   timestru.nHour, timestru.nMin, timestru.nSec, getpid());
    
    va_start(_va_list, pformat);/*初始化变长参数列表*/
    nLen += vsprintf(szBuf+nLen, pformat, _va_list);/*传递变长参数*/
    va_end(_va_list);/*结束使用变长参数列表*/
    
    nLen += sprintf(szBuf + nLen, "\n");/*添加换行符*/
    
    if (fputs(szBuf, pfile) != EOF && fflush(pfile) != EOF) {//输出并刷新文件流  fputs输出失败则返回EOF  任何时候，都可以使用fflush刷新缓冲区，并将缓冲区的内容强制输出到文件中，参数pfile指明了更新的文件流，当其值为NULL时，系统将刷新全部文件流的缓冲区。
        return 0;
    }
    
    
    if (fflush(pfile) == EOF) {
        printf("%s", strerror(errno));
    }
    
    return -2;//错误返回
}

/*将日志写入文件TRACE_FILE中*/
int PrintTraceLog(const char* pformat, ...)
{
    va_list _va_list;
	char szBuf[MAXBUF];
 	FILE *fp;
 	int ret;
	if (pformat == NULL) return -1; 	/*判断指针是否正确*/
    
 	va_start(_va_list, pformat);
 	vsprintf(szBuf, pformat, _va_list);
 	va_end(_va_list);
    
 	if ((fp = fopen(TRACE_FILE, "a")) != NULL)/*打开文件*/
 	{
 		ret = PrintLog(fp, szBuf);	/*写日志文件*/
 		fclose(fp);			/*关闭日志文件*/
	 	return(ret);
 	}
 	return -1;
}

/*以十六进制输出内存信息到文件流pfile中， 程序只需将每16字节转化为一行十六进制字符串，再通过pringlog输出即可*/
int PrintHexLog(FILE * pfile, void * pData, int nSize)
{
    char cLine[16], *pcData;/*16字节*/
	char szBuf[MAXBUF + 1];
	int nPos, nLineSize, nLine, nLen, n;
	if (pfile == NULL || pData == NULL) return -1;
	
	pcData = (char*)pData;
	nLine = 1;
	nLen = sprintf(szBuf, "address[%08X] size[%d]\n", (unsigned int)pData, nSize);
	for (nPos = 0; nPos < nSize; nLine++)
	{
		nLineSize = min(nSize - nPos, 16);		/*防止最后一行数据不足，要么是16，要么比16小 nLineSize表示读入数据长度*/
		memcpy(cLine, pcData + nPos, nLineSize);/*拷贝到内存*/
		nPos += nLineSize;/*移动读取位置*/
		nLen += sprintf(szBuf + nLen, "[%02d]:  ", nLine);/*移动存储到buf的位置，并写入szbuf一个标志：[行号]*/
		for (n = 0; n < nLineSize; n++)
		{
			if (n == 8) nLen += sprintf(szBuf + nLen, " ");/*每八个字节后空格，并移动存储到buf的位置*/
			nLen += sprintf(szBuf + nLen, "%02X ", cLine[n] & 0x00FF);/*将cline中的值逐个写入buf  当字符数组中字符元素充当整型使用时，与0x00ff执行按位与操作可以确保转化整型的正确性，如：cLine[n] & 0x00FF ，否则cLine[n]值可能大于255*/
		}
		for (n = nLineSize; n < 16; n++)	/*最后一行不够16个补空格*/
		{/*采取同样的措施八个字节加空格*/
			if (n == 8) nLen += sprintf(szBuf + nLen, " ");
			nLen += sprintf(szBuf + nLen, "   ");/*3个空格*/
		}
		nLen += sprintf(szBuf + nLen, " :");/*加入一个" :"*/
		for (n = 0; n < nLineSize; n++)		/*以字符显示内存数据*/
		{
			if (!isprint(cLine[n])) cLine[n] = '.';	/*以.显示非打印字符*/
			nLen += sprintf(szBuf + nLen, "%c", cLine[n]);/*将内存数据以字符形式写入*/
		}
		nLen += sprintf(szBuf + nLen, "\n");/*写入回车*/
	}
	PrintLog(pfile, szBuf);
	return 0;
}


/*十六进制日志输出*/
int PrintTraceHexLog(void * pData, int nSize)
{
 	FILE *fp;
 	int ret;
 	if ((fp = fopen(TRACE_FILE, "a")) != NULL)
 	{
 		ret = PrintHexLog(fp, pData, nSize);	/*以十六进制输出内存信息到文件流pfile中*/
 		fclose(fp);				/*关闭文件*/
	 	return(ret);
 	}
 	return -1;
}

/*信息判断函数  当标志bStatus为真时，函数不执行任何操作，否则Verify将向日志文件输出错误信息。其中字符串szBuf描述了错误信息，字符串szFile描述了发生错误的源文件名称，参数nLine代表发生错误的行号
 函数Verify是否能够正确地提示错误所在的源文件名及错误代码所在的行数，关键在于参数szFile和nLine是否正确。事实上，用户不必亲自计算当前代码所在的行数，标准文件编程库提供两个全局标识符："__FILE__"和“__LINE__”,它们在程序编译时自动转化为文件名或者行号等信息
 有了全局标识符，函数Verify的调用宏为
 #define VERIFY(b) Verify((errno = 0) || (b), NULL, __FILE__, __LINE__)
 #define ASSERT(b)  if (VERIFY(b)) \
                        ;\
                    else \
                        return -1;
 */
int Verify(int bStatus, const char * szBuf, const char * szFile, int nLine)
{
        FILE *fp;
        char szFileLine[128], szError[128];
        if (!bStatus)
        {
            memset(szFileLine, 0, sizeof(szFileLine));
            memset(szError, 0, sizeof(szError));
            if (errno != 0) sprintf(szError, "\t> %s\n", strerror(errno));
            if (szFile == NULL) strcpy(szFileLine, "\t> Invalid file name");
            else sprintf(szFileLine, "\t> In line %d file %s", nLine, szFile);
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

