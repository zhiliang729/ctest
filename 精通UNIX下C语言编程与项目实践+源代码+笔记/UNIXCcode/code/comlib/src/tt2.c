#include <comlib.h>
void main()
{
		int semid;
	/* ���ź��� */	
semid = semget(1000, 2, 0666);


printf("%d", Semop(semid, 1 , 10));
}

