#include <stdarg.h>
#include <stdio.h>
int PrintLog(FILE* pfile, const char * pformat, ...)
{
	va_list _va_list;
	char szBuf[1024];
	if (pformat == NULL || pfile == NULL) return -1; 	/* �ж�ָ���Ƿ���ȷ*/
	va_start(_va_list, pformat); 					/* ��ʼ���䳤�����б� */
	vsprintf(szBuf, pformat, _va_list); 			/* ���ݱ䳤���� */
	va_end(_va_list); 								/* ����ʹ�ñ䳤�����б� */
	fputs(szBuf, pfile); 							/* ������ļ��� */
	return 0;
}
void main()
{
	PrintLog(stderr, "[%s][%s][%d][%c]\n", "This", "Is", 5, 'a');
	PrintLog(stderr, "Error[%p][%.2f][%X]\n", NULL, 3.123, 100);
}

