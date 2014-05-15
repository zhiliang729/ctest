#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
/*VERIFYERR(a, b)�Ƕ���꣬��a�����ǣ�����ӡʧ����Ϣ�������ӡ�ɹ���Ϣ��*/
#define VERIFYERR(a, b)  \
	if (a) \
	{\
		fprintf(stderr, "%s failed.\n", b);  \
		return;\
	}\
	else \
		fprintf(stderr, "%s success.\n", b);  
void main()
{
	int shmid;
	char *pmat = NULL;
	char buf[1024];
	int no;

	/* ���ź��� */	
	VERIFYERR((shmid = shmget(0x1234, 10*1024, 0666|IPC_CREAT)) == -1, "Open(Create) Shm");
	/* ӳ�乲���ڴ棬��ַ�洢��pmat�� */
	VERIFYERR((pmat = (char *)shmat(shmid, 0, 0)) == 0, "Link Shm");
	/* ������µ��ڴ��� */
	printf("Please Input No.(0~9):");
	scanf("%d", &no);
	VERIFYERR(no < 0 || no > 9, "Input No.");
	/* ������µ����� */
	printf("Please Input Data:");
	memset(buf, 0, sizeof(buf));
	scanf("%s", buf);
	/* ���¹����ڴ����� */;
	memcpy(pmat+no*1024, buf, 1024);
	/* �ͷŹ����ڴ�ӳ�� */
	shmdt(pmat);
}
