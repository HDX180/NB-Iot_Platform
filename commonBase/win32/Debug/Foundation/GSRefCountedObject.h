/*******************************************************************************
  Copyright (C), 2015-2015, GOSUNCN 
  File name   : GSREFCOUNTEDOBJECT.H      
  Author      : 江盛欣      
  Version     : 1.0.0        
  DateTime    : 2015/11/20 10:52
  Description : 引用计数对象
*******************************************************************************/
#ifndef GSREFCOUNTEDOBJECT_DEF_H
#define GSREFCOUNTEDOBJECT_DEF_H

#include "GSFdtDefs.h"
#include "GSSysApi.h"

//#if !defined(_ANDROID_) && !defined(_ios_) 

class GSFDT_CLASS GSRefCountedObject
{
public:
	GSRefCountedObject( void );

	void Duplicate( void ) const;

	void Release( void ) const;

	long ReferenceCount( void ) const;

protected:
	virtual ~GSRefCountedObject( void );

	GS_CLASS_NOCOPY(GSRefCountedObject);

	mutable GSAtomic m_Refs;
};

//#endif

#endif // GSREFCOUNTEDOBJECT_DEF_H