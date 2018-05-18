/*
******************************************
Copyright (C): GOSUN 二月 2017
Filename : GSBlockDeadlineClock.h
Author : xinxz 
Description: 高精度阻塞器
********************************************
*/

 
#ifndef __GSBlockDeadlineClock_H__
#define __GSBlockDeadlineClock_H__
#include "GSTypes.h"
#include "GSFdtDefs.h"
#include <Windows.h>
#include <mmsystem.h>
#include <sys/timeb.h>
#include <process.h>
#include <time.h>
#include "GxxList.hpp"


class GSFDT_CLASS GSBlockDeadlineClock
{
public:
    GSBlockDeadlineClock();
    ~GSBlockDeadlineClock();

    /**
     * Method		Reset
     * @brief		重置计时起始时刻
     * @return		void
     */
    void	Reset();
    /**
     * Method		GetElapsed
     * @brief		获取从Reset时刻开始到当前时刻的流逝时间
     * @return		UInt64	  返回流逝的时间，单位ms
     */
    UInt64	GetElapsed();
    /**
     * Method		BlockToAwaked
     * @brief		阻塞当前线程，直到从Reset开始到当前时刻的累计时间大于或等于ulMisiseconds
     * @param[in]	ulMiliseconds   距离Reset时刻的累计时间，单位ms
     * @return		bool  若ulMisiseconds小于累计时间，则立即返回false,否则返回true
     */
    bool	BlockToAwaked(UInt64 ulMiliseconds);

    /**
     * Method		Init
     * @brief		初始化环境
     * @return		bool	若返回false，表示环境不支持
     */
    static bool Init();

    /**
     * Method		UnInit
     * @brief		反初始化环境
     * @return		bool
     */
    static bool UnInit();

protected:
private:

    bool	SuspendCurrentThread(UInt64 milliSecond);
    static	void ResumeTimeoutThread();

    static	void CALLBACK TimerFunc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
    static	bool InitialWorkThread();

private:

    const static int	m_nClockStep = 1;

    static CRITICAL_SECTION	m_critical;
    static MMRESULT		    m_timerID;
    static double			m_dCountPerMilliSecond;
    static bool				m_bInitedOK;

    LARGE_INTEGER		    m_refScale;
    struct WaitedHandle
    {
        HANDLE					threadHandle;
        Int64					elapsed;//超时时间
    } ;

    typedef GxxListInner< WaitedHandle* > HandleListType;
    static HandleListType	waitList;
};


#endif // __GSBlockDeadlineClock_H__
