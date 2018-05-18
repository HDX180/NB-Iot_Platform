#include "DevicePluginAccess.h"

using namespace XC;


//#define CMD_TIMEOUT 300000   // 5分钟
#define UPDATE_DATA  240000    // 4分钟

CDevicePluginAccess::CDevicePluginAccess(void)
{
}

CDevicePluginAccess::~CDevicePluginAccess()
{

}

EnumErrorCode CDevicePluginAccess::Init( SystemInfo* sys )
{
	//加载设备插件
	return ERR_SUCCESS;
}

void CDevicePluginAccess::Uninit( void )
{
	//卸载设备插件
}

void CDevicePluginAccess::HandleMasterMsg(void)
{
	//如果是获取普通数据请求

		//从缓存数据返回


		//若无则调用设备接口获取，返回并加到缓存中

	//如果是其他请求

		//调用设备接口
}

void CDevicePluginAccess::ThreadHandleRequestFunc( GSThread &thread, void *pThreadData )
{
	XC_ASSERT_RET(pThreadData);
	CDevicePluginAccess *pThis = (CDevicePluginAccess*)pThreadData;

	while ( !thread.TestExit() )
	{
		pThis->HandleMasterMsg();

		if ( pThis->IsReqListEmpty() )   // 空则休眠...
			GSSleep(10);
	}
}

void CDevicePluginAccess::TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen )
{
	XC_ASSERT_RET(m_pLog);
	m_pLog(strMsg.c_str(), iLevel, bOnScreen);
}

EnumErrorCode CDevicePluginAccess::StartTimer( Int32 nIntervalMS, FunPtrTimerCallback fnCB, void *pTimerParam, TimerHandle *pTimerID )
{
	LOG_FUNCTION("CDevicePluginAccess::StartTimer()");

	XC_ASSERT_RET_VAL(m_pStartTimer,ERR_MEMORY_EXCEPTION);

	*pTimerID = m_pStartTimer(nIntervalMS, fnCB, pTimerParam);
	if ( *pTimerID == INVALID_TIMER_HANDLE )
	{
		TRACE_LOG("CDevicePluginAccess::StartTimer() start timer fail! ", LOGGER_LEVEL_ERROR, true);
		return ERR_CALL_INTERFACE_FAIL;
	}

	TRACE_LOG("start timer success. timerid: "<<*pTimerID, LOGGER_LEVEL_INFO, false);

	return ERR_SUCCESS;
}

//void CDevicePluginAccess::TestCmdTimeout(void)
//{
//	GSAutoMutex csAuto(m_csMutex);
//
//	if ( m_MasterRequstList.empty() )
//	{
//		m_csTimeMeter.Reset();
//		return false;
//	}
//
//	return m_csTimeMeter.GetElapsed()>CMD_TIMEOUT; 
//}
//
//void CDevicePluginAccess::ThreadHandleCmdTimeOutFunc( GSThread &thread, void *pThreadData )
//{
//	XC_ASSERT_RET(pThreadData);
//	CDevicePluginAccess *pThis = (CDevicePluginAccess*)pThreadData;
//
//	while ( !thread.TestExit() )
//	{
//		pThis->TestCmdTimeout();
//
//		for ( int i=0; i<10&&!thread.TestExit(); i++ )
//			GSSleep(1000);
//	}
//}

EnumErrorCode CDevicePluginAccess::Start( void )
{

	LOG_FUNCTION("CDevicePluginAccess::Start()");

	// 启动处理Master请求调度线程
	if ( !m_ThreadHandleReq.Start(ThreadHandleRequestFunc,this) )
	{
		TRACE_LOG("启动处理Master请求调度线程 失败! ",LOGGER_LEVEL_ERROR,true);
		return ERR_INIT_FAIL;
	}

	//if ( !m_ThreadCmdTimeOut.Start(ThreadHandleCmdTimeOutFunc,this) )
	//{
	//	TRACE_LOG("启动处理检测命令队列超时线程 失败! ",LOGGER_LEVEL_ERROR,true);
	//	return ERR_INIT_FAIL;
	//}

	StartTimer(UPDATE_DATA, OnTimeUpdateFunc, this, &m_pTimerID);

	m_bShutDown = FALSE;

	return ERR_SUCCESS;
}

void CDevicePluginAccess::OnTimeUpdateFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam )
{
	XC_ASSERT_RET(pTimerParam);
	CDevicePluginAccess *pThis = (CDevicePluginAccess*)pTimerParam;

	pThis->UpdateCacheData();
}

void CDevicePluginAccess::UpdateCacheData(void)
{
	//调用设备插件获取


	//跟新缓存数据
}

void CDevicePluginAccess::StopTimer( TimerHandle pTimerID )
{
	LOG_FUNCTION("CScheduling::StopTimer()");

	XC_ASSERT_RET(pTimerID);
	m_pStopTimer(pTimerID);

	TRACE_LOG("stop timer success. timerid: "<<pTimerID, LOGGER_LEVEL_INFO, false);
}

void CDevicePluginAccess::Stop( void )
{
	LOG_FUNCTION("CDevicePluginAccess::Stop()");

	m_ThreadHandleReq.Stop();
	m_ThreadHandleReq.Join();

	//m_ThreadCmdTimeOut.Stop();
	//m_ThreadCmdTimeOut.Join();

	StopTimer(m_pTimerID);

	m_bShutDown = TRUE;
}

void CDevicePluginAccess::HandleRequest( StruProCommData *pRequest )
{
	AddAsyncCmd(pRequest->pData,pRequest->iLen);
}