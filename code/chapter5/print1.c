#include <stdarg.h>
#include <stdio.h>
int PrintLog(FILE* pfile, const char * pformat, ...)
{
	va_list _va_list;
	char szBuf[1024];
	if (pformat == NULL || pfile == NULL) return -1; 	/* 判断指针是否正确*/
	va_start(_va_list, pformat); 					/* 初始化变长参数列表 */
	vsprintf(szBuf, pformat, _va_list); 			/* 传递变长参数 */
	va_end(_va_list); 								/* 结束使用变长参数列表 */
	fputs(szBuf, pfile); 							/* 输出到文件流 */
	return 0;
}
void main()
{
	PrintLog(stderr, "[%s][%s][%d][%c]\n", "This", "Is", 5, 'a');
	PrintLog(stderr, "Error[%p][%.2f][%X]\n", NULL, 3.123, 100);
}

