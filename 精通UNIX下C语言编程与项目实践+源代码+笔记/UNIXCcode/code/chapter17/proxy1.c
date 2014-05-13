#include <comlib.h>

int main(int argc, char *argv[])
{
    	int nListenSock = 0, nLocalSock = 0, nRemoteSock = 0;
	if (argc != 4) 		/* �����в�������proxy1 Զ��IP��ַ Զ�̶˿� ���ض˿ڡ� */
	{
    		PrintLog(stdout, "proxy1 IP PORT LOCALPORT\n");
    		return 1;
    	}
    	/* ���������׽��� */
    	ASSERT(CreateSock(&nListenSock, atoi(argv[3]), 8) == 0);	
    	PrintLog(stdout, "listen service: service is activated.");
  	while (1)
   	{
     	/* �����׽������� */
    		if (!VERIFY(AcceptSock(&nLocalSock, nListenSock) == 0)) 	
				continue;					/* �������� */
	    	/* ����Ŀ���*/
		if (!VERIFY(ConnectSock(&nRemoteSock, atoi(argv[2]), argv[1]) == 0))
		{									/* ����Ŀ���ʧ�� */
			PrintLog(stderr, "Connect Server Fail,Addr[%s],Port[%d]", argv[1], argv[2]);
			return 2;
		}
		SendReceiveServer(nLocalSock, nRemoteSock);		/* ͨ��ת�� */
    		close(nLocalSock);					/* �رձ��ض��׽��� */
    		close(nRemoteSock);					/* �ر�Ŀ����׽��� */
	}
}


int SendReceiveServer(int nLocalSock, int nRemoteSock)
{
	int nMaxSock;
	struct timeval	wait;
	fd_set fdset;
	int ret;
	while(1)
	{
		/* �ٴ����׽������������� */
		FD_ZERO(&fdset);
		FD_SET(nLocalSock,&fdset);
		FD_SET(nRemoteSock,&fdset);

		/* ��׼����ʱʱ�� */
		wait.tv_sec=60;
		wait.tv_usec=0;
		/* �۵���select����鷵�� */
		nMaxSock = nLocalSock > nRemoteSock ? nLocalSock : nRemoteSock;
		if (select(nMaxSock+1,&fdset, NULL, NULL, &wait) == -1)
		{
			PrintLog(stderr, "Select Failed.");
			return 1;
		}
		/* �ܼ��״̬�仯���׽��� */
		
		/* ����׽���nLocalSock���Խ��գ���������ݣ��������ݵ��׽���nRemoteSock */
		if (FD_ISSET(nLocalSock, &fdset))
		{
			PrintLog(stderr, "Local To Remote");
			ret = TransSock(nLocalSock, nRemoteSock);
		}
		/* ����׽���nRemoteSock���Խ��գ���������ݣ����͵��׽���nLocalSock */
		else if (FD_ISSET(nRemoteSock, &fdset))			
		{
			PrintLog(stderr, "Remote To Local");
			ret = TransSock(nRemoteSock, nLocalSock);
		}
		/* ����ж� */
		if (ret == 1) return 1;
		else if (ret == 2) break;
	}/* end for while */
	return 0;
}

int TransSock(int nReadSock, int nWriteSock)
{
	int nread;
	char	buf[4096];
	memset(buf,0,sizeof(buf));
	if ((nread = read(nReadSock, buf, sizeof(buf))) < 0) /* �������� */
	{
		if (errno != EINTR) return 1;
		else return 0;
	}
	else if (nread == 0)				/* �׽��ֹر� */
	{ 
		PrintLog(stderr, "client is close");
		return 2;
	}
	PrintLog(stderr, "[%d]", nread);
	if (WriteFile(nWriteSock, buf, nread) != 0)	/* ת������ */
	{
		PrintLog(stderr, "Write message to server error[%d]", nread);
		perror("write");
		return 1;
	}
	return 0;
}
