#include <comlib.h>
void main()
{
	int i=10000;
	PrintLog(stderr, "This is test[%d]", i);
	PrintTraceLog("This is test[%d]", i);
	PrintHexLog(stderr, &i, sizeof(i));
	PrintTraceHexLog(&i, sizeof(int));
	PrintTraceLog("------------------");
	Verify(0, NULL, __FILE__, __LINE__);
	PrintTraceLog("------------------");
	VERIFY(0);
	PrintTraceLog("------------------");
	VERIFY(1);
}
