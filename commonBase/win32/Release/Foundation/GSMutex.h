/*
******************************************
Copyright (C): GOSUN
Filename : GSMUTEX.H
Author :  zouyx
Description: 同步锁的定义
********************************************
*/

#ifndef _GS_H_GSMUTEX_H_
#define _GS_H_GSMUTEX_H_

#include "GSFdtDefs.h"

class GSCond;

/*
********************************************************************
类注释
类名    :    GSMutex
类描述  :    普通同步锁， 非递归锁
*********************************************************************
*/

class GSFDT_CLASS GSMutex
{
private :
	friend class GSCond;
	int m_nLocked;
#ifdef _MSWINDOWS_
	CRITICAL_SECTION m_hOS;
#else
	pthread_mutex_t m_hOS;
#endif
	

public:
	GSMutex(void);
	~GSMutex(void);
	void	Lock();		//加锁
	void	Unlock();	//解锁
	bool  TryLock(); //true获取锁成功，false失败



	GS_CLASS_NOCOPY(GSMutex)
};



class GSFDT_CLASS GSAutoMutex
{
private :
	GSMutex &m_mutex;
public :
	GSAutoMutex( GSMutex &mutex );

	~GSAutoMutex(void);

	GS_CLASS_NOCOPY(GSAutoMutex)
};



/*
********************************************************************
类注释
类名    :   GSMutexRW
类描述  :	读写锁, 写优先锁
*********************************************************************
*/
typedef struct _StruRWMutexDebug  StruRWMutexDebug;

class GSFDT_CLASS GSMutexRW
{
private :
	
	//调试下 获取到锁的线程id
	StruRWMutexDebug *m_pDBGThread;
	int m_isWLocked;
#ifdef _MSWINDOWS_
	union {			
		SRWLOCK srwlock;
		struct {	
			CRITICAL_SECTION locker;
			HANDLE hReadEvent;
			HANDLE hWriteEvent;
			int nReaders; //读锁 已经加锁的个数
			int nWaitReaders; //读锁 等待加锁的个数
			int nWriters; //写锁 等待加锁的个数+已经加锁的个数			
			int flags;
		}fallback;
	}m_hOS;
	bool m_isHadSRWlock;
	
#else
	pthread_rwlock_t m_hOS;
	pthread_rwlockattr_t m_attr;
#endif
	
public:

	GSMutexRW(  void );
	~GSMutexRW(void); 

	void LockRead(void);//加读锁
	void LockWrite(void);//加写锁
	void Unlock(void); //解锁


private :
	void UnlockRead(void);
	void UnlockWrite(void);



	GS_CLASS_NOCOPY(GSMutexRW)
};


class GSFDT_CLASS GSAutoReadLock
{
private :
	GSMutexRW &m_wrmutex;
public :
	GSAutoReadLock( GSMutexRW &wrmutex);
	~GSAutoReadLock(void);

	GS_CLASS_NOCOPY(GSAutoReadLock)

};

class GSFDT_CLASS GSAutoWriteLock
{
private :
	GSMutexRW &m_wrmutex;
public :
	GSAutoWriteLock( GSMutexRW &wrmutex);
	~GSAutoWriteLock(void);

	GS_CLASS_NOCOPY(GSAutoWriteLock)

};

/*
********************************************************************
类注释
类名    :    GSCond
类描述  :    条件变量
*********************************************************************
*/

class GSFDT_CLASS GSCond
{
private :
#ifdef _MSWINDOWS_
	bool m_isHadCondVar;
	union {			
		CONDITION_VARIABLE condVar;
		struct {				
				unsigned int nWaitersCount;
				CRITICAL_SECTION lockerWaitersCount;
				HANDLE hSignalEvent;
				HANDLE hbroadcastEvent;		
		}fallback;
	}m_hOS;	
#else
	pthread_cond_t m_hOS;
	pthread_condattr_t m_attr;
#endif

public :
	typedef enum
	{
		COND_SUCCESS = 0,
		COND_ERR = -1,
		COND_TIMEOUT = -2,
	}CondErrno;
	GSCond( void );
	~GSCond(void); 

	void Wait( GSMutex &mutex );
	GSCond::CondErrno  WaitTimeout(GSMutex &mutex, int millisecond);

	void Signal(void);
	void Broadcast(void);

private :
#ifdef _MSWINDOWS_
	GSCond::CondErrno CondWaitHelper(GSMutex &mutex,int millisecond);
#endif
	GS_CLASS_NOCOPY(GSCond)
};



#if !defined(_ANDROID_) && !defined(_ios_) 
/*
********************************************************************
类注释
类名    :    GSProcessMutex
类描述  :    进程间锁
*********************************************************************
*/
class GSFDT_CLASS GSProcessMutex
{
public:
	GSProcessMutex( const char *szKey );
	~GSProcessMutex(void);
	void Lock( void );
	void Unlock( void );
	bool TryLock();//非阻塞的加锁，成功返回true,失败返回false

private:

#ifdef _MSWINDOWS_
	HANDLE m_GSProcessMutex;
#else
	int m_GSProcessMutex;
	GSMutex m_csMutex;    // 多线程互斥
#endif

	GS_CLASS_NOCOPY(GSProcessMutex)

};

#endif


#endif //end _GS_H_GSMUTEX_H_
