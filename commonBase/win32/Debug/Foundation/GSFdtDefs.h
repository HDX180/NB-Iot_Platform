/*
******************************************
Copyright (C): GOSUN
Filename : GSFDTEXPORT.H
Author :  zouyx
Description: 导出函数定义
********************************************
*/

#ifndef _GS_H_GSFDTEXPORT_H_
#define _GS_H_GSFDTEXPORT_H_


#include "XCTypes.h"
#include "XCDefs.h"

#if defined(_C14_Support_)
	#include "C14_GSFoundation_Interface.h"
#endif

#ifdef _GSFOUNDATION_STATIC

# define GSFDT_CLASS

#if defined(__GNUC__) || defined(_LINUX)

# define GSFDT_API EXPORT_API

#else
# define GSFDT_API   

#endif



#else


#ifdef GSFOUNDATION_EXPORTS

# define GSFDT_CLASS  EXPORT_CLASS
# define GSFDT_API    EXPORT_API

#else

# define GSFDT_CLASS  IMPORT_CLASS
# define GSFDT_API    IMPORT_API

#endif

#endif



#ifdef _LINUX

#include <unistd.h>
#include <pthread.h>

#endif





#endif //end _GS_H_GSFDTEXPORT_H_
