#include "SlaveManage.h"
#include "SlaveServer.h"
using namespace XC;

CSlaveManage::CSlaveManage()
{
	m_bShutDown = TRUE;	
}

CSlaveManage::~CSlaveManage()
{

}

void CSlaveManage::MessageCallback( GSMemCommHandle hChannel, void *pData, int iLen, void *pUserData )
{
	XC_ASSERT_RET(pUserData);
	CSlaveManage *pSlaveManage = (CSlaveManage *)pUserData;
	CSlave *pThis = pSlaveManage->GetSlavePtr(hChannel);
	XC_ASSERT_RET(pThis);

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

	m_sys = sys;
	m_pLog = sys->log;
	m_pSendMsg = sys->sendMsg;
	m_pGetConfig = sys->getConfig;
	m_pStartTimer = sys->startTimer;
	m_pStopTimer = sys->stopTimer;
	m_pNotifyModMsg = sys->notifyModMessage;

	if ( GSMemComm_Init(EventCallback, this, MessageCallback, this) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("GSMemComm_Init fail ", LOGGER_LEVEL_INFO, true);
	}

	return ERR_SUCCESS;
}

void CSlaveManage::Uninit()
{
	GSMemComm_Cleanup();
}

EnumErrorCode CSlaveManage::Start()
{
	LOG_FUNCTION("CSlaveManage::Start()");

	m_bShutDown = FALSE;
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
		LoadSlave(slaveNum);
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

	// 解析 ...
	const char* szCmdType = msg.GetUserData("......");
	const char* szDevCodeID = msg.GetUserData("....");

	XC_ASSERT_RET_VAL(szCmdType,ERR_PARAMETER_ERROR);
	XC_ASSERT_RET_VAL(szDevCodeID,ERR_PARAMETER_ERROR);

	TRACE_LOG("< In cmd:"<<szCmdType<<",DevCodeID:"<<szDevCodeID<<", sn:"<<msg.GetSeq()<<">", LOGGER_LEVEL_INFO, true);
	
	// 检测最大接入 ...


	//slave调度算法
	
	int iSlaveIndex = XCStrUtil::ToNumber<int>(szDevCodeID) / MAX_SLAVE_DEVNUM;
	CSlave* pSlave = GetSlavePtr(iSlaveIndex);
	
	if (!pSlave)
	{
		TRACE_LOG("<没找到可调度的slave, 创建新slave"<<iSlaveIndex<<">", LOGGER_LEVEL_INFO, true);
		pSlave = new CSlave(XCStrUtil::ToString(iSlaveIndex));
		XCAbort(pSlave);
		EnumErrorCode eRet = pSlave->Init();
		if ( eRet != ERR_SUCCESS )
		{
			TRACE_LOG("LoadSlave() Slave: "<<pSlave->m_strSlaveName<<" 启动失败",LOGGER_LEVEL_ERROR,true);
			XCAssert(0);
			return ERR_MEMORY_EXCEPTION;
		}

		GSAutoMutex csAuto(m_slaveVecMux);
		m_vecSlaveHandle.push_back(pSlave);
	}
	else
	{
		if (!pSlave->IsOnline())
		{
			TRACE_LOG("<调度的slave不在线, 稍候重试", LOGGER_LEVEL_ERROR, true);
			return ERR_SERVER_BUSY;
		}	
	}

	pSlave->SendMsg((void *)msg.GetContent(),msg.GetContentSize());
	return ERR_SUCCESS;
}

void CSlaveManage::TestSlaveStatus( void )
{
	GSAutoMutex csAuto(m_slaveVecMux);

	for ( SlaveList::iterator iter = m_vecSlaveHandle.begin();
		iter != m_vecSlaveHandle.end();
		iter ++ )
	{
		if ( !(*iter)->IsOnline() )
		{
			if ( (*iter)->Init() != ERR_SUCCESS )
			{
				TRACE_LOG("TestSlaveStatus() Slave: "<<(*iter)->m_strSlaveName<<" 重新启动失败,待下次重试!",LOGGER_LEVEL_ERROR,true);
				XCAssert(0);
			}
		}
	}
}

EnumErrorCode CSlaveManage::GetDevNumFromDB(int &SlaveNum)
{
	/*
	GSIConnection *pDBConnect = NULL;
	Int32 iErr = m_sys->getDBConnection(&pDBConnect);
	if ( iErr )
	{
		TRACE_LOG(__FUNCTION__<<"getDBConnection fail! err:"<<iErr,LOGGER_LEVEL_ERROR,true);
		return ERR_CONNECT_FAIL;
	}

	int iDevNum = 0;
	XCString strSql = "SELECT COUNT(*) AS DEVNUM FROM TB_DEVINFO";
	GSIRecordSet* pSet = pDBConnect->ExecuteQuery(strSql.c_str());
	if (pSet)
	{
		
		unsigned int availSize = 0;
		pSet->GetValue("DEVNUM", &iDevNum, sizeof(int), &availSize, DT_INT32);
		pDBConnect->ReleaseRecordSet(&pSet);
	}
	else
	{
		TRACE_LOG(__FUNCTION__<<" ExecuteQuery fail,sql: "<<strSql<<",Errcode: "<<pDBConnect->GetErrorCode()
					<<",Errmsg: "<<pDBConnect->GetErrorMessage(),LOGGER_LEVEL_ERROR,true);
		m_sys->releaseDBConnection(&pDBConnect);
		return ERR_CALL_INTERFACE_FAIL;
	}

	m_sys->releaseDBConnection(&pDBConnect);
	SlaveNum = iDevNum/MAX_SLAVE_DEVNUM;
	*/
	SlaveNum = 1;

	return ERR_SUCCESS;
}

void CSlaveManage::TimerFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam )
{
	XC_ASSERT_RET(pTimerParam);
	CSlaveManage *pThis = (CSlaveManage*)pTimerParam;
	if ( !pThis->m_bShutDown )
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

	while ( !m_bShutDown && PopData(&pData,iLen) )
	{
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


CSlave* CSlaveManage::GetSlavePtr( GSMemCommHandle hHandle )
{
	GSAutoMutex csAuto(m_slaveVecMux);
	for ( SlaveList::iterator iter = m_vecSlaveHandle.begin();
		iter != m_vecSlaveHandle.end();
		iter ++ )
	{
		if (hHandle == (*iter)->m_hChn)
			return *iter;
	}
	return NULL;
}

CSlave* CSlaveManage::GetSlavePtr( int iSlaveIndex )
{
	GSAutoMutex csAuto(m_slaveVecMux);
	for ( SlaveList::iterator iter = m_vecSlaveHandle.begin();
		iter != m_vecSlaveHandle.end();
		iter ++ )
	{
		if (XCStrUtil::ToString(iSlaveIndex) == (*iter)->m_strSlaveName)
			return *iter;
	}
	return NULL;
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
		//while ( pThis->Init() != ERR_SUCCESS )
		//{
		//	TRACE_LOG("TestSlaveStatus() Slave: "<<pThis->m_strSlaveName<<" 重新启动失败,待下次重试!",LOGGER_LEVEL_ERROR,true);
		//	XCAssert(0);
		//}
		break;
	case 2:  // 异常错误
		TRACE_LOG("slave: "<<pThis->m_strSlaveName<<" 异常错误!",LOGGER_LEVEL_INFO,true);
		pThis->SetSlaveStatus(SLAVE_EXCEPTION);
		break;

	default:
		break;
	}
}

void CSlaveManage::LoadSlave( int iSlaveNum )
{
	for(int i = 1; i <= iSlaveNum; ++i)
	{
		CSlave *pSlave = new CSlave(XCStrUtil::ToString(i));
		XCAbort(pSlave);
		EnumErrorCode eRet = pSlave->Init();
		if ( eRet != ERR_SUCCESS )
		{
			TRACE_LOG("LoadSlave() Slave: "<<pSlave->m_strSlaveName<<" 启动失败",LOGGER_LEVEL_ERROR,true);
			XCAssert(0);
			break;
		}

		GSAutoMutex csAuto(m_slaveVecMux);
		m_vecSlaveHandle.push_back(pSlave);
	}
}

void CSlaveManage::UnloadSlave( void )
{
	GSAutoMutex csAuto(m_slaveVecMux);

	for ( SlaveList::iterator iter = m_vecSlaveHandle.begin();
		iter != m_vecSlaveHandle.end();
		iter ++ )
	{
		(*iter)->UnInit();
		delete *iter;
	}

	m_vecSlaveHandle.clear();
}

