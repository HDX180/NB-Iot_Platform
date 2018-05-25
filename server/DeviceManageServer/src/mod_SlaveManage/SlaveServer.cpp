#include "SlaveServer.h"
#include "SlaveManage.h"


using namespace XC;

CSlave::CSlave(XCString strSlaveName)
{
	m_strSlaveName = strSlaveName;
	m_eStatus = SLAVE_INIT;
}

CSlave::~CSlave(void)
{

}

EnumErrorCode CSlave::Init()
{
	if ( GSMemComm_CreateChannel(&m_hChn, m_strSlaveName.c_str()) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("CSlave::Init() GSMemComm_CreateChannel fail! slave name: "
			<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
		return ERR_INIT_FAIL;
	}

	if ( !OpenSlaveProcess(m_strSlaveName) )
	{
		GSMemComm_DestroyChannel(m_hChn);
		m_hChn = NULL;
		return ERR_INIT_FAIL;
	}

	return ERR_SUCCESS;
}

void CSlave::CloseSlaveProcess()
{
	XC_ASSERT_RET(m_lProcessID>0);
	GSCloseProcess(m_lProcessID);

	TRACE_LOG("CSlave::TestCloseProcess() ProcessID["<<m_lProcessID<<"]", LOGGER_LEVEL_INFO, false);

	for ( Int32 i=0; i<10; i++ )
	{
		if ( !GSIsProcessAlive(m_lProcessID) )
		{
			m_lProcessID = -1;
			break;
		}
		GSSleep(500);
	}

	if ( m_lProcessID != -1 )
	{
		GSTerminateProcess(m_lProcessID);
		m_lProcessID = -1;
	}
}

void CSlave::UnInit()
{
	CloseSlaveProcess();

	GSMemComm_DestroyChannel(m_hChn);

	SetSlaveStatus(SLAVE_OFFLINE);
}

bool CSlave::OpenSlaveProcess( const XCString& strSlaveName )
{
	XCString strSlaveAppPath = GSGetApplicationPath();

#ifdef _WIN32

#ifdef _DEBUG
	strSlaveAppPath += "dms_slave/DMSSlaveD.exe";
#else
	strSlaveAppPath += "dgw_slave/DMSSlave.exe";
#endif

#else
	strSlaveAppPath += "dgw_slave/DMSSlave";
#endif

	Int32 argc = 1;
	const char *argv[1];
	argv[0] = strSlaveName.c_str();

	m_lProcessID = GSOpenProcess(strSlaveAppPath.c_str(),true,argc,argv);
	if  ( m_lProcessID == -1 )
	{
		TRACE_LOG("CSlave::OpenSlaveProcess() GSOpenProcess fail! slave app path: "<<strSlaveAppPath, LOGGER_LEVEL_ERROR, true);
		return FALSE;
	}

	return TRUE;
}

BOOL CSlave::IsOnline( void )
{
	return m_eStatus != SLAVE_OFFLINE;
}

void CSlave::SetSlaveStatus( EnumSlaveStatus eStatus )
{
	m_eStatus = eStatus; 
}

EnumErrorCode CSlave::SendMsg( void *pData, Int32 iLen )
{
	if ( !IsOnline() )
		return ERR_DEVICE_OFFLINE;

	XC_ASSERT_RET_VAL(m_hChn, ERR_MEMORY_EXCEPTION);

	if ( GSMemComm_SendMsg(m_hChn,pData,iLen) != GSMEMCOMM_SUCCESS )
	{
		TRACE_LOG("CSlave::SendMsg() GSMemComm_SendMsg fail! slave name: "<<m_strSlaveName, LOGGER_LEVEL_ERROR, true);
		return ERR_CALL_INTERFACE_FAIL;
	}

	return ERR_SUCCESS;
}

void CSlave::TestCloseProcess( long& lProcessID )
{
	XC_ASSERT_RET(lProcessID > 0);
	GSCloseProcess(lProcessID);

	TRACE_LOG("CSlave::TestCloseProcess() ProcessID["<<lProcessID<<"]", LOGGER_LEVEL_INFO, false);

	for ( Int32 i=0; i<10; i++ )
	{
		if ( !GSIsProcessAlive(lProcessID) )
		{
			lProcessID = -1;
			break;
		}
		GSSleep(500);
	}

	if ( lProcessID != -1 )
	{
		GSTerminateProcess(lProcessID);
		lProcessID = -1;
	}
}
