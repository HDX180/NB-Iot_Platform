#include "mod_Scheduling.h"
#include "Scheduling.h"

using namespace XC;

static Int32 mod_scheduling_init(SystemInfo* sys,void** pluginData)
{
	if ( INSTANCE(CScheduling).Init(sys) )
	{
		sys->log("CScheduling::Init() fail!", LOGGER_LEVEL_ERROR, true);
		return FAIL;
	}

	TRACE_LOG("mod_scheduling_init success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_scheduling_start(SystemInfo* sys,void* pluginData)
{
	if ( INSTANCE(CScheduling).Start() )
	{
		TRACE_LOG("CScheduling::Start() fail!", LOGGER_LEVEL_ERROR,true);
		return FAIL;
	}

	TRACE_LOG("mod_scheduling_start success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_scheduling_handleMsg(SystemInfo* sys,const MessageInfo& msg,void* pluginData)
{
	return SUCCEED;
}

static Int32 mod_scheduling_messageNotify(SystemInfo* sys,int messageID ,void* inMessage, void* outResult, void* pluginData)
{

	XC_ASSERT_RET_VAL(inMessage,FAIL);

	EnumErrorCode eRet = ERR_SUCCESS;

	switch ( messageID )
	{

	case MESSAGE_SEND_TO_MASTER:
		{
			StruProCommData *pData = (StruProCommData *)inMessage;
			INSTANCE(CScheduling).SendMsgToMaster(pData);
		}
		break;
	default:
		{

		}
		break;
	}

	return eRet==ERR_SUCCESS?SUCCEED:FAIL;
}

static Int32 mod_scheduling_stop(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_scheduling_stop...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CScheduling).Stop();

	return SUCCEED;
}


static Int32 mod_scheduling_cleanup(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_scheduling_cleanup...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CScheduling).Uninit();

	return SUCCEED;
}

static Int32 mod_scheduling_eventNotify(SystemInfo* sys,ENUM_EVENT_TYPE eventType,void* pluginData)
{
	switch ( eventType )
	{
	case EVENT_CONNECTED:
		xc_printf("mod_scheduling: receive connected event.\n");
		break;
	case EVENT_DISCONNECTED:
		xc_printf("mod_scheduling: receive disconnected event.\n");
		break;
	case EVENT_PUBLISH_FAIL:
		xc_printf("mod_scheduling: receive publish fail event.\n");
		break;
	default:
		xc_printf("mod_scheduling: receive unknown event: %d .\n", eventType);
		break;
	}

	return SUCCEED;
}

static Int32 mod_scheduling_httpQuery(SystemInfo* sys,HttpQueryInfos* pHttpQueryInfos,void* pluginData)
{
	//需要查询的信息
	//XCString strParam("");
	//Int32 iCount = 0;

	//strParam = "VERSION";
	//memcpy(pHttpQueryInfos->info[iCount].key,strParam.c_str(),strParam.length());
	//strParam = VERSION;
	//memcpy(pHttpQueryInfos->info[iCount++].value,strParam.c_str(),strParam.length());

	//pHttpQueryInfos->infoCnt = iCount;

	return SUCCEED;
}


MOD_SCHEDULING_API int PluginInit(struct _PluginInfo* pInfo)
{

	pInfo->version = MOD_VERSION_SCHEDULING;
	xc_strncpy(pInfo->name, MOD_NAME_SCHEDULING,strlen(MOD_NAME_SCHEDULING));
	pInfo->init = mod_scheduling_init;
	pInfo->start = mod_scheduling_start;
	pInfo->handleMsg = mod_scheduling_handleMsg;
	pInfo->stop = mod_scheduling_stop;
	pInfo->cleanup = mod_scheduling_cleanup;
	pInfo->eventNotify = mod_scheduling_eventNotify;
	pInfo->httpQuery = mod_scheduling_httpQuery;
	pInfo->modMessageNotify = mod_scheduling_messageNotify;


	return 0;
}