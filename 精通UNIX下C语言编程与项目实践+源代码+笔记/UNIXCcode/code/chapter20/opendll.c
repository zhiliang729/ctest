#include <dlfcn.h>
typedef int (*PFUNC)(char*);		
extern int errno;
int main(int argc, char **argv)
{
	void *pHandle;
	int *p;
	PFUNC pFunc;
	if (argc != 4) return 1;
	if ((pHandle = dlopen(argv[1], RTLD_NOW)) == 0)	/* �򿪶�̬��*/
	{
		printf("Cann't find %s %d\n", argv[1], errno);
		printf(dlerror());
		return 2;
	}
	if ((pFunc = (PFUNC) dlsym(pHandle, argv[2])) != 0)	/* ��ȡ�⺯����ַ */
	/* ��ȡ�⺯������ɹ���ִ�п⺯�� */
	{
		pFunc(argv[3]);
	}
	else				/* ��ȡ�⺯������ʧ�ܣ���ʾ֮ */
	{
		printf("Cann't find func %s\n", argv[2]);
	}
	dlclose(pHandle); 	/* �رն�̬�� */
	return 0;
}
