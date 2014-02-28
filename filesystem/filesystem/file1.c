//
//  file1.c
//  filesystem
//
//  Created by zhangliang on 14-2-26.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <sys/mount.h>
#include <sys/param.h>

//int statfs(const char * path, struct statfs * buf);
//int fstatfs(int fd, struct statfs * buf);
int main(int argc, char * argv[])
{
    struct statfs buf;
    
    if (argc != 2) {
        printf("error!\n");
        return 1;
    }
    
//    struct statfs {
//        short	f_otype;		/* TEMPORARY SHADOW COPY OF f_type */
//        short	f_oflags;		/* TEMPORARY SHADOW COPY OF f_flags */
//        long	f_bsize;		/* fundamental file system block size */
//        long	f_iosize;		/* optimal transfer block size */
//        long	f_blocks;		/* total data blocks in file system */
//        long	f_bfree;		/* free blocks in fs */
//        long	f_bavail;		/* free blocks avail to non-superuser */
//        long	f_files;		/* total file nodes in file system */
//        long	f_ffree;		/* free file nodes in fs */
//        fsid_t	f_fsid;			/* file system id */
//        uid_t	f_owner;		/* user that mounted the filesystem */
//        short	f_reserved1;	/* spare for later */
//        short	f_type;			/* type of filesystem */
//        long	f_flags;		/* copy of mount exported flags */
//        long    f_reserved2[2];	/* reserved for future use */
//        char	f_fstypename[MFSNAMELEN]; /* fs type name */
//        char	f_mntonname[MNAMELEN];	/* directory on which mounted */
//        char	f_mntfromname[MNAMELEN];/* mounted filesystem */
//        char	f_reserved3;	/* For alignment */
//        long	f_reserved4[4];	/* For future use */
//    };

    statfs(argv[1], &buf);
    printf("type of filesystem = %d\n", buf.f_type);
    printf("fundamental file system block size=%d\n", buf.f_bsize);
    printf("total data blocks in file system=%lld\n", buf.f_blocks);
    printf("free blocks in file system=%lld\n", buf.f_bfree);
    printf("total file nodes in file system=%lld\n", buf.f_files);
    printf("free file nodes in file system=%lld\n", buf.f_ffree);
    printf("%s : %lld blocks  %lld i-nodes\n", argv[1], buf.f_bfree, buf.f_ffree);
    return 0;
}