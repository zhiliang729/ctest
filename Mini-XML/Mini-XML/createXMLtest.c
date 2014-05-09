//
//  createXMLtest.c
//  Mini-XML
//
//  Created by zhangliang on 14-5-9.
//  Copyright (c) 2014年 com.gooagoo.Gooagoo. All rights reserved.
//

#include "mxml.h"

/*
 创建 XML 文档
 
 <?xml version="1.0"?>
 <data>
    <node>val1</node>
    <node>val2</node>
    <node>val3</node>
    <group>
        <node>val4</node>
        <node>val5</node>
        <node>val6</node>
    </group>
    <node>val7</node>
    <node>val8</node>
 </data>
 
 可以在内存中创建和更新XML文档，使用mxmlNew 一系列函数
 */


int main(int argc, const char * argv[])
{
    mxml_node_t * xml; /*<?xml ... ?>*/
    mxml_node_t * data; /*<data>*/
    mxml_node_t * node; /*<node>*/
    mxml_node_t * group; /*group*/
/*xml*/
    xml = mxmlNewXML("1.0");
/*data*/
    data = mxmlNewElement(xml, "data");
    /*3个node*/
    node = mxmlNewElement(data, "node");
    mxmlNewText(node, 0, "val1");
    
    node = mxmlNewElement(data, "node");
    mxmlNewText(node, 0, "val2");
    
    node = mxmlNewElement(data, "node");
    mxmlNewText(node, 0, "val3");
    /*group*/
    group = mxmlNewElement(data, "group");
    /*group 下的三个node*/
    node = mxmlNewElement(group, "node");
    mxmlNewText(node, 0, "val4");
    
    node = mxmlNewElement(group, "node");
    mxmlNewText(node, 0, "val5");
    
    node = mxmlNewElement(group, "node");
    mxmlNewText(node, 0, "val6");
    
    /*2个node*/
    node = mxmlNewElement(data, "node");
    mxmlNewText(node, 0, "val7");
    
    node = mxmlNewElement(data, "node");
    mxmlNewText(node, 0, "val8");
    
/*自动折行控制------似乎不顶用！！！！！！*/
    mxmlSetWrapMargin(100);/*设置100列折行*/
    mxmlSetWrapMargin(0);/*取消自动折行*/
    
/*保存*/
    /*保存到文件*/
    FILE * fp;
    fp = fopen("/Users/zhangliang/Desktop/Project/C/Mini-XML/Mini-XML/testxml2.xml", "w");
    mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
    fclose(fp);
    
    
    /*保存到字符串中*/
    char buffer[4096];
    char * ptr;
    
    mxmlSaveString(xml, buffer, sizeof(buffer), MXML_NO_CALLBACK);
    
    ptr = mxmlSaveAllocString(xml, MXML_NO_CALLBACK);
    
    printf("buffer: %s\n", buffer);
    printf("alloc : %s\n", ptr);
    free(ptr);
    
    
    
    return 0;
}

