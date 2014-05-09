/*
 * "$Id: mxml-private.c 451 2014-01-04 21:50:06Z msweet $"
 *
 * Private functions for Mini-XML, a small XML-like file parsing library.
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

/*
 * Include necessary headers...
 */

#include "mxml-private.h"


/*
 * Some crazy people think that unloading a shared object is a good or safe
 * thing to do.  Unfortunately, most objects are simply *not* safe to unload
 * and bad things *will* happen.
 *
 * The following mess of conditional code allows us to provide a destructor
 * function in Mini-XML for our thread-global storage so that it can possibly
 * be unloaded safely, although since there is no standard way to do so I
 * can't even provide any guarantees that you can do it safely on all platforms.
 *
 * This code currently supports AIX, HP-UX, Linux, Mac OS X, Solaris, and
 * Windows.  It might work on the BSDs and IRIX, but I haven't tested that.
 */

#if defined(__sun) || defined(_AIX)
#  pragma fini(_mxml_fini)
#  define _MXML_FINI _mxml_fini
#elif defined(__hpux)
#  pragma FINI _mxml_fini
#  define _MXML_FINI _mxml_fini
#elif defined(__GNUC__) /* Linux and Mac OS X */
#  define _MXML_FINI __attribute((destructor)) _mxml_fini
#else
#  define _MXML_FINI _fini
#endif /* __sun */

#pragma mark - ***************** 展示错误信息
/*
 * 'mxml_error()' - Display an error message.
 */

void
mxml_error(const char *format,		/* I - Printf-style format string */
...)				/* I - Additional arguments as needed */
{
    va_list	ap;			/* Pointer to arguments */
    char		s[1024];		/* Message string */
    _mxml_global_t *global = _mxml_global();
    /* Global data */
    
    
    /*
     * Range check input...
     */
    
    if (!format)
        return;
    
    /*
     * Format the error message string...
     */
    
    va_start(ap, format);
    
    vsnprintf(s, sizeof(s), format, ap);
    
    va_end(ap);
    
    /*
     * And then display the error message...
     */
    
    if (global->error_cb)
        (*global->error_cb)(s);
    else
        fprintf(stderr, "mxml: %s\n", s);
}

#pragma mark - ***************** 忽略值的默认回调
/*
 * 'mxml_ignore_cb()' - Default callback for ignored values.
 */

mxml_type_t				/* O - Node type */
mxml_ignore_cb(mxml_node_t *node)	/* I - Current node */
{
    (void)node;
    
    return (MXML_IGNORE);
}

#pragma mark - ***************** 整型值的默认回调
/*
 * 'mxml_integer_cb()' - Default callback for integer values.
 */

mxml_type_t				/* O - Node type */
mxml_integer_cb(mxml_node_t *node)	/* I - Current node */
{
    (void)node;
    
    return (MXML_INTEGER);
}

#pragma mark - ***************** 模糊值的默认回调
/*
 * 'mxml_opaque_cb()' - Default callback for opaque values.
 */

mxml_type_t				/* O - Node type */
mxml_opaque_cb(mxml_node_t *node)	/* I - Current node */
{
    (void)node;
    
    return (MXML_OPAQUE);
}

#pragma mark - ***************** 实数值的默认回调
/*
 * 'mxml_real_cb()' - Default callback for real number values.
 */

mxml_type_t				/* O - Node type */
mxml_real_cb(mxml_node_t *node)		/* I - Current node */
{
    (void)node;
    
    return (MXML_REAL);
}

#pragma mark - ***************** 多线程操作
#ifdef HAVE_PTHREAD_H			/**** POSIX threading ****/
#include <pthread.h>

static pthread_key_t	_mxml_key = -1;	/* Thread local storage key */
static pthread_once_t	_mxml_key_once = PTHREAD_ONCE_INIT;
/* One-time initialization object */
static void		_mxml_init(void);
static void		_mxml_destructor(void *g);


#pragma mark - 释放内存
/*
 * '_mxml_destructor()' - Free memory used for globals...
 */

static void
_mxml_destructor(void *g)		/* I - Global data */
{
    free(g);
}

#pragma mark - 加载完毕时清除
/*
 * '_mxml_fini()' - Clean up when unloaded.
 */

static void
_MXML_FINI(void)
{
    _mxml_global_t	*global;	/* Global data */
    
    
    if (_mxml_key != -1)
    {
        if ((global = (_mxml_global_t *)pthread_getspecific(_mxml_key)) != NULL)
            _mxml_destructor(global);
        
        pthread_key_delete(_mxml_key);
        _mxml_key = -1;
    }
}

#pragma mark - 得到全局数据
/*
 * '_mxml_global()' - Get global data.
 */

_mxml_global_t *			/* O - Global data */
_mxml_global(void)
{
    _mxml_global_t	*global;	/* Global data */
    
    
    pthread_once(&_mxml_key_once, _mxml_init);
    
    if ((global = (_mxml_global_t *)pthread_getspecific(_mxml_key)) == NULL)
    {
        global = (_mxml_global_t *)calloc(1, sizeof(_mxml_global_t));
        pthread_setspecific(_mxml_key, global);
        
        global->num_entity_cbs = 1;
        global->entity_cbs[0]  = _mxml_entity_cb;
        global->wrap           = 72;
    }
    
    return (global);
}

#pragma mark - 初始化全局数据
/*
 * '_mxml_init()' - Initialize global data...
 */

static void
_mxml_init(void)
{
    pthread_key_create(&_mxml_key, _mxml_destructor);
}


#elif defined(WIN32) && defined(MXML1_EXPORTS) /**** WIN32 threading ****/
#  include <windows.h>

static DWORD _mxml_tls_index;		/* Index for global storage */


/*
 * 'DllMain()' - Main entry for library.
 */

BOOL WINAPI				/* O - Success/failure */
DllMain(HINSTANCE hinst,		/* I - DLL module handle */
        DWORD     reason,		/* I - Reason */
        LPVOID    reserved)		/* I - Unused */
{
    _mxml_global_t	*global;	/* Global data */
    
    
    (void)hinst;
    (void)reserved;
    
    switch (reason)
    {
        case DLL_PROCESS_ATTACH :		/* Called on library initialization */
            if ((_mxml_tls_index = TlsAlloc()) == TLS_OUT_OF_INDEXES)
                return (FALSE);
            break;
            
        case DLL_THREAD_DETACH :		/* Called when a thread terminates */
            if ((global = (_mxml_global_t *)TlsGetValue(_mxml_tls_index)) != NULL)
                free(global);
            break;
            
        case DLL_PROCESS_DETACH :		/* Called when library is unloaded */
            if ((global = (_mxml_global_t *)TlsGetValue(_mxml_tls_index)) != NULL)
                free(global);
            
            TlsFree(_mxml_tls_index);
            break;
            
        default:
            break;
    }
    
    return (TRUE);
}


/*
 * '_mxml_global()' - Get global data.
 */

_mxml_global_t *			/* O - Global data */
_mxml_global(void)
{
    _mxml_global_t	*global;	/* Global data */
    
    
    if ((global = (_mxml_global_t *)TlsGetValue(_mxml_tls_index)) == NULL)
    {
        global = (_mxml_global_t *)calloc(1, sizeof(_mxml_global_t));
        
        global->num_entity_cbs = 1;
        global->entity_cbs[0]  = _mxml_entity_cb;
        global->wrap           = 72;
        
        TlsSetValue(_mxml_tls_index, (LPVOID)global);
    }
    
    return (global);
}


#else					/**** No threading ****/
/*
 * '_mxml_global()' - Get global data.
 */

_mxml_global_t *			/* O - Global data */
_mxml_global(void)
{
    static _mxml_global_t	global =	/* Global data */
    {
        NULL,				/* error_cb */
        1,					/* num_entity_cbs */
        { _mxml_entity_cb },		/* entity_cbs */
        72,					/* wrap */
        NULL,				/* custom_load_cb */
        NULL				/* custom_save_cb */
    };
    
    
    return (&global);
}
#endif /* HAVE_PTHREAD_H */


/*
 * End of "$Id: mxml-private.c 451 2014-01-04 21:50:06Z msweet $".
 */