/*
******************************************
Copyright (C): GOSUN
Filename : GSTHREAD.H
Description: 线程定义
********************************************
*/

#ifndef _GS_H_GSTHREAD_H_
#define _GS_H_GSTHREAD_H_

#include "GSFdtDefs.h"
#include "GSMutex.h"


class GSThread;

/*******************************************************************************
功能说明：线程类，封装线程常用基本功能
*******************************************************************************/
typedef  void  (GS_CALLBACK *FunPtrThreadCallback)(GSThread &thread, void *pThreadData );

class GSFDT_CLASS GSThread
{
protected :
#if  defined(_MSWINDOWS_)
	HANDLE m_hThread;
#else
	pthread_t m_hThread;
#endif

	int m_flags;
	GSMutex m_mutex;
	GSCond m_cond;
	FunPtrThreadCallback m_fnUser;
	void *m_pFnThreadData;

	void *m_pUserData;
protected :
	long m_threadId;
public:
	GSThread(void);
	virtual ~GSThread(void);

	bool Start(FunPtrThreadCallback fnOnEvent, void *pThreadData=NULL);	 //开始线程，成功返回TRUE,失败返回FALSE	

	void Stop(void);	//停止线程

	bool TestExit(void) ;	//判断线程是否退出，在线程函数的循环中调用，执行暂停和唤醒操作	

	void Join(void);	//等待线程结束

	void Suspend(void);		//线程暂停

	bool Resume(void);		//唤醒线程,成功返回TRUE,失败返回FALSE	

	bool IsPause(void) ;    //线程是否在暂停
	bool IsRunning(void) ;	//返回线程运行状态，TRUE为正在运行，FALSE为未运行	

	long GetThreadID(void); //返回线程ID 号

	void SetUserData(void *pUserData);
	void *GetUserData(void);


	static long  GetCurrentThreadID(void); //返回当前线程ID

private :
	void JoinInner(void);

	void Detach(void);

	GS_CLASS_NOCOPY(GSThread)
};

#endif //end _GS_H_GSTHREAD_H_
