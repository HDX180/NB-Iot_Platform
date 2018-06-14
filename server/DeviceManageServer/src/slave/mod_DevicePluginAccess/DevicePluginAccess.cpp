#include "DevicePluginAccess.h"
#include "GSDBAccessApi.h"
#include <time.h>

using namespace XC;


//#define CMD_TIMEOUT 300000   // 5分钟
#define UPDATE_CACHE_INTERVAL  300000    // 5分钟

CDevicePluginAccess::CDevicePluginAccess():m_devCacheData(MAX_SLAVE_DEVNUM, NULL)
{
	m_bShutDown = TRUE;
}

CDevicePluginAccess::~CDevicePluginAccess()
{

}

EnumErrorCode CDevicePluginAccess::Init( SystemInfo* sys )
{

	XC_ASSERT_RET_VAL(sys, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->log, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->sendMsg, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->getConfig, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->startTimer, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->stopTimer, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->notifyModMessage, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(m_bShutDown, ERR_MEMORY_EXCEPTION);

	m_sys = sys;
	m_pLog = sys->log;
	m_pSendMsg = sys->sendMsg;
	m_pGetConfig = sys->getConfig;
	m_pStartTimer = sys->startTimer;
	m_pStopTimer = sys->stopTimer;
	m_pNotifyModMsg = sys->notifyModMessage;

	//加载设备插件

	void *pData = NULL;
	Int32 iLen = 0;
	sys->getUserDataFunc(&pData,&iLen);
	XC_ASSERT_RET_VAL(pData, ERR_PARAMETER_ERROR);
	m_strSlaveName = (char*)pData;


	return ERR_SUCCESS;
}

void CDevicePluginAccess::Uninit( void )
{
	//卸载设备插件
}

int CDevicePluginAccess::DevCodeToIndex(const int iDevCodeID)
{
	return iDevCodeID - XCStrUtil::ToNumber<int>(m_strSlaveName) * 1000;
}

void CDevicePluginAccess::HandleMasterMsg(void)
{
	void *pData = NULL;
	Int32 iLen = 0;

	while ( !m_bShutDown && PopCmd(&pData,iLen) )
	{
		int iDevCodeID = 0, iCommand = 0;

		int iDevIndex = DevCodeToIndex(iDevCodeID);

		//如果是获取普通数据请求
		if (DEV_GET_HEALTHDATA == iCommand)
		{
			StruDevData *pCacheData = m_devCacheData[iDevIndex];
			if (pCacheData)//从缓存数据返回
			{

			}
			else//若无则调用设备接口获取
			{
				

				//并加到缓存中
				//pCacheData->cache_row_mutex.Lock();
			}
			
		}
		else//如果是其他请求
		{
			//调用设备接口
			

		}

		//封装协议..

		StruProCommData stProCommData;
		if ( m_pNotifyModMsg(MOD_NAME_DEVICEPLUGINACCESS,MESSAGE_FROM_MASTER,&stProCommData,NULL) != SUCCEED )
		{
			TRACE_LOG("m_pNotifyModMsg() fail! modname: "<<MOD_NAME_SCHEDULING<<", slave name: "<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
		}
	}
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

bool CDevicePluginAccess::GreaterSort(StruDevData* a,StruDevData* b)
{
	return a->iDevCodeid > b->iDevCodeid;
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

	StartTimer(UPDATE_CACHE_INTERVAL, OnTimeUpdateFunc, this, &m_pTimerID);
	//if ( !m_ThreadUpdateCache.Start(OnTimeUpdateFunc,this) )
	//{
	//	TRACE_LOG("启动处理Master请求调度线程 失败! ",LOGGER_LEVEL_ERROR,true);
	//	return ERR_INIT_FAIL;
	//}

	if ( !m_ThreadUploadDBData.Start(ThreadUploadDBDataFunc,this) )
	{
		TRACE_LOG("启动处理上传数据中心调度线程 失败! ",LOGGER_LEVEL_ERROR,true);
		return ERR_INIT_FAIL;
	}


	m_bShutDown = FALSE;

	return ERR_SUCCESS;
}

void CDevicePluginAccess::ThreadUploadDBDataFunc( GSThread &thread, void *pThreadData )
{
	XC_ASSERT_RET(pThreadData);
	CDevicePluginAccess *pThis = (CDevicePluginAccess*)pThreadData;

	while ( !thread.TestExit() )
	{
		pThis->UploadDevDataToDB();

		if ( pThis->IsDevCacheListEmpty() )   // 空则休眠...
			GSSleep(1000);
	}
}

void CDevicePluginAccess::OnTimeUpdateFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam )
{
	XC_ASSERT_RET(pTimerParam);
	CDevicePluginAccess *pThis = (CDevicePluginAccess*)pTimerParam;
	
	if ( !pThis->m_bShutDown )
		pThis->UpdateCacheData();

}

void CDevicePluginAccess::UpdateCacheData(void)
{
	
	for (int i = 0; i < MAX_SLAVE_DEVNUM; ++i)
	{
		
		StruDevData strDevData;
		//调用设备插件获取
			

		//跟新实时缓存数据
		//GSAutoMutex csAuto(m_devCacheData[i]->cache_row_mutex);


		//加载到设备时间点数据缓冲区 
		AddDevDataTimeCache(strDevData);		
	}

}

void CDevicePluginAccess::AddDevDataTimeCache(const StruDevData& _strDevData)
{
	time_t time_seconds = time(NULL);
	struct tm* now_time = localtime(&time_seconds);
	if(iCurrTime < now_time->tm_hour)
	{
		GSAutoMutex csAuto(m_csMutexDevTimeDataList);
		m_devCacheTimeData.push_back(new StruDevData(_strDevData));
	}
}

void CDevicePluginAccess::UploadDevDataToDB()
{
	GSAutoMutex csAuto(m_csMutexDevTimeDataList);

	sort(m_devCacheTimeData.begin(), m_devCacheTimeData.end(), GreaterSort);

	//GSIConnection *pDBConnect = NULL;
	//Int32 iErr = m_sys->getDBConnection(&pDBConnect);
	//if ( iErr )
	//{
	//	TRACE_LOG(__FUNCTION__<<"getDBConnection fail! err:"<<iErr,LOGGER_LEVEL_ERROR,true);
	//	return ;
	//}

	////....根据codeid分区或分表插入

	//XCString strSql;
	//XCStrUtil::Format(strSql, "INSERT INTO TB_DEVDATA VALUES(%d)", strDevData.iTemprature);
	//if(!pDBConnect->ExecuteSql(strSql.c_str()))
	//{
	//	TRACE_LOG(__FUNCTION__<<" ExecuteSql fail,sql: "<<strSql<<",Errcode: "<<pDBConnect->GetErrorCode()
	//		<<",Errmsg: "<<pDBConnect->GetErrorMessage(),LOGGER_LEVEL_ERROR,true);
	//	m_sys->releaseDBConnection(&pDBConnect);
	//	return ;
	//}

	//m_sys->releaseDBConnection(&pDBConnect);

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
	m_bShutDown = TRUE;

	m_ThreadHandleReq.Stop();
	m_ThreadHandleReq.Join();

	//m_ThreadCmdTimeOut.Stop();
	//m_ThreadCmdTimeOut.Join();

	StopTimer(m_pTimerID);

	//m_ThreadUpdateCache.Stop();
	//m_ThreadUpdateCache.Join();

	void *pData = NULL;
	Int32 iLen = 0;
	while ( PopCmd(&pData,iLen) )
		FreeCmd(pData);
}

void CDevicePluginAccess::HandleRequest( StruProCommData *pRequest )
{
	AddAsyncCmd(pRequest->pData,pRequest->iLen);
}