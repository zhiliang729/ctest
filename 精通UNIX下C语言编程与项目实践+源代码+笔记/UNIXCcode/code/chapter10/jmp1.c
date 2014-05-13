#include  <setjmp.h>
int j = 0;
jmp_buf env;
void main ()
{
	int i, k=0;
	i = setjmp(env);
	printf("setjmp=[%d];j=[%d];k=[%d]\n", i, j++, k++);
	if (j > 2) exit(0);
	sleep(1);
	longjmp(env, 2);        
}
