#include <unistd.h>
#include <stdio.h>
void main()
{
	pid_t pid;
	if ((pid = fork()) == 0)	/* 子进程 */
	{
		fprintf(stderr, "--begin--\n");
		execl("/bin/ls", "-l", "/u", 0);
		fprintf(stderr, "--end--\n");	
	} 
	else if (pid > 0)	 		/* 父进程 */
	{
		fprintf(stderr, "fork child pid=[%d]\n", pid);			
	}
	else fprintf(stderr, "Fork failed.\n"); /* fork失败 */
}

