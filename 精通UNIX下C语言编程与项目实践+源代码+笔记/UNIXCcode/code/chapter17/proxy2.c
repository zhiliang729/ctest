#include <comlib.h>
int main(int argc, char *argv[])
{
    	int nListenSock = -1, nLocalSock = -1, nRemoteSock = -1;
	pid_t nChild;	    	
	if (argc != 2) 							/* �����в�������proxy1 ���ض˿ڡ� */
	{
    		PrintLog(stdout, "proxy1 LOCALPORT\n");
    		return 1;
    	}
    	/* ---------------------------������------------------------------- */
    											/* �� ���������׽��� */
    	ASSERT(CreateSock(&nListenSock, atoi(argv[1]), 8) == 0);	
    	PrintLog(stdout, "listen service: service is activated.");
    	InitServer();							/* �� ����ת��̨���� */
  	while (1)
   	{
     										/* �� �ȴ������������׽��� */
    		if (!VERIFY(AcceptSock(&nLocalSock, nListenSock) == 0)) 	
				continue;					/* �� �����ȴ� */
		VERIFY((nChild = fork()) >= 0);		/* �� �����ӽ��� */
		if (nChild == 0) break;				/* �ӽ�����ת���ӽ��̴��� */
		close(nLocalSock);					/* �� �����̹ر������׽��� */
	}
	/* ---------------------------�ӽ���------------------------------- */
	close(nListenSock);						/* �� �ӽ��̹ر������׽��� */
								/* ����HTTP����ͷ������Ŀ��������������� */
	if (HttpConnect(nLocalSock, &nRemoteSock) == 0)
		SendReciveServer(nLocalSock, nRemoteSock);			/* ͨ��ת�� */
    	if (nLocalSock >= 0) close(nLocalSock);  /* �� �ӽ��̹رձ��ض��׽��� */
    	if (nRemoteSock >= 0) close(nRemoteSock);/* �� �ӽ��̹ر�Ŀ����׽��� */
}
/* --- ����HTTP����ͷ������Ŀ��������������� --- */
int HttpConnect(int nLocalSock, int *nRemoteSock)
{
	char szIp[30], buf[4096];
	int nread;	
	memset(buf,0,sizeof(buf));
											/* ������ҳ����˷��͵��������� */
	while ((nread = read(nLocalSock, buf, sizeof(buf))) < 0) 
	{	
		if (errno != EINTR)					/* �źŵ���read�жϣ��������� */
		{
			perror("read");					/* �����������ӽ��� */
			return 1;				
		}
	}
	memset(szIp, 0, sizeof(szIp));
	/* ����HTTP���ģ�����֤���ģ�����ȡĿ���������IP��ַ */
	if (GetObjectIp(buf, szIp) != 0) return 2;
	/* ����Ŀ����������˿�80����ҳ����˿ڣ� */
	if (!VERIFY(ConnectSock(nRemoteSock, 80, szIp) == 0))
	{									
		PrintLog(stderr, "Connect Server Fail,Addr[%s]", szIp);
		return 3; 							/* ����Ŀ���ʧ�ܣ������ӽ��� */
	}
	if (WriteFile(*nRemoteSock, buf, nread) != 0)			/* ת������ */
	{
		PrintLog(stderr, "Write message to server error[%d]", nread);
		perror("write");					/* ����ת��ʧ�ܣ������ӽ��� */
		return 4;
	}	
	return 0;								/* �ɹ����� */
}
/* --- ����HTTP���ģ�����֤���ģ�����ȡĿ���������IP��ַ --- */
int GetObjectIp(char *buf, char *ip)
{
	char *p = NULL, *p1 = NULL, hostname[50];
	struct hostent *host; 
	if (strstr(buf, "HTTP") == NULL) return 1;	/* ��֤�Ƿ�ΪHTTP���� */
	/* HTTP���ĵġ�Host: ��ʵ��ͷ��Ϣ�м�����Ŀ������������������»�ȡ�������� */
	if ((p = strstr(buf, "Host:")) == NULL) return 2;	/* HTTP���Ĵ� */
	if ((p1 = strstr(p, "\n")) == NULL) return 3;		/* HTTP���Ĵ� */
	memset(hostname, 0, sizeof(hostname));
	strncpy(hostname, p+strlen("Host: "), p1 - p - strlen("Host: "));
	/* ȥ���ַ���ĩ�˵�CRLF���س����У� */
	if (hostname[strlen(hostname) - 1] == '\n' || hostname[strlen(hostname) - 1] == '\r')  hostname[strlen(hostname) - 1] = 0;
	PrintLog(stderr, "[%s]", hostname);
	if ((host = gethostbyname(hostname)) == NULL)	/* ����תЭ���ַ */
	{
		herror("gethostbyname");	
		return 3;
	}
	strcpy(ip, (char *)inet_ntoa(*((struct in_addr *)host->h_addr)));
	PrintLog(stderr, ip);
	return 0;
}

int SendReciveServer(int nLocalSock, int nRemoteSock)
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
