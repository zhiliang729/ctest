//
//  strParser2.c
//  comlib
//
//  Created by zhangliang on 14-5-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
 固定分割的字符串报文解析
 解析代发工资报文的例子，
 4个数据域，每个数据域的说明、长度和类型分别为：
 1.序号域，长度为3字节，整型；
 2.卡号域，长度为19字节，字符型；
 3.姓名域，长度为6字节，字符型；
 4.资金域，长度为12字节，浮点型。
 
 报文存储在字符数组szCestr中。
 */

char szCestr[] = "1|!9999999999999999999|!张良|!1234567.89";
int main(int argc, char * argv[])
{
    int nIndex;
    ssize_t nCeLen[4];/*无法直接获取每个数据域的长度*/
    char * nCeAddr[4], szAccno[20], szName[10];
    double fAmt;
    STRRESVAR stuStr;
    stuStr.nAmount = 4;/*数据域总数*/
    stuStr.nFlag = 1;/*固定分割类型*/
    stuStr.filedlen = nCeLen;/*为存储数据域的长度分配缓冲区*/
    stuStr.nCompartlen = 2;/*分割字符串的长度2*/
    strcpy(stuStr.szCompart, "|!");/*分割字符串*/
    stuStr.filedaddr = nCeAddr;/*为存储数据域的首地址分配缓冲区*/
    strrespre(szCestr, &stuStr);/*准备报文解析结构*/
    /*获取报文数据信息*/
    strresvalue(szCestr, stuStr, 0, &nIndex, STRINT);
    strresvalue(szCestr, stuStr, 1, szAccno, STRSTR);
    strresvalue(szCestr, stuStr, 2, szName, STRSTR);
    strresvalue(szCestr, stuStr, 3, &fAmt, STRDOUBLE);
    /*打印报文数据信息*/
    PrintLog(stderr, "id = %d", nIndex);
    PrintLog(stderr, "accno = [%d]%s", strlen(szAccno), szAccno);
    PrintLog(stderr, "name = [%d]%s", strlen(szName), szName);
    PrintLog(stderr, "amt = %.2f", fAmt);
}

