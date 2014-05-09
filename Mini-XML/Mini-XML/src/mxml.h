/*
 * "$Id: mxml.h 451 2014-01-04 21:50:06Z msweet $"
 *
 * Header file for Mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003-2014 by Michael R Sweet.
 *
 * These coded instructions, statements, and computer programs are the
 * property of Michael R Sweet and are protected by Federal copyright
 * law.  Distribution and use rights are outlined in the file "COPYING"
 * which should have been included with this file.  If this file is
 * missing or damaged, see the license at:
 *
 *     http://www.msweet.org/projects.php/Mini-XML
 */

#pragma mark - **********************************防止重复包含
/*
 * Prevent multiple inclusion... 防止重复包含
 */

#ifndef _mxml_h_
#define _mxml_h_

#pragma mark - **********************************头文件包含
/*
 * Include necessary headers... 必要的头文件包含
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#pragma mark - ***************常量宏定义
/*
 * Constants... 常量
 */

#pragma mark - **********************************版本号
#define MXML_MAJOR_VERSION	2	/* Major version number */
#define MXML_MINOR_VERSION	8	/* Minor version number */

#define MXML_TAB		8	/* Tabs every N columns */

#define MXML_NO_CALLBACK	0	/* Don't use a type callback */
#define MXML_INTEGER_CALLBACK	mxml_integer_cb
/* Treat all data as integers 把所有数据当做整型*/
#define MXML_OPAQUE_CALLBACK	mxml_opaque_cb
/* Treat all data as opaque 把所有数据当做不透明数据？*/
#define MXML_REAL_CALLBACK	mxml_real_cb
/* Treat all data as real numbers 把所有数据当做实数*/
#define MXML_TEXT_CALLBACK	0	/* Treat all data as text 把所有的数据作为text*/
#define MXML_IGNORE_CALLBACK	mxml_ignore_cb
/* Ignore all non-element content 忽略所有非元素内容*/

#define MXML_NO_PARENT	0	/* No parent for the node 没有父节点*/

#define MXML_DESCEND		1	/* Descend when finding/walking 向下查找/遍历*/
#define MXML_NO_DESCEND	0	/* Don't descend when finding/walking 不要向下查找/遍历*/
#define MXML_DESCEND_FIRST	-1	/* Descend for first find 首次向下查找/遍历？*/

#define MXML_WS_BEFORE_OPEN	0	/* Callback for before open tag 在开始标签之前Callback*/
#define MXML_WS_AFTER_OPEN	1	/* Callback for after open tag 在开始标签之后Callback*/
#define MXML_WS_BEFORE_CLOSE	2	/* Callback for before close tag 在关闭标签之前Callback*/
#define MXML_WS_AFTER_CLOSE	3	/* Callback for after close tag 在关闭标签之后Callback*/

#define MXML_ADD_BEFORE	0	/* Add node before specified node   添加到节点之前*/
#define MXML_ADD_AFTER	1	/* Add node after specified node  添加到节点之后*/
#define MXML_ADD_TO_PARENT	NULL	/* Add node relative to parent  添加到父节点*/


#pragma mark - **********************************数据类型
/*
 * Data types...
 */

typedef enum mxml_sax_event_e		/**** SAX event type. ****/
{
    MXML_SAX_CDATA,			/* CDATA node */
    MXML_SAX_COMMENT,			/* Comment node */
    MXML_SAX_DATA,			/* Data node */
    MXML_SAX_DIRECTIVE,			/* Processing directive node */
    MXML_SAX_ELEMENT_CLOSE,		/* Element closed */
    MXML_SAX_ELEMENT_OPEN			/* Element opened */
} mxml_sax_event_t;

typedef enum mxml_type_e		/**** The XML node type. ****/
{
    MXML_IGNORE = -1,			/* Ignore/throw away node @since Mini-XML 2.3@ 被忽略的节点*/
    MXML_ELEMENT,				/* XML element with attributes 还是xml文件*/
    MXML_INTEGER,				/* Integer value  整型*/
    MXML_OPAQUE,				/* Opaque string 不透明字符串*/
    MXML_REAL,				/* Real value 浮点数*/
    MXML_TEXT,				/* Text fragment 文本*/
    MXML_CUSTOM				/* Custom data @since Mini-XML 2.1@ 自定义数据*/
} mxml_type_t;

typedef void (*mxml_custom_destroy_cb_t)(void *);
/**** Custom data destructor 自定义数据析构函数****/

typedef void (*mxml_error_cb_t)(const char *);
/**** Error callback function 错误回调函数****/

typedef struct mxml_attr_s		/**** An XML element attribute value. @private@ 元素的属性值 ****/
{
    char			*name;		/* Attribute name 属性名称*/
    char			*value;		/* Attribute value 属性值*/
} mxml_attr_t;

typedef struct mxml_element_s		/**** An XML element value. @private@ 元素值****/
{
    char			*name;		/* Name of element 元素名称*/
    int			num_attrs;	/* Number of attributes 元素的属性个数*/
    mxml_attr_t		*attrs;		/* Attributes 元素的属性数组指针*/
} mxml_element_t;

typedef struct mxml_text_s		/**** An XML text value. @private@ 文本值****/
{
    int			whitespace;	/* Leading whitespace 开始的空格数*/
    char			*string;	/* Fragment string 字符串片段*/
} mxml_text_t;

typedef struct mxml_custom_s		/**** An XML custom value. @private@ 自定义值****/
{
    void			*data;		/* Pointer to (allocated) custom data 指向自定义数据的指针*/
    mxml_custom_destroy_cb_t destroy;	/* Pointer to destructor function 指向析构函数的指针*/
} mxml_custom_t;

typedef union mxml_value_u		/**** An XML node value. @private@ 节点值的union（类型，选其一） ****/
{
    mxml_element_t	element;	/* Element 节点元素*/
    int			integer;	/* Integer number 整型值*/
    char			*opaque;	/* Opaque string 模糊字符串*/
    double		real;		/* Real number 实数*/
    mxml_text_t		text;		/* Text fragment 文本片段*/
    mxml_custom_t		custom;		/* Custom data @since Mini-XML 2.1@ 自定义数据*/
} mxml_value_t;

struct mxml_node_s			/**** An XML node. @private@ 节点 ****/
{
    mxml_type_t		type;		/* Node type 节点类型*/
    struct mxml_node_s	*next;		/* Next node under same parent 同一父节点下的下一个节点*/
    struct mxml_node_s	*prev;		/* Previous node under same parent 同一父节点下的上一个节点*/
    struct mxml_node_s	*parent;	/* Parent node 父节点*/
    struct mxml_node_s	*child;		/* First child node 首个子节点*/
    struct mxml_node_s	*last_child;	/* Last child node 最后一个子节点*/
    mxml_value_t		value;		/* Node value 节点值*/
    int			ref_count;	/* Use count 使用计数*/
    void			*user_data;	/* User data 用户数据*/
};

typedef struct mxml_node_s mxml_node_t;	/**** An XML node. 重命名类型****/

struct mxml_index_s			 /**** An XML node index. @private@ 节点索引 ****/
{
    char			*attr;		/* Attribute used for indexing or NULL 索引所使用的属性*/
    int			num_nodes;	/* Number of nodes in index 索引中的节点数*/
    int			alloc_nodes;	/* Allocated nodes in index 索引中分配的节点数*/
    int			cur_node;	/* Current node 当前节点*/
    mxml_node_t		**nodes;	/* Node array 节点数组*/
};

typedef struct mxml_index_s mxml_index_t;
/**** An XML node index. ****/

typedef int (*mxml_custom_load_cb_t)(mxml_node_t *, const char *);
/**** Custom data load callback function 自定义数据加载回调函数****/

typedef char *(*mxml_custom_save_cb_t)(mxml_node_t *);
/**** Custom data save callback function 组定义数据保存回调函数****/

typedef int (*mxml_entity_cb_t)(const char *);
/**** Entity callback function 入口回调函数*/

typedef mxml_type_t (*mxml_load_cb_t)(mxml_node_t *);
/**** Load callback function 加载回调函数****/

typedef const char *(*mxml_save_cb_t)(mxml_node_t *, int);
/**** Save callback function 保存回调函数****/

typedef void (*mxml_sax_cb_t)(mxml_node_t *, mxml_sax_event_t, void *);
/**** SAX callback function SAX回调****/

#pragma mark - ***********************  C++支持
/*
 * C++ support...
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /*
     * Prototypes...
     */
    
    extern void		mxmlAdd(mxml_node_t *parent, int where,
                            mxml_node_t *child, mxml_node_t *node);
    extern void		mxmlDelete(mxml_node_t *node);
    extern void		mxmlElementDeleteAttr(mxml_node_t *node,
                                          const char *name);
    extern const char	*mxmlElementGetAttr(mxml_node_t *node, const char *name);
    extern void		mxmlElementSetAttr(mxml_node_t *node, const char *name,
                                       const char *value);
    extern void		mxmlElementSetAttrf(mxml_node_t *node, const char *name,
                                        const char *format, ...)
#ifdef __GNUC__
    __attribute__ ((__format__ (__printf__, 3, 4)))
#endif /* __GNUC__ */
    ;
    extern int		mxmlEntityAddCallback(mxml_entity_cb_t cb);
    extern const char	*mxmlEntityGetName(int val);
    extern int		mxmlEntityGetValue(const char *name);
    extern void		mxmlEntityRemoveCallback(mxml_entity_cb_t cb);
    extern mxml_node_t	*mxmlFindElement(mxml_node_t *node, mxml_node_t *top,
                                         const char *name, const char *attr,
                                         const char *value, int descend);
    extern mxml_node_t	*mxmlFindPath(mxml_node_t *node, const char *path);
    extern const char	*mxmlGetCDATA(mxml_node_t *node);
    extern const void	*mxmlGetCustom(mxml_node_t *node);
    extern const char	*mxmlGetElement(mxml_node_t *node);
    extern mxml_node_t	*mxmlGetFirstChild(mxml_node_t *node);
    extern int		mxmlGetInteger(mxml_node_t *node);
    extern mxml_node_t	*mxmlGetLastChild(mxml_node_t *node);
    extern mxml_node_t	*mxmlGetNextSibling(mxml_node_t *node);
    extern const char	*mxmlGetOpaque(mxml_node_t *node);
    extern mxml_node_t	*mxmlGetParent(mxml_node_t *node);
    extern mxml_node_t	*mxmlGetPrevSibling(mxml_node_t *node);
    extern double		mxmlGetReal(mxml_node_t *node);
    extern int		mxmlGetRefCount(mxml_node_t *node);
    extern const char	*mxmlGetText(mxml_node_t *node, int *whitespace);
    extern mxml_type_t	mxmlGetType(mxml_node_t *node);
    extern void		*mxmlGetUserData(mxml_node_t *node);
    extern void		mxmlIndexDelete(mxml_index_t *ind);
    extern mxml_node_t	*mxmlIndexEnum(mxml_index_t *ind);
    extern mxml_node_t	*mxmlIndexFind(mxml_index_t *ind,
                                       const char *element,
                                       const char *value);
    extern int		mxmlIndexGetCount(mxml_index_t *ind);
    extern mxml_index_t	*mxmlIndexNew(mxml_node_t *node, const char *element,
                                      const char *attr);
    extern mxml_node_t	*mxmlIndexReset(mxml_index_t *ind);
    extern mxml_node_t	*mxmlLoadFd(mxml_node_t *top, int fd,
                                    mxml_type_t (*cb)(mxml_node_t *));
    extern mxml_node_t	*mxmlLoadFile(mxml_node_t *top, FILE *fp,
                                      mxml_type_t (*cb)(mxml_node_t *));
    extern mxml_node_t	*mxmlLoadString(mxml_node_t *top, const char *s,
                                        mxml_type_t (*cb)(mxml_node_t *));
    extern mxml_node_t	*mxmlNewCDATA(mxml_node_t *parent, const char *string);
    extern mxml_node_t	*mxmlNewCustom(mxml_node_t *parent, void *data,
                                       mxml_custom_destroy_cb_t destroy);
    extern mxml_node_t	*mxmlNewElement(mxml_node_t *parent, const char *name);
    extern mxml_node_t	*mxmlNewInteger(mxml_node_t *parent, int integer);
    extern mxml_node_t	*mxmlNewOpaque(mxml_node_t *parent, const char *opaque);
    extern mxml_node_t	*mxmlNewReal(mxml_node_t *parent, double real);
    extern mxml_node_t	*mxmlNewText(mxml_node_t *parent, int whitespace,
                                     const char *string);
    extern mxml_node_t	*mxmlNewTextf(mxml_node_t *parent, int whitespace,
                                      const char *format, ...)
#ifdef __GNUC__
    __attribute__ ((__format__ (__printf__, 3, 4)))
#endif /* __GNUC__ */
    ;
    extern mxml_node_t	*mxmlNewXML(const char *version);
    extern int		mxmlRelease(mxml_node_t *node);
    extern void		mxmlRemove(mxml_node_t *node);
    extern int		mxmlRetain(mxml_node_t *node);
    extern char		*mxmlSaveAllocString(mxml_node_t *node,
                                         mxml_save_cb_t cb);
    extern int		mxmlSaveFd(mxml_node_t *node, int fd,
                               mxml_save_cb_t cb);
    extern int		mxmlSaveFile(mxml_node_t *node, FILE *fp,
                                 mxml_save_cb_t cb);
    extern int		mxmlSaveString(mxml_node_t *node, char *buffer,
                                   int bufsize, mxml_save_cb_t cb);
    extern mxml_node_t	*mxmlSAXLoadFd(mxml_node_t *top, int fd,
                                       mxml_type_t (*cb)(mxml_node_t *),
                                       mxml_sax_cb_t sax, void *sax_data);
    extern mxml_node_t	*mxmlSAXLoadFile(mxml_node_t *top, FILE *fp,
                                         mxml_type_t (*cb)(mxml_node_t *),
                                         mxml_sax_cb_t sax, void *sax_data);
    extern mxml_node_t	*mxmlSAXLoadString(mxml_node_t *top, const char *s,
                                           mxml_type_t (*cb)(mxml_node_t *),
                                           mxml_sax_cb_t sax, void *sax_data);
    extern int		mxmlSetCDATA(mxml_node_t *node, const char *data);
    extern int		mxmlSetCustom(mxml_node_t *node, void *data,
                                  mxml_custom_destroy_cb_t destroy);
    extern void		mxmlSetCustomHandlers(mxml_custom_load_cb_t load,
                                          mxml_custom_save_cb_t save);
    extern int		mxmlSetElement(mxml_node_t *node, const char *name);
    extern void		mxmlSetErrorCallback(mxml_error_cb_t cb);
    extern int		mxmlSetInteger(mxml_node_t *node, int integer);
    extern int		mxmlSetOpaque(mxml_node_t *node, const char *opaque);
    extern int		mxmlSetReal(mxml_node_t *node, double real);
    extern int		mxmlSetText(mxml_node_t *node, int whitespace,
                                const char *string);
    extern int		mxmlSetTextf(mxml_node_t *node, int whitespace,
                                 const char *format, ...)
#ifdef __GNUC__
    __attribute__ ((__format__ (__printf__, 3, 4)))
#endif /* __GNUC__ */
    ;
    extern int		mxmlSetUserData(mxml_node_t *node, void *data);
    extern void		mxmlSetWrapMargin(int column);
    extern mxml_node_t	*mxmlWalkNext(mxml_node_t *node, mxml_node_t *top,
                                      int descend);
    extern mxml_node_t	*mxmlWalkPrev(mxml_node_t *node, mxml_node_t *top,
                                      int descend);
    
    
    /*
     * Semi-private functions...
     */
    
    extern void		mxml_error(const char *format, ...);
    extern mxml_type_t	mxml_ignore_cb(mxml_node_t *node);
    extern mxml_type_t	mxml_integer_cb(mxml_node_t *node);
    extern mxml_type_t	mxml_opaque_cb(mxml_node_t *node);
    extern mxml_type_t	mxml_real_cb(mxml_node_t *node);
    
    
    /*
     * C++ support...
     */
    
#ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_mxml_h_ */


/*
 * End of "$Id: mxml.h 451 2014-01-04 21:50:06Z msweet $".
 */
