#include <comlib.h>
#define SIZEMAXMSGSIZE 2048
typedef struct structMSGSTRU
{
    long nType;
    char cText[SIZEMAXMSGSIZE];

} MSGSTRU;
typedef MSGSTRU * PMSGSTRU;

int ReadMsg(int nPid, void * pText, int * pSize, int * pType)
{
	MSGSTRU sMsg;
	int n;
	ASSERT(pText != NULL && pSize != NULL && pType != NULL && nPid > 0 && *pSize > 0);
	memset(pText, 0, *pSize);
	memset(&sMsg, 0, sizeof(MSGSTRU));
	while ((n = msgrcv(nPid, &sMsg, *pSize, *pType, !IPC_NOWAIT)) < 0)
	{
		if (errno == EINTR) continue;
		ASSERT(0);
	}
	memcpy(pText, sMsg.cText, n);
	*pSize = n;
	*pType = sMsg.nType;
	return 0;
}

int WriteMsg(int nPid, void * pText, int nSize, int nType)
{
	MSGSTRU sMsg;
	int n;
	ASSERT(pText != NULL && nPid > 0 && nSize > 0 && nSize <= SIZEMAXMSGSIZE && nType > 0);
	memset(&sMsg, 0, sizeof(sMsg));
	sMsg.nType = nType;
	memcpy(sMsg.cText, pText, nSize);
	while ((n = msgsnd(nPid, &sMsg, nSize, !IPC_NOWAIT)) < 0)
	{
		if (errno == EINTR)  continue;
		ASSERT(0);
	}
	return 0;
}



static int nFlag = 0;

static void OnTimeout(int nSignal)
{
	signal(nSignal, SIG_IGN);
	nFlag = 1;
	return;
}

int ReadMsgExt(int nPid, void * pText, int * pSize, int * pType, int nTimeout)
{
	MSGSTRU sMsg;
	int n;
	ASSERT(pText != NULL && pSize != NULL && pType != NULL && nPid > 0 && *pSize > 0);
	memset(pText, 0, *pSize);
	memset(&sMsg, 0, sizeof(sMsg));
	nFlag = 0;
	alarm(nTimeout);
	signal(SIGALRM, OnTimeout);
	while ((n = msgrcv(nPid, &sMsg, *pSize, *pType, !IPC_NOWAIT)) < 0)
	{
		if (errno == EINTR)
		{
			if (nFlag == 1) ASSERT(0)
			else  continue;
				
		}
		ASSERT(0);
	}
	signal(SIGALRM, SIG_IGN);
	alarm(0);
	memcpy(pText, sMsg.cText, n);
	*pSize = n;
	if (*pType != 0) *pType = sMsg.nType;
	return 0;
}

int WriteMsgExt(int nPid, void * pText, int nSize, int nType, int nTimeout)
{
	MSGSTRU sMsg;
	int n;
	ASSERT(pText != NULL && nPid > 0 && nSize > 0 && nSize <= SIZEMAXMSGSIZE && nType > 0);
	memset(&sMsg, 0, sizeof(sMsg));
	sMsg.nType = nType;
	memcpy(sMsg.cText, pText, nSize);
	nFlag = 0;
	alarm(nTimeout);
	signal(SIGALRM, OnTimeout);
	while ((n = msgsnd(nPid, &sMsg, nSize, !IPC_NOWAIT)) < 0)
	{
		if (errno == EINTR)
		{
			if (nFlag == 1) ASSERT(0)
			else continue;
		}
		ASSERT(0);
	}
	signal(SIGALRM, SIG_IGN);
	alarm(0);
	ASSERT(n != nSize);
	return 0;
}
