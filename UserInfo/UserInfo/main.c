//
//  main.c
//  UserInfo
//
//  Created by zhangliang on 14-5-6.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <grp.h>

//struct passwd {
//	char	*pw_name;		/* user name */
//	char	*pw_passwd;		/* encrypted password */
//	uid_t	pw_uid;			/* user uid */
//	gid_t	pw_gid;			/* user gid */
//	__darwin_time_t pw_change;		/* password change time */
//	char	*pw_class;		/* user access class */
//	char	*pw_gecos;		/* Honeywell login info */
//	char	*pw_dir;		/* home directory */
//	char	*pw_shell;		/* default shell */
//	__darwin_time_t pw_expire;		/* account expiration */
//};

int main(int argc,char** argv,char** env)
{
    struct passwd* pwd;
    pwd=getpwuid(getuid());
    if(pwd)
    {
        printf("你的用户名是：%s\n",pwd->pw_name);
        printf("你的密码是：%s\n", pwd->pw_passwd);
        printf("你的用户组id是：%u\n",pwd->pw_gid);
        printf("你的用户id是：%u\n",pwd->pw_uid);
        printf("你的access class：%s\n", pwd->pw_class);
        printf("你的login info：%s\n", pwd->pw_gecos);
        printf("你的工作目录是：%s\n",pwd->pw_dir);
        printf("你的shell是：%s\n",pwd->pw_shell);
    }
    printf("+++++++++++++++++++++++\n");
    pwd=getpwuid(geteuid());
    if(pwd)
    {
        printf("你的实际用户名是：%s\n", pwd->pw_name);
        printf("你的实际用户组id是：%u\n",pwd->pw_gid);
        printf("你的实际用户id是：%u\n",pwd->pw_uid);
        printf("你的实际工作目录是：%s\n",pwd->pw_dir);
        printf("你的实际shell是：%s\n",pwd->pw_shell);
    }
    
    printf("+++++++++++++++++++++++\n");
    struct group* grp;
    grp=getgrgid(getgid());
    if(grp)
    {
        printf("你的实际用户组名是：%s\n", grp->gr_name);
        printf("你的实际用户组id是：%u\n", grp->gr_gid);
        
    }
    return 0;
}
