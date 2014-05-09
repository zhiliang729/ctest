//
//  main.c
//  Mini-XML
//
//  Created by zhangliang on 14-5-8.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "mxml.h"

int main_1(int argc, const char * argv[])
{
    mxml_node_t *tree, *node;
    
    FILE *fp;
    fp = fopen("/Users/zhangliang/Desktop/Project/C/Mini-XML/Mini-XML/testxml.xml", "r");/*打开xml文件*/
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);/*读取xml文件到xml树*/
    fclose(fp);/*关闭文件*/
    
/*解析xml文件*/
    mxml_node_t *id,*password;
    /*解析note节点及属性*/
    node = mxmlFindElement(tree, tree, "note",NULL, NULL,MXML_DESCEND);
    printf(" year:%s \n",mxmlElementGetAttr(node,"year"));
    printf(" date:%s \n",mxmlElementGetAttr(node,"date"));
    printf(" month:%s \n",mxmlElementGetAttr(node,"month"));
    
    /*解析id节点*/
    id = mxmlFindElement(node, tree, "id",NULL, NULL,MXML_DESCEND);
    printf("[%s]\n",id->child->value.text.string);
    
    /*解析password节点*/
    password = mxmlFindElement(node, tree, "password",NULL, NULL,MXML_DESCEND);
    printf("[%s]\n",password->child->value.text.string);
    
/*释放内存*/
    mxmlDelete(tree);
    
    return 0;
}

