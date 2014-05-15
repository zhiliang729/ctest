#include <comlib.h>

int optname[] = 	/* 选项内容 */
{
	SO_DEBUG,	SO_REUSEADDR,	SO_REUSEPORT,	SO_KEEPALIVE,
	SO_DONTROUTE,	SO_BROADCAST,	SO_OOBINLINE,	SO_SNDBUF,	
	SO_RCVBUF,	SO_SNDLOWAT,	SO_RCVLOWAT,	SO_TYPE,	
	SO_ERROR
};
char optstr[][20] = 	/* 选项描述，打印提示用 */
{
	"SO_DEBUG",	"SO_REUSEADDR",	"SO_REUSEPORT",	"SO_KEEPALIVE",
	"SO_DONTROUTE",	"SO_BROADCAST",	"SO_OOBINLINE",	"SO_SNDBUF",	
	"SO_RCVBUF",	"SO_SNDLOWAT",	"SO_RCVLOWAT",	"SO_TYPE",	
	"SO_ERROR"
};
int main()
{
	int nSock;
	int i, result, nSize;
	struct linger ling;		/* 选项SO_LINGER传输数据结构 */
	struct timeval val1, val2;	/* 发送、接收超时时间数据结构 */
	ASSERT((nSock = socket(AF_INET, SOCK_STREAM, 0)) > 0);	/* 创建TCP套接字描述符 */
	for (i=0; i<sizeof(optname)/sizeof(4); i++)
	{
		result = -1;
		nSize = sizeof(result);
		/* 获取套接字选项取值到result中 */
		getsockopt(nSock, SOL_SOCKET, optname[i], &result, &nSize);
		printf("%s:%d\n", optstr[i], result);
	}
	/*----------------获取套接字选项SO_LINGER取值到linger结构ling中-------- */
	nSize = sizeof(ling);
	getsockopt(nSock, SOL_SOCKET, SO_LINGER, &ling, &nSize);
	printf("SO_LINGER:%d.%d\n", ling.l_onoff, ling.l_linger);
	/*----------------获取套接字选项SO_SNDTIMEO取值到timeval结构val1中-------- */
	nSize = sizeof(val1);
	getsockopt(nSock, SOL_SOCKET, SO_SNDTIMEO, &val1, &nSize);
	printf("SO_SNDTIMEO:%d.%d\n", val1.tv_sec, val1.tv_usec);
	/*----------------获取套接字选项SO_RCVTIMEO取值到timeval结构val2中-------- */
	nSize = sizeof(val2);
	getsockopt(nSock, SOL_SOCKET, SO_RCVTIMEO, &val2, &nSize);
	printf("SO_RCVTIMEO:%d.%d\n", val2.tv_sec, val2.tv_usec);
}

