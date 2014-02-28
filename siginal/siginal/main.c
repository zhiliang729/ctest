//
//  main.c
//  siginal
//
//  Created by zhangliang on 13-12-19.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "apue.h"

//eg 1
//#include <stdio.h>
//#include <limits.h>
//static void sig_int(int);
//
//
//int main(int argc, const char * argv[])
//{
//    char buf[MAXLINE];
//    pid_t pid;
//    int status;
//    
//    if (signal(SIGINT, sig_int) == SIG_ERR) {
//        err_sys("signal error");
//    }
//    
//    printf("%% ");
//    
//    while (fgets(buf, MAXLINE, stdin) != NULL) {
//        if (buf[strlen(buf) - 1] == '\n') {
//            buf[strlen(buf) - 1] = 0;
//        }
//        
//        if ((pid = fork()) < 0) {
//            err_sys("fork error");
//        }else if(pid == 0){
//            execlp(buf, buf, (char *)0);
//            err_ret("couldn't execute : %s", buf);
//            exit(127);
//        }
//        
//        if ((pid = waitpid(pid, &status, 0)) < 0) {
//            err_sys("waitpid error");
//        }
//        printf("%% ");
//    }
//    return 0;
//}
//
//void sig_int(int signo)
//{
//    printf("interrupt \n");
//}


//eg 2-3
//#include <errno.h>
//#include <limits.h>
//
//static void	pr_sysconf(char *, int);
//static void	pr_pathconf(char *, char *, int);
//
//int
//main(int argc, char *argv[])
//{
//	if (argc != 2)
//		err_quit("usage: a.out <dirname>");
//    
//#ifdef ARG_MAX
//	printf("ARG_MAX defined to be %d\n", ARG_MAX+0);
//#else
//	printf("no symbol for ARG_MAX\n");
//#endif
//#ifdef _SC_ARG_MAX
//	pr_sysconf("ARG_MAX =", _SC_ARG_MAX);
//#else
//	printf("no symbol for _SC_ARG_MAX\n");
//#endif
//    
//    /* similar processing for all the rest of the sysconf symbols... */
//    
//#ifdef MAX_CANON
//	printf("MAX_CANON defined to be %d\n", MAX_CANON+0);
//#else
//	printf("no symbol for MAX_CANON\n");
//#endif
//#ifdef _PC_MAX_CANON
//	pr_pathconf("MAX_CANON =", argv[1], _PC_MAX_CANON);
//#else
//	printf("no symbol for _PC_MAX_CANON\n");
//#endif
//    
//    /* similar processing for all the rest of the pathconf symbols... */
//    
//	exit(0);
//}
//
//static void
//pr_sysconf(char *mesg, int name)
//{
//	long	val;
//    
//	fputs(mesg, stdout);
//	errno = 0;
//	if ((val = sysconf(name)) < 0) {
//		if (errno != 0) {
//			if (errno == EINVAL)
//				fputs(" (not supported)\n", stdout);
//			else
//				err_sys("sysconf error");
//		} else {
//			fputs(" (no limit)\n", stdout);
//		}
//	} else {
//		printf(" %ld\n", val);
//	}
//}
//
//static void
//pr_pathconf(char *mesg, char *path, int name)
//{
//	long	val;
//    
//	fputs(mesg, stdout);
//	errno = 0;
//	if ((val = pathconf(path, name)) < 0) {
//		if (errno != 0) {
//			if (errno == EINVAL)
//				fputs(" (not supported)\n", stdout);
//			else
//				err_sys("pathconf error, path = %s", path);
//		} else {
//			fputs(" (no limit)\n", stdout);
//		}
//	} else {
//		printf(" %ld\n", val);
//	}
//}


//eg 2-3为路径名动态地分配空间
//#include <errno.h>
//#include <limits.h>
//
//#ifdef PATH_MAX
//static int pathmax = PATH_MAX;
//#else
//static int pathmax = 0;
//#endif
//
//#define SUSV3 201312L
//
//static long posix_version = 0;
//
///*
// if PATH_MAX is indeterminate, no guarantee this is adequate
// */
//
//#define PATH_MAX_GUESS 1024
//
//char * path_alloc(int * sizep)
//{
//    char * ptr;
//    int size;
//    
//    if (posix_version == 0) {
//        posix_version = sysconf(_SC_VERSION);
//    }
//    
//    if (pathmax == 0) {
//        errno = 0;
//    }
//    
//    
//    if ((pathmax = pathconf("/", _PC_PATH_MAX) < 0)) {
//        if (errno == 0) {
//            pathmax = PATH_MAX_GUESS;
//        }else{
//            err_sys("pathconf error for _PC_PATH_MAX");
//        }
//    }else{
//        pathmax++;
//    }
//    
//    if (posix_version < SUSV3) {
//        size = pathmax + 1;
//    }else {
//        size = pathmax;
//    }
//    
//    if ((ptr = malloc(size)) == NULL) {
//        err_sys("malloc error for pathname");
//    }
//    
//    if (sizep != NULL) {
//        *sizep = size;
//    }
//    return ptr;
//}
//
//int main(int argc, char * argv[])
//{
//    int size = 0;
//    char * p = path_alloc(&size);
//    printf(" %s size : %d\n", p, size);
//    return 0;
//}

//eg 2-4 确定文件描述符数
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <time.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, we're not guaranteed that this is adequate.
 *
 */
#define OPEN_MAX_GUESS 256

long open_max(void)
{
    if (openmax == 0) {
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0) {
                openmax = OPEN_MAX_GUESS;
            }else{
                err_sys("sysconf error for _SC_OPEN_MAX");
            }
        }
    }
    return openmax;
}

