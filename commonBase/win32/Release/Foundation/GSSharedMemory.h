/*******************************************************************************
  Copyright (C), 2015-2015, GOSUNCN 
  File name   : GSSHAREDMEMORY.H      
  Author      : ½­Ê¢ÐÀ      
  Version     : 1.0.0        
  DateTime    : 2015/11/19 15:43
  Description : ¹²ÏíÄÚ´æ
*******************************************************************************/
#ifndef GSSHAREDMEMORY_DEF_H
#define GSSHAREDMEMORY_DEF_H

#include "GSFdtDefs.h"

//#if !defined(_ANDROID_) && !defined(_ios_) 

class GSSharedMemoryImpl;

class GSFDT_CLASS GSSharedMemory
{
public:
	enum EnumAccessMode
	{
		AM_READ = 0,
		AM_WRITE
	};

	GSSharedMemory( void );

	GSSharedMemory( const char* szName, UInt32 iSize, EnumAccessMode eMode, const void* pAddrHint = NULL, bool bServer = true );

	GSSharedMemory( const char* szFilePath, EnumAccessMode eMode, const void* pAddrHint = NULL );

	GSSharedMemory ( const GSSharedMemory& other );

	~GSSharedMemory( void );

	GSSharedMemory& operator = (const GSSharedMemory& other);

	bool Create( void );

	char* Begin( void ) const;

	char* End( void ) const;

	void Swap( GSSharedMemory& other );

private:
	GSSharedMemoryImpl* m_pImpl;
};

//#endif

#endif // GSSHAREDMEMORY_DEF_H