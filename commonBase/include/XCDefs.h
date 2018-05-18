/*
******************************************
Copyright (C): XC
Filename : XCDEFS.H
Author :  hudaixin
Description: 通用宏定义
********************************************
*/

#ifndef _XC_H_XCDEFS_H_
#define _XC_H_XCDEFS_H_

#include "XCTypes.h"
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#ifdef _MSWINDOWS_
#include <process.h>
#include <string.h>
#else
#include <XCString.h>
#endif

#define xc_sprintf sprintf

#define xc_fprintf fprintf

#define xc_printf printf

#ifdef _MSWINDOWS_
//#define xc_snprintf _snprintf
#define  xc_snprintf(xbuf, xbufsize , xfmt, ... ) _snprintf_s(xbuf,xbufsize, _TRUNCATE,xfmt, __VA_ARGS__)

#ifndef bzero
# define bzero(_m, _s) memset(_m, 0, _s)
#endif

#ifndef xc_vsnprintf
# define xc_vsnprintf(xbuf, xbufsize , xfmt, ... ) _vsnprintf_s(xbuf,xbufsize, _TRUNCATE,xfmt, __VA_ARGS__)
#endif

#ifndef strcasecmp
# define strcasecmp _stricmp 
#endif

#ifndef strncasecmp
#define strncasecmp _strnicmp 
#endif

#pragma warning(disable:4996)
#define xc_strncpy(_dest,_src, _ncount ) strncpy(_dest,_src, _ncount)
#define xc_strdup _strdup

#else
#define xc_snprintf snprintf
#define xc_vsnprintf vsnprintf
#define xc_strncpy strncpy
#define xc_strdup strdup
#endif





static void MyAssert(char *s)
{
	xc_printf("%s\n", s);

#ifdef _MSWINDOWS_
	//DebugBreak();
	assert(0);
#else
	(*(int*)0) = 0;
#endif

}

static void MyAbort(char *s)
{
	xc_printf("%s\n", s);
#ifdef _MSWINDOWS_
	abort();
#else
	(*(int*)0) = 0;
#endif
}


#define kAssertBuffSize 256

#ifdef _DEBUG

#define XCAssert(condition)    {                              \
	\
	if (!(condition))                                       \
{                                                       \
	char s[kAssertBuffSize];                            \
	s[kAssertBuffSize -1] = 0;                          \
	xc_snprintf (s,kAssertBuffSize -1, "_Assert: %s, %s, %d", #condition, __FILE__, __LINE__ ); \
	MyAssert(s);                                        \
}   }


#define XCAssertV(condition,errNo)    {                                   \
	if (!(condition))                                                   \
{                                                                   \
	char s[kAssertBuffSize];                                        \
	s[kAssertBuffSize -1] = 0;                                      \
	xc_snprintf( s,kAssertBuffSize -1, "_AssertV: %s, %s, %d (%d)", #condition, __FILE__, __LINE__, errNo );    \
	MyAssert(s);                                                    \
}   }

#else
#define XCAssert(condition) do{ if(condition){break;} } while(0)
#define XCAssertV(condition,errNo) do{ if(condition){break;} } while(0)
#endif


#define XCAbort(condition)    {                              \
	\
	if (!(condition))                                       \
{                                                       \
	char s[kAssertBuffSize];                            \
	s[kAssertBuffSize -1] = 0;                          \
	xc_snprintf (s,kAssertBuffSize -1, "_Abort: %s, %s, %d", #condition, __FILE__, __LINE__ ); \
	MyAbort(s);                                        \
}   }


#define XCAbortV(condition,errNo)    {                                   \
	if (!(condition))                                                   \
{                                                                   \
	char s[kAssertBuffSize];                                        \
	s[kAssertBuffSize -1] = 0;                                      \
	xc_snprintf( s,kAssertBuffSize -1, "_AbortV: %s, %s, %d (%d)", #condition, __FILE__, __LINE__, errNo );    \
	MyAbort(s);                                                    \
}   }


#define XCWarn(condition) {                                       \
	if (!(condition))                                       \
	xc_printf( "_Warn: %s, %s, %d\n",#condition, __FILE__, __LINE__ );     }                                                           

#define XCWarnV(condition,msg)        {                               \
	if (!(condition))                                               \
	xc_printf ("_WarnV: %s, %s, %d (%s)\n",#condition, __FILE__, __LINE__, msg );  }                                                   

#define XCWarnVE(condition,msg,err)  {                           		\
	if (!(condition))                                               \
	xc_printf ("_WarnVE: %s, %s, %d (%s [err=%d])\n",#condition, __FILE__, __LINE__, msg, err );  }


#define XC_CLASS_NOCOPY(classname) \
private : \
classname( const classname &dest ); \
classname &operator= (const  classname &dest);

#ifndef XC_ASSERT_RET_VAL
# define XC_ASSERT_RET_VAL(_condition, _val) if(!(_condition)){XCAssert(0); return _val;}
#endif

#ifndef XC_ASSERT_RET
#define XC_ASSERT_RET(_condition) if(!(_condition)){ XCAssert(0); return;}
#endif

#ifndef MIN
#define MIN(_a , _b) (((_a) < (_b)) ? (_a) : (_b))
#endif

#ifndef MAX
#define MAX(_a , _b) (((_a) > (_b)) ? (_a) : (_b))
#endif



#define XC_ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))


#define _XCT(x) x


#endif //end _XC_H_XCDEFS_H_
