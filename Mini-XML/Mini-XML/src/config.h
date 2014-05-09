/*
 * "$Id: config.h 451 2014-01-04 21:50:06Z msweet $"
 *
 * Configuration file for Mini-XML, a small XML-like file parsing library.
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

#pragma mark - 一些说明
/*
 Mini-XML中的节点类型定义和其他有些解析器有些不同，其中整数、浮点、和文本节点是指在一个XML元素中一系列的使用空格作为分割的值，每个元素可以拥有多个以上节点，并可以选择使用空格分开，如：<abc>aa bb cc</abc>，Mini-MXML在使用参数：MXML_TEXT_CALLBACK进行载入时，将在abc元素下面生成3个text类 型的子节点。在创建时也可以使用同样的方式创建节点。整数和浮点也是同样方式，但如果转换失败则MiniXML报错。而不透明字符串类型(OPAQUE) 则不进行字符串分割，在载入时需要使用MXML_OPAQUE_CALLBACK参数，将所有字符串形成一个子节点。详情见：使用加载回调函数。每一个节点总是有一个成员变量:user_data 可以允许你为每一个节点关联你需要的应用数据.
 新的节点可以使用以下函数进行创建 mxmlNewElement, mxmlNewInteger, mxmlNewOpaque, mxmlNewReal, mxmlNewText mxmlNewTextf mxmlNewXML . 只有 elements 可以拥有子节点，顶级节点必须是一个 element , 通常是<?xml version="1.0"?> 使用mxmlNewXML()函数创建的节点.
 每个节点都有一些关联节点的指针，上(parent), 下(child), 左(prev), and 右(next) 相对应于当前节点. 
 当你使用完毕这些XML数据后，使用函数mxmlDelete 来释放指定节点或者整个XML树节点和它下面所有子节点的内存。
*/

/*
 * Include necessary headers...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


/*
 * Version number...
 */

#define MXML_VERSION	""


/*
 * Inline function support...
 */

#define inline


/*
 * Long long support...
 */

#undef HAVE_LONG_LONG


/*
 * Do we have the snprintf() and vsnprintf() functions?
 */

#define HAVE_SNPRINTF
#define HAVE_VSNPRINTF


/*
 * Do we have the strXXX() functions?
 */

#define HAVE_STRDUP


/*
 * Do we have threading support?
 */

#define HAVE_PTHREAD_H


/*
 * Define prototypes for string functions as needed...
 */

#  ifndef HAVE_STRDUP
extern char	*_mxml_strdup(const char *);
#    define strdup _mxml_strdup
#  endif /* !HAVE_STRDUP */

extern char	*_mxml_strdupf(const char *, ...);
extern char	*_mxml_vstrdupf(const char *, va_list);

#  ifndef HAVE_SNPRINTF
extern int	_mxml_snprintf(char *, size_t, const char *, ...);
#    define snprintf _mxml_snprintf
#  endif /* !HAVE_SNPRINTF */

#  ifndef HAVE_VSNPRINTF
extern int	_mxml_vsnprintf(char *, size_t, const char *, va_list);
#    define vsnprintf _mxml_vsnprintf
#  endif /* !HAVE_VSNPRINTF */

/*
 * End of "$Id: config.h 451 2014-01-04 21:50:06Z msweet $".
 */
