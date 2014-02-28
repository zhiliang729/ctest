#include <dlfcn.h>
void main()
{
	void *pHandle;
	void (*pFunc)();								/* ָ������ָ�� */
	int *p;
	pHandle = dlopen("./dll.so", RTLD_NOW); 		/* �򿪶�̬��*/
	if (!pHandle)
	{
		printf("Cann't find dll.so\n");
		return;
	}
	pFunc = (void (*)()) dlsym(pHandle, "print");	/* ��ȡ�⺯����ַ */
	if (pFunc) 			/* ��ȡ�⺯������ɹ���ִ�п⺯�� */
	{
		pFunc();
	}
	else				/* ��ȡ�⺯������ʧ�ܣ���ʾ֮ */
	{
		printf("Cann't find func print\n");
	}
	p = (int *) dlsym(pHandle, "p");			/* ��ȡ�������ַ */
	if (p) 				/* ��ȡ������ɹ���ʹ��֮ */
	{
		printf("p=%d\n", *p);
	}
	else				/* ��ȡ�����ʧ�ܣ���ʾ֮ */
	{
		printf("Cann't find int p\n");
	}
	dlclose(pHandle); 	/* �رն�̬�� */
}

