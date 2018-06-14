#include "Scheduling.h"

using namespace XC;

CScheduling::CScheduling(void)
{
	m_bShutDown = TRUE;
}

CScheduling::~CScheduling(void)
{

}

void CScheduling::EventCallback( GSMemCommHandle hChannel, int iStatus, void *pUserData )
{
	XC_ASSERT_RET(pUserData);
	CScheduling *pThis = (CScheduling*)pUserData;

	switch ( iStatus )
	{
	case 0:   // 连接成功
		TRACE_LOG(pThis->m_strSlaveName<<" recv event: connect ",LOGGER_LEVEL_INFO,true);
		break;
	case 1:  // 断开连接
		TRACE_LOG(pThis->m_strSlaveName<<" recv event: disconnect. Ready to exit.",LOGGER_LEVEL_INFO,true);
		exit(-1); // 直接退出？
		break;
	case 2:  // 异常错误
		TRACE_LOG(pThis->m_strSlaveName<<" recv event: exception ",LOGGER_LEVEL_INFO,true);
		break;

	default:
		break;
	}
}

void CScheduling::MessageCallback( GSMemCommHandle hChannel, void *pData, int iLen, void *pUserData )
{
	XC_ASSERT_RET(pUserData);

	CScheduling *pThis = (CScheduling*)pUserData;
	TRACE_LOG("< Slave " << pThis->m_strSlaveName << " recv msg: "<<pData<<" >", LOGGER_LEVEL_INFO, true);

	pThis->OnMessageCBFun(hChannel, pData, iLen);
}

void CScheduling::OnMessageCBFun( GSMemCommHandle hChannel, void *pData, int iLen )
{
	StruProCommData stProCommData;
	stProCommData.iLen = iLen;
	stProCommData.pData = pData;

	if ( m_pNotifyModMsg(MOD_NAME_DEVICEPLUGINACCESS,MESSAGE_FROM_MASTER,&stProCommData,NULL) != SUCCEED )
	{
		TRACE_LOG("m_pNotifyModMsg() fail! modname: "<<MOD_NAME_SCHEDULING<<", slave name: "<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
	}
}

EnumErrorCode CScheduling::Init( SystemInfo* sys )
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

	LOG_FUNCTION("CScheduling::Init()");

	if ( GSMemComm_Init(EventCallback,this,MessageCallback,this) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("CScheduling::Init() GSMemComm_Init fail!", LOGGER_LEVEL_ERROR, true);
		return ERR_INIT_FAIL;
	}

	void *pData = NULL;
	Int32 iLen = 0;
	sys->getUserDataFunc(&pData,&iLen);
	XC_ASSERT_RET_VAL(pData, ERR_PARAMETER_ERROR);
	m_strSlaveName = (char*)pData;
	TRACE_LOG("Slave name: "<<m_strSlaveName, LOGGER_LEVEL_INFO, true);

	return ERR_SUCCESS;
}

void CScheduling::Uninit(void)
{
	LOG_FUNCTION("CScheduling::Uninit()");
	GSMemComm_Cleanup();
}

EnumErrorCode CScheduling::Start(void)
{
	LOG_FUNCTION("CScheduling::Start()");

	XC_ASSERT_RET_VAL(!m_hChn, ERR_MEMORY_EXCEPTION);
	if ( GSMemComm_OpenChannel(&m_hChn, m_strSlaveName.c_str()) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("CScheduling::Start() GSMemComm_OpenChannel fail! slave name: "<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
		return ERR_CALL_INTERFACE_FAIL;
	}

	m_bShutDown = FALSE;

	return ERR_SUCCESS;
}

void CScheduling::Stop(void)
{
	LOG_FUNCTION("CScheduling::Stop()");

	m_bShutDown = TRUE;

	XC_ASSERT_RET(m_hChn);

	GSMemComm_DestroyChannel(m_hChn);
	m_hChn = NULL;
}

EnumErrorCode CScheduling::SendMsgToMaster( StruProCommData *pstProCommData )
{
	if ( m_bShutDown )
		return ERR_SYSTEM_SHUTDOWN;

	XC_ASSERT_RET_VAL(m_hChn, ERR_MEMORY_EXCEPTION);

	if ( GSMemComm_SendMsg(m_hChn,pstProCommData->pData,pstProCommData->iLen) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("CScheduling::HandleRequest() GSMemComm_SendMsg fail! slave name: "<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
		return ERR_CALL_INTERFACE_FAIL;
	}

	return ERR_SUCCESS;
}

void CScheduling::TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen )
{
	XC_ASSERT_RET(m_pLog);
	m_pLog(strMsg.c_str(), iLevel, bOnScreen);
}