#include <comlib.h>

struct cc
{
	long i;
	char c[100];
};

int WriteMsg(int nMsg, void * pData, int nSize, int nType)
{
    struct cc  msgstru;
    int n;

    memset(&msgstru, 0, sizeof(msgstru));

    msgstru.i = nType;
    memcpy(msgstru.c, pData, nSize);

    while ((n = msgsnd(nMsg, &msgstru, nSize, !IPC_NOWAIT)) < 0)
    {
        if (errno == EINTR)
            continue;

        ASSERT(1);
    }

    return 0;
}

void main()
{
	int msid;
	struct cc c1;
	msid = msgget(0x1234, 0666|IPC_CREAT);
	c1.i = 100;
	WriteMsg(msid , &c1, 100, 1001);
}
