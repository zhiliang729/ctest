//
//  strParser1.c
//  comlib
//
//  Created by zhangliang on 14-5-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
 固定长度的字符串报文解析
 解析代发工资报文的例子，
    4个数据域，每个数据域的说明、长度和类型分别为：
        1.序号域，长度为3字节，整型；
        2.卡号域，长度为19字节，字符型；
        3.姓名域，长度为6字节，字符型；
        4.资金域，长度为12字节，浮点型。
 
    报文存储在字符数组szCestr中。
 */

char szCestr_1[] = "0019999999999999999999张良1234567.89";

ssize_t nCeLen[4] = {3, 19, 6, 12};/*直接获取固定长度报文中每个数据域的长度*/
int main_str1(int argc, char * argv[])
{
    int nIndex;
    char * nCeAddr[4], szAccno[20], szName[10];
    double fAmt;
    STRRESVAR stuStr;
    stuStr.nAmount = 4;/*数据域总数*/
    stuStr.nFlag = 0;/*固定长度报文*/
    stuStr.filedlen = nCeLen;/*每个数据域的长度*/
    stuStr.nCompartlen = 0;/*分割字符串长度为0*/
    stuStr.szCompart[0] = 0;/*无分割字符串*/
    stuStr.filedaddr = nCeAddr;/*为存储数据域的首地址分配缓冲区*/
    strrespre(szCestr_1, &stuStr);/*准备报文解析结构*/
    /*获取报文数据信息*/
    strresvalue(szCestr_1, stuStr, 0, &nIndex, STRINT);
    strresvalue(szCestr_1, stuStr, 1, szAccno, STRSTR);
    strresvalue(szCestr_1, stuStr, 2, szName, STRSTR);
    strresvalue(szCestr_1, stuStr, 3, &fAmt, STRDOUBLE);
    /*打印报文数据信息*/
    PrintLog(stderr, "id = %d", nIndex);
    PrintLog(stderr, "accno = [%d]%s", strlen(szAccno), szAccno);
    PrintLog(stderr, "name = [%d]%s", strlen(szName), szName);
    PrintLog(stderr, "amt = %.2f", fAmt);
    
    return 0;
}





