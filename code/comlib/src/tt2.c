#include <comlib.h>
void main()
{
		int semid;
	/* 打开信号量 */	
semid = semget(1000, 2, 0666);


printf("%d", Semop(semid, 1 , 10));
}

