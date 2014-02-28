#include <dlfcn.h>
void main()
{
	void *pHandle;
	void (*pFunc)();								/* 指向函数的指针 */
	int *p;
	pHandle = dlopen("./dll.so", RTLD_NOW); 		/* 打开动态库*/
	if (!pHandle)
	{
		printf("Cann't find dll.so\n");
		return;
	}
	pFunc = (void (*)()) dlsym(pHandle, "print");	/* 获取库函数地址 */
	if (pFunc) 			/* 获取库函数对象成功，执行库函数 */
	{
		pFunc();
	}
	else				/* 获取库函数对象失败，提示之 */
	{
		printf("Cann't find func print\n");
	}
	p = (int *) dlsym(pHandle, "p");			/* 获取库变量地址 */
	if (p) 				/* 获取库变量成功，使用之 */
	{
		printf("p=%d\n", *p);
	}
	else				/* 获取库变量失败，提示之 */
	{
		printf("Cann't find int p\n");
	}
	dlclose(pHandle); 	/* 关闭动态库 */
}

