//
//  configINI.c
//  comlib
//
//  Created by zhangliang on 14-5-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "comlib.h"

/*
 本函数打开INI配置文件szPath（带路径的文件名），读取szRoot数据域中szName配置项的配置信息。参数pValue指向一个缓冲区，
 它回传配置项数据，这个数据项是数据类型成功转换后的取值。参数nType是配置项数据的类型，当前所支持的数据类型为int， char double。成功时返回0，否则返回非0值。
 
 INI类型配置文件由多行字符串组成，这些字符串可分为三种类型：
 1.数据域。由“[”开始，由“]”结尾，在中括号中间的就是数据域，它是配置项的分类
 2.配置项。字符串按照“配置项名称=配置项取值”的格式，相当于一个以“=”分隔的固定分隔字符串，它只有两个数据域，第一个是配置项名称，第二个是配置项的数据。
 3.注释行。行首字符是“#”的就是注释行，读取INI格式配置文件时应该忽略之。
 
 （1）在配置文件中，查找配置项字符串：
 1.1. 在配置文件中找到数据域为szRoot的行，操作方法是判断行首是否与字符串[szRoot]匹配。
 1.2. 从该数据域所管辖的范围内，查找配置项szName行，操作方法是判断行首是否与字符串“[szName]”匹配。
 1.3. 如果查找过程中遇到了注释行，则跳过。
 1.4. 如果查找过程中超过了文件尾或者第1.2步匹配到了另一个数据域，说明配置文件中没有要求的配置项，函数返回
 */
int GetINIConfigValue(char * szPath, char * szRoot, char * szName, void * pValue, int nType)
{
    FILE * fp;
    int nFlag = 0;/*进入数据域的标志*/
    char buf[1024], szRootExt[128], szNameExt[128];
    ASSERT((fp = fopen(szPath, "r")) != NULL);
    sprintf(szRootExt, "[%s]", szRoot);/*数据域"[szRoot]"格式*/
    sprintf(szNameExt, "%s=", szName);/*数据项“szName=”格式*/
    
    /*
     检测流上的文件结束符
     The function feof() tests the end-of-file indicator for the stream
     pointed to by stream, returning non-zero if it is set. The end-of-file indicator can only be cleared by the function clearerr().
     如果文件结束，则返回非0值，否则返回0，文件结束符只能被clearerr()清除。
     
     feof(fp)有两个返回值:如果遇到文件结束，函数feof（fp）的值为非零值，否则为0。
     EOF是文本文件结束的标志。在文本文件中，数据是以字符的ASCⅡ代码值的形式存放，普通字符的ASCⅡ代码的范围是32到127（十进制），EOF的16进制代码为0x1A（十进制为26），因此可以用EOF作为文件结束标志。[1]
     当把数据以二进制形式存放到文件中时，就会有-1值的出现，因此不能采用EOF作为二进制文件的结束标志。为解决这一个问题，ASCI C提供一个feof函数，用来判断文件是否结束。feof函数既可用以判断二进制文件又可用以判断文本文件。*/
    
    /*
     函数原型
     
     char *fgets(char *buf, int bufsize, FILE *stream);
     参数
     
     *buf: 字符型指针，指向用来存储所得数据的地址。
     bufsize: 整型数据，指明存储数据的大小。
     *stream: 文件结构体指针，将要读取的文件流。
     返回值
     
     成功，则返回第一个参数buf；
     在读字符时遇到end-of-file，则eof指示器被设置，如果还没读入任何字符就遇到这种情况，则buf保持原来的内容，返回NULL；
     如果发生读入错误，error指示器被设置，返回NULL，buf的值可能被改变。
     注意：读取时，连带行尾的'\n'也读入了，故若要进行字符串的比较等操作，必须检查最后一个字符，若为‘\n’，则替换为‘\0’;
     */
    while (feof(fp) == 0) {/*文件未结束*/
//        PrintLog(stderr, "len: %ld", strlen(buf));//strlen计算的是字符串的长度，长度有可能各种值。
//        PrintLog(stderr, "sizeof : %ld", sizeof(buf));//sizeof计算的事实数组的长度，故buf数组总为1024
        memset(buf, 0, sizeof(buf));/*因为要清零整个缓冲区，所以用sizeof*/
        fgets(buf, sizeof(buf), fp);/*读取文件中的每一行字符串， 最多读取buf的长度个字符*/
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = '\0';
        }
        if (buf[0] == '#') {/*注释行，跳过*/
            continue;
        }
        if (nFlag == 0 && buf[0] != '[') {/*未进入数据域时，只查询数据域行*/
            continue;
        }else if(nFlag == 0 && buf[0] == '['){/*未进入数据域，当前处于数据域行*/
            if (strncmp(buf, szRootExt, strlen(szRootExt)) == 0) {/*匹配当前数据域是否为所求域，若是，设置标志， 字符串对比时，对比的是字符串的有效字符，所以不能用sizeof，要用strlen*/
                nFlag = 1;
            }
        }else if(nFlag == 1 && buf[0] == '['){/*匹配到了另一个数据域，即所要求的数据域遍历完毕*/
            break;
        }else {/* 已进入数据域, 遍历数据域中的配置项*/
            if (strncmp(buf, szNameExt, strlen(szNameExt)) == 0) {/*匹配当前配置项是否为所求项*/
                
                /*从配置项中分解配置取值*/
                ASSERT(GetINIConfigStrValue(buf, pValue, nType) == 0);
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    return -1;
}

/*解析配置项文件之INI格式文件配置项解析
 配置项字符串是一个以“=”分隔的，共有两个数据域的固定分隔字符串报文。下面函数解析该串报文并按照指定的数据类型返回配置项的取值。函数解析字符串报文buf并按照参数nType指定的数据类型返回数据域取值到缓冲区pValue中。函数调用成功时返回0，否则返回非0值。
 */
int GetINIConfigStrValue(char * buf, void * pValue, int nType)
{
    char * nCeAddr[2];
    ssize_t nCeLen[2];
    STRRESVAR stuStr;
    stuStr.nAmount = 2;/*数据域总数*/
    stuStr.nFlag = 1;/*固定分隔*/
    stuStr.filedlen = nCeLen;/*每个域的长度*/
    stuStr.nCompartlen = 1;/*分隔符号的长度*/
    strcpy(stuStr.szCompart, "=");/*分隔字符串*/
    stuStr.filedaddr = nCeAddr;/*指向每个域首的指针*/
    strrespre(buf, &stuStr);/*解析字符串报文*/
    ASSERT(strresvalue(buf, stuStr, 1, pValue, nType) == 0);
    return 0;
}


