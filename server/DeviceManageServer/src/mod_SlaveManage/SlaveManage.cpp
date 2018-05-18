#include "SlaveManage.h"
#include "SlaveServer.h"
using namespace XC;


CSlaveManage::CSlaveManage()
{

}

CSlaveManage::~CSlaveManage()
{

}

void CSlaveManage::MessageCallback( GSMemCommHandle hChannel, void *pData, int iLen, void *pUserData )
{
	XC_ASSERT_RET(pUserData);
	CSlaveManage *pSlaveManage = (CSlaveManage *)pUserData;
	CSlave *pThis = pSlaveManage->GetSlavePtr(hChannel);

	TRACE_LOG("<recv msg: "<<pData<<", from"<<pThis->m_strSlaveName<<">", LOGGER_LEVEL_INFO, true);

	pSlaveManage->AddData(pData, iLen);
	
}

EnumErrorCode CSlaveManage::Init(SystemInfo* sys)
{
	XC_ASSERT_RET_VAL(sys, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->log, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->sendMsg, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->getConfig, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->startTimer, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->stopTimer, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(sys->notifyModMessage, ERR_MEMORY_EXCEPTION);
	XC_ASSERT_RET_VAL(m_bShutDown, ERR_MEMORY_EXCEPTION);

	m_pLog = sys->log;
	m_pSendMsg = sys->sendMsg;
	m_pGetConfig = sys->getConfig;
	m_pStartTimer = sys->startTimer;
	m_pStopTimer = sys->stopTimer;
	m_pNotifyModMsg = sys->notifyModMessage;

	return ERR_SUCCESS;
}

void CSlaveManage::Uninit()
{

}

EnumErrorCode CSlaveManage::Start()
{
	LOG_FUNCTION("CSlaveManage::Start()");
	CleanupAliveSlave();   // 先清理当前路径所有活动slave进程

	// 启动接收slave数据调度线程
	if ( !m_csThreadRecv.Start(ThreadFunc,this) )
	{
		TRACE_LOG("启动接收slave数据调度线程 失败! ",LOGGER_LEVEL_ERROR,true);
		return ERR_INIT_FAIL;
	}
	
	// 加载启动slave
	int slaveNum = 0;
	if( !GetDevNumFromDB(slaveNum) )
	{
		LoadSlave();
	}

	// 启动Slave状态管理定时器
	return  StartTimer(1000, TimerFunc, this, &m_pTimerID);

}

void CSlaveManage::StopTimer( TimerHandle pTimerID )
{
	LOG_FUNCTION("CSlaveManage::StopTimer()");

	XC_ASSERT_RET(m_pStopTimer);
	XC_ASSERT_RET(pTimerID);
	m_pStopTimer(pTimerID);

	TRACE_LOG("stop timer success. timerid: "<<pTimerID, LOGGER_LEVEL_INFO, false);
}

void CSlaveManage::Stop(void)
{
	LOG_FUNCTION("CSlaveManage::Stop()");
	m_bShutDown = TRUE;

	StopTimer(m_pTimerID);
	m_pTimerID = INVALID_TIMER_HANDLE;

	m_csThreadRecv.Stop();
	m_csThreadRecv.Join();

	void *pData = NULL;
	Int32 iLen = 0;
	while ( PopData(&pData,iLen) )
		FreeData(pData);

	UnloadSlave();
}

EnumErrorCode CSlaveManage::HandleRequest( const MessageInfo& msg )
{
	if ( m_bShutDown )
		return ERR_SYSTEM_SHUTDOWN;


	// 从pData中解析 ...


	// 检测最大接入 ...


	//slave调度算法


	//pSlave->SendMsg(pData,iLen);


	return ERR_SUCCESS;
}

void CSlaveManage::TestSlaveStatus( void )
{
	GSAutoMutex csAuto(m_slaveMapMux);

	for ( SlaveList::iterator iter = m_mapSlaveHandle.begin();
		iter != m_mapSlaveHandle.end();
		iter ++ )
	{
		CSlave* pSlave = (*iter).second;
		if ( !pSlave->IsOnline() )
		{
			if ( pSlave->Init() != ERR_SUCCESS )
			{
				TRACE_LOG("TestSlaveStatus() Slave: "<<pSlave->m_strSlaveName<<" 重新启动失败,待下次重试!",LOGGER_LEVEL_ERROR,true);
				XCAssert(0);
			}
		}
	}
}

EnumErrorCode CSlaveManage::GetDevNumFromDB(int &SlaveNum)
{
	return ERR_SUCCESS;
}

void CSlaveManage::TimerFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam )
{
	XC_ASSERT_RET(pTimerParam);
	CSlaveManage *pThis = (CSlaveManage*)pTimerParam;

	pThis->TestSlaveStatus();

}

EnumErrorCode CSlaveManage::StartTimer( Int32 nIntervalMS, FunPtrTimerCallback fnCB, void *pTimerParam, TimerHandle *pTimerID )
{
	LOG_FUNCTION("CSlaveManage::StartTimer()");

	XC_ASSERT_RET_VAL(m_pStartTimer,ERR_MEMORY_EXCEPTION);

	*pTimerID = m_pStartTimer(nIntervalMS, fnCB, pTimerParam);
	if ( *pTimerID == INVALID_TIMER_HANDLE )
	{
		TRACE_LOG("CSlaveManage::StartTimer() start timer fail!", LOGGER_LEVEL_ERROR, true);
		return ERR_CALL_INTERFACE_FAIL;
	}

	TRACE_LOG("start timer success. timerid: "<<*pTimerID, LOGGER_LEVEL_INFO, false);

	return ERR_SUCCESS;
}

void CSlaveManage::CleanupAliveSlave( void )
{
	XCString strSlaveAppPath = GSGetApplicationPath();

#ifdef _WIN32

#ifdef _DEBUG
	strSlaveAppPath += "dms_slave/DMSSlaveD.exe";
#else
	strSlaveAppPath += "dms_slave/DMSSlave.exe";
#endif

#else
	strSlaveAppPath += "dms_slave/DMSSlave";
#endif

	long lProcessID[64] = {0};
	Int32 iProcessNum = 64;

	if ( !GSGetAliveProcessList(strSlaveAppPath.c_str(), lProcessID, iProcessNum) )
		return ;

	for ( Int32 i=0; i<iProcessNum; i++ )
		CSlave::TestCloseProcess(lProcessID[i]);

}

void CSlaveManage::HandleSlaveMsg( void )
{
	void *pData = NULL;
	Int32 iLen = 0;

	while ( PopData(&pData,iLen) )
	{
		if ( m_bShutDown )
		{
			FreeData(pData);  
			break;
		}
		
		//Handle Response/Notify Message


	}
}

void CSlaveManage::ThreadFunc( GSThread &thread, void *pThreadData )
{
	XC_ASSERT_RET(pThreadData);
	CSlaveManage *pThis = (CSlaveManage*)pThreadData;

	while ( !thread.TestExit() )
	{
		pThis->HandleSlaveMsg();

		if ( pThis->IsListEmpty() )   // 空则休眠...
			GSSleep(10);
	}
}

void CSlaveManage::TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen )
{
	XC_ASSERT_RET(m_pLog);
	m_pLog(strMsg.c_str(), iLevel, bOnScreen);
}


CSlave* CSlaveManage::GetSlavePtr( GSMemCommHandle hChannel )
{
	CSlave *p = NULL;

	GSAutoMutex csAuto(m_slaveMapMux);
	std::map<GSMemCommHandle,CSlave*>::iterator iter = m_mapSlaveHandle.find(hChannel);
	if ( iter != m_mapSlaveHandle.end() )
		p = (*iter).second;

	return p;
}

void CSlaveManage::EventCallback( GSMemCommHandle hChannel, int iStatus, void *pUserData )
{
	XC_ASSERT_RET(pUserData);
	CSlaveManage *pSlaveManage = (CSlaveManage *)pUserData;
	CSlave *pThis = pSlaveManage->GetSlavePtr(hChannel);
	XC_ASSERT_RET(pThis);

	switch ( iStatus )
	{
	case 0:   // 连接成功
		TRACE_LOG("slave: "<<pThis->m_strSlaveName<<" 上线!",LOGGER_LEVEL_INFO,true);
		pThis->SetSlaveStatus(SLAVE_ONLINE);
		break;
	case 1:  // 断开连接
		TRACE_LOG("slave: "<<pThis->m_strSlaveName<<" 下线!",LOGGER_LEVEL_INFO,true);
		pThis->SetSlaveStatus(SLAVE_OFFLINE);
		break;
	case 2:  // 异常错误
		TRACE_LOG("slave: "<<pThis->m_strSlaveName<<" 异常错误!",LOGGER_LEVEL_INFO,true);
		pThis->SetSlaveStatus(SLAVE_EXCEPTION);
		break;

	default:
		break;
	}
}

void CSlaveManage::LoadSlave( void )
{
	if ( GSMemComm_Init(EventCallback, NULL, MessageCallback, this) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("GSMemComm_Init fail ", LOGGER_LEVEL_INFO, true);
	}

	CSlave *pSlave = new CSlave();
	XCAbort(pSlave);
	EnumErrorCode eRet = pSlave->Init();
	if ( eRet != ERR_SUCCESS )
	{
		TRACE_LOG("LoadSlave() Slave: "<<pSlave->m_strSlaveName<<" 启动失败",LOGGER_LEVEL_ERROR,true);
		XCAssert(0);
	}

	GSAutoMutex csAuto(m_slaveMapMux);
	m_mapSlaveHandle[pSlave->m_hChn] = pSlave;
}

void CSlaveManage::UnloadSlave( void )
{
	GSAutoMutex csAuto(m_slaveMapMux);

	for ( SlaveList::iterator iter = m_mapSlaveHandle.begin();
		iter != m_mapSlaveHandle.end();
		iter ++ )
	{
		(*iter).second->UnInit();
	}

	m_mapSlaveHandle.clear();
}

