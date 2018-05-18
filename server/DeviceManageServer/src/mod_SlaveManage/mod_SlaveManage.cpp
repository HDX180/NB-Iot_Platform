#include "mod_SlaveManage.h"
#include "SlaveManage.h"

using namespace XC;
static Int32 mod_slavemanage_init(SystemInfo* sys,void** pluginData)
{
	if ( INSTANCE(CSlaveManage).Init(sys) )
	{
		sys->log("CSlaveManage::Init() fail!", LOGGER_LEVEL_ERROR, true);
		return FAIL;
	}

	TRACE_LOG("mod_slavemanage_init success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_slavemanage_start(SystemInfo* sys,void* pluginData)
{
	if ( INSTANCE(CSlaveManage).Start() )
	{
		TRACE_LOG("CSlaveManage::Start() fail!", LOGGER_LEVEL_ERROR,true);
		return FAIL;
	}

	TRACE_LOG("mod_slavemanage_start success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_slavemanage_handleMsg(SystemInfo* sys,const MessageInfo& msg,void* pluginData)
{
	return INSTANCE(CSlaveManage).HandleRequest(msg) == ERR_SUCCESS? SUCCEED : FAIL;
}

static Int32 mod_slavemanage_messageNotify(SystemInfo* sys,int messageID ,void* inMessage, void* outResult, void* pluginData)
{
	switch ( messageID )
	{


	default:
		break;
	}
	return SUCCEED;
}

static Int32 mod_slavemanage_stop(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_slavemanage_stop...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CSlaveManage).Stop();

	return SUCCEED;
}


static Int32 mod_slavemanage_cleanup(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_slavemanage_cleanup...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CSlaveManage).Uninit();

	return SUCCEED;
}

static Int32 mod_slavemanage_eventNotify(SystemInfo* sys,ENUM_EVENT_TYPE eventType,void* pluginData)
{
	switch ( eventType )
	{
	case EVENT_CONNECTED:
		xc_printf("mod_slavemanage: receive connected event.\n");
		break;
	case EVENT_DISCONNECTED:
		xc_printf("mod_slavemanage: receive disconnected event.\n");
		break;
	case EVENT_PUBLISH_FAIL:
		xc_printf("mod_slavemanage: receive publish fail event.\n");
		break;
	default:
		xc_printf("mod_slavemanage: receive unknown event: %d .\n", eventType);
		break;
	}

	return SUCCEED;
}

static Int32 mod_slavemanage_httpQuery(SystemInfo* sys,HttpQueryInfos* pHttpQueryInfos,void* pluginData)
{
	//需要查询的信息


	return SUCCEED;
}


MOD_SLAVEMANAGE_API int PluginInit(struct _PluginInfo* pInfo)
{

	pInfo->version = MOD_VERSION_SLAVEMANAGE;
	xc_strncpy(pInfo->name, MOD_NAME_SLAVEMANAGE,strlen(MOD_NAME_SLAVEMANAGE));
	pInfo->init = mod_slavemanage_init;
	pInfo->start = mod_slavemanage_start;
	pInfo->handleMsg = mod_slavemanage_handleMsg;
	pInfo->stop = mod_slavemanage_stop;
	pInfo->cleanup = mod_slavemanage_cleanup;
	pInfo->eventNotify = mod_slavemanage_eventNotify;
	pInfo->httpQuery = mod_slavemanage_httpQuery;
	pInfo->modMessageNotify = mod_slavemanage_messageNotify;


	return 0;
}