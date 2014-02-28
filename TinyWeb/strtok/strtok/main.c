//
//  main.c
//  strtok
//
//  Created by zhangliang on 13-12-12.
//  Copyright (c) 2013年 com.gooagoo.Gooagoo. All rights reserved.
//

#include <stdio.h>
#include <string.h>
int isEmptyString(const char * const str);
int main(int argc, const char * argv[])
{
    char * ptr;
    char * buffer;
    char * delims = {"&"};
    char  *array[100];
    buffer = strdup("mobf01&account=zhi&password=123456");
    printf("%s \n", buffer);
    ptr = strtok(buffer, delims);
    for(int j = 0; ptr != NULL && j < 100; j++) {
        printf("word: %s\n", ptr);
        array[j] = ptr;
        ptr = strtok(NULL, delims);
        if (ptr == NULL) {
            break;
        }
    }
    printf("%s \n\n", buffer);
    
    char * mob;//接口
    char * maccount;//account
    char * mpassword;//password
    
    for (int k = 0 ; k < 100; k++) {
        if (array[k] != NULL) {
            int contain = 0;
            char * interface = strstr(array[k], "mob");
            if (interface != NULL) {
                contain = 1;
                mob = interface;
                continue;
            }else{
                
            }
            
            char * account = strstr(array[k], "account");
            if (account != NULL) {
                contain = 1;
                char * aa = strstr(account, "=");
                if (aa) {
                    maccount = ++aa;
                }
                continue;
            }else{
                
            }
            
            char * password = strstr(array[k], "password");
            if (password != NULL) {
                contain = 1;
                char * aa = strstr(password, "=");
                if (aa) {
                    mpassword = ++aa;
                }
                continue;
            }else{
                
            }
            if (contain == 0) {
                break;
            }else{
                continue;
            }
        }else{
            break;
        }
    }
    
    printf("接口：%s\n", mob);
    printf("账号：%s\n", maccount);
    printf("密码：%s\n", mpassword);
    
    if (isEmptyString(mob) == 1) {//不为空
        
    }
    return 0;
}

int isEmptyString(const char * const str)//如果为0则为空，否则不为空
{
    int isEmpty = 1;
    size_t length = strlen(str);
    
    int n;
    for (n = 0; n < length; n++) {
        if (str[n] == 32) {
            isEmpty = 1;
        }else{
            isEmpty = 0;
            //存在一个字符则跳出循环
            break;
        }
    }
    
    if (length == 0) {
        return isEmpty;
    }else {
        return isEmpty;
    }
}
