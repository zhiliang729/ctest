#include <dlfcn.h>
typedef int (*PFUNC)(char*);		
extern int errno;
int main(int argc, char **argv)
{
	void *pHandle;
	int *p;
	PFUNC pFunc;
	if (argc != 4) return 1;
	if ((pHandle = dlopen(argv[1], RTLD_NOW)) == 0)	/* 打开动态库*/
	{
		printf("Cann't find %s %d\n", argv[1], errno);
		printf(dlerror());
		return 2;
	}
	if ((pFunc = (PFUNC) dlsym(pHandle, argv[2])) != 0)	/* 获取库函数地址 */
	/* 获取库函数对象成功，执行库函数 */
	{
		pFunc(argv[3]);
	}
	else				/* 获取库函数对象失败，提示之 */
	{
		printf("Cann't find func %s\n", argv[2]);
	}
	dlclose(pHandle); 	/* 关闭动态库 */
	return 0;
}
