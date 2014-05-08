//
//  str.c
//  comlib
//
//  Created by zhangliang on 14-5-7.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"
/*
 填充字符串报文解析的结构
 字符串报文存储在buf中，参数pStrstu指向一个字符串报文解析结构，函数成功时返回0，否则返回非0值。
 函数对固定长度报文和固定分割报文采用了不同的解析方法：对于前者，函数要求调用者提供每个数据域的长度，函数根据此值填充每个数据域的首地址值；对于后者，函数从报文头开始查询，每找到一个分割字符串，就意味着分解了一个数据域，分割字符串的下一个字节就是下一个数据域的首地址，两个字符串之间的距离就是数据域的长度
 */
int strrespre(char * buf, PSTRRESVAR pStrstu)
{
    int i = 0;
    unsigned long j = 0;
    char * p1, * p2;
    ASSERT(buf != NULL && pStrstu != NULL && pStrstu->filedlen != NULL);
    ASSERT(pStrstu->filedaddr != NULL);
    if (pStrstu->nFlag == 0) {/*固定长度报文*/
        for (i = 0; i < pStrstu->nAmount; i++) {
            pStrstu->filedaddr[i] = buf + j;//指定每个数据域的首地址
            j = j + pStrstu->filedlen[i] + pStrstu->nCompartlen;
        }
    }else{/*固定分割*/
        p1 = buf;
        p2 = p1;
        while (p1 != NULL && i <= pStrstu->nAmount) {
            pStrstu->filedaddr[i] = p1;/*p1指向数据域首地址*/
            p2 = strstr(p1, pStrstu->szCompart);/*查找下一个分割符*/
            /*
             char *
             strstr(const char *s1, const char *s2);
             
             The strstr() function locates the first occurrence of the null-terminated
             string s2 in the null-terminated string s1.
             */
            if (p2 == NULL) {/*最后一个数据域*/
                pStrstu->filedlen[i] = strlen(p1);
                p1 = NULL;/*跳出循环*/
            }else{/*非最后一个数据域*/
                pStrstu->filedlen[i] = p2 - p1;/*数据域长度*/
                p1 = p2 + pStrstu->nCompartlen;/*p1指向下一个数据与首地址*/
            }
            i++;/*当前解析的数据域编号*/
        }
    }
    return 0;
}

/*
 字符串报文数据域的读取,一旦字符串报文解析结构填充完毕，用户就可调用下面函数获取每一个数据域的值
 本函数读取字符串文件buf中第nIndex项（从0开始计数）数据的取值。其中参数Strstu是一个字符串报文解析结构，它在函数strrespre中被填充。参数nType制定了该项数据域的数据类型，函数所支持的数据类型定义在头文件“comlib.h”中，参数pValue指定了回传数据域取值的缓冲区。函数成功时返回0，否则返回非0值
 */
int strresvalue(char * buf, STRRESVAR Strstu, int nIndex, void * pValue, int nType)
{
    char szStrbuf[1024];
    ASSERT(buf != NULL && pValue != NULL);
    ASSERT(Strstu.filedlen != NULL && Strstu.filedaddr != NULL);
    ASSERT(nIndex >= 0 && nIndex < Strstu.nAmount);
    memset(szStrbuf, 0, sizeof(szStrbuf));
    /*拷贝数据与原始信息*/
    strncpy(szStrbuf, Strstu.filedaddr[nIndex], Strstu.filedlen[nIndex]);/*字符串报文中第nIndex号数据域的首地址在“buf+filedaddr[nindex]”处，那么第nIndex号数据域的取值就是从该地址开始的“filedlen[nIndex]”字节的字符数据，程序将这部分字符串拷贝出来，并转化为相应地数据类型后回传出去*/
    TrimString(szStrbuf);/*清除原始数据首尾的空格*/
    /*转化数据类型并回传数据信息*/
    switch (nType) {
        case STRINT:/*整型*/
            *(int *)pValue = atoi(szStrbuf);
            break;
        case STRDOUBLE:
            *(double *)pValue = atof(szStrbuf);
            break;
        default:
            strcpy((char *)pValue, szStrbuf);
            break;
    }
    return 0;
}

/*清除字符串首尾的空格， 返回该字符串的首地址，即szDest的传入值
    去除字符串右边空格的方法比较简单，只需将字符串最后一位非空格字符右边（后面）的字符值设定为ASCII码的0即可（字符串默认以0结尾，自动放弃0以后的数据）。而去除字符串左边空格的方法稍微复杂一点，需要先找到字符串的第一位非空字符，再将这个字符移动到字符串的第一位字符，后面的依次类推（使用strcpy完成这一系列操作）。
 */
char * TrimString(char * szDest)
{
    ssize_t n, nLen;
    if (szDest != NULL) {
        /*--------以下删除字符串右边的空格-----*/
        for (n = strlen(szDest); n > 0; n--) {
            if (!isblank(szDest[n - 1])) {
                break;
            }
        }
        szDest[n] = '\0';/*将右边最靠左的一个空格替换为0即可*/
        /*----以下删除字符串左边的空格----*/
        nLen = strlen(szDest);
        for (n = 0; n < nLen; n++) {
            if (!isblank(szDest[n])) {
                break;
            }
        }
        strcopy(szDest, szDest + n);/*从左边第一个非空格起向前移动到串首即可*/
    }
    return szDest;
}

char * strcopy(char * dest, const char * src)
{
    char * destCopy = dest;
    if ((NULL == dest) || (NULL == src)) {
        return NULL;
    }
//    assert((NULL != dest) && (NULL != src));
    while ((* destCopy++ = * src++) != '\0') {
        ;
    }
    return destCopy;
}





