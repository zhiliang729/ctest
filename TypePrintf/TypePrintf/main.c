//
//  main.c
//  TypePrintf
//
//  Created by zhangliang on 13-11-22.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef unsigned char * byte_pointer;

int big_endian(void)
{
    int val = 1;
    char *p = (char *)&val;
    if (*p > 0) {
        return 0;
    }
    return 1;
}

void show_bytes(byte_pointer start, int len)
{
    int i ;
    for (i = 0; i < len; i++) {
        printf("%.2x ", start[i]);
    }
    printf("\n");
}

void show_int(int x)
{
    show_bytes((byte_pointer ) &x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_double(double x)
{
    show_bytes((byte_pointer) &x, sizeof(double));
}

void show_point(void * x)
{
    show_bytes((byte_pointer)&x, sizeof(void*));
}

void test_show_bytes(int val)
{
    int ival = val;
    float fval = (float)ival;
    int * pval = &ival;
    show_int(ival);
    show_float(fval);
    show_point(pval);
}

void test_show_byte(void)
{
    /*
     大端机器和小端机器是不同的
     */
    int val = 0x12345678;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1);
    show_bytes(valp, 2);
    show_bytes(valp, 3);
}

void inplace_swap(int * x, int * y)
{
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

void left_move()
{
    int val = 128;
    printf("%d", val >> 2);
}

int main(int argc, const char * argv[])
{
    left_move();
    printf("\n");
    
    big_endian();
    test_show_bytes(10);
    printf("\n");
    test_show_byte();
    printf("\n");
    show_int(10);
    show_float(10.0);
    show_double(123.12121);
    
    void * x;
    show_point(&x);
    
    char * s = "ABCDEF";
    show_bytes((unsigned char *)s,  (unsigned long)strlen(s));
    
    int aa = 10, bb = 20;
    inplace_swap(&aa, &bb);
    printf("aa:%d, bb:%d\n", aa, bb);
    return 0;
}



