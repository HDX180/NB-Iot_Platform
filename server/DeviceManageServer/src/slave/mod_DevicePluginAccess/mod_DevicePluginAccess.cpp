#include "mod_DevicePluginAccess.h"
#include "DevicePluginAccess.h"



using namespace XC;

static Int32 mod_devicepluginaccess_init(SystemInfo* sys,void** pluginData)
{
	if ( INSTANCE(CDevicePluginAccess).Init(sys) )
	{
		sys->log("CDevicePluginAccess::Init() fail!", LOGGER_LEVEL_ERROR, true);
		return FAIL;
	}

	TRACE_LOG("mod_devicepluginaccess_init success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_devicepluginaccess_start(SystemInfo* sys,void* pluginData)
{
	if ( INSTANCE(CDevicePluginAccess).Start() )
	{
		TRACE_LOG("CDevicePluginAccess::Start() fail!", LOGGER_LEVEL_ERROR,true);
		return FAIL;
	}

	TRACE_LOG("mod_devicepluginaccess_start success", LOGGER_LEVEL_DEBUG,false);

	return SUCCEED;
}

static Int32 mod_devicepluginaccess_handleMsg(SystemInfo* sys,const MessageInfo& msg,void* pluginData)
{

	return SUCCEED;
}

static Int32 mod_devicepluginaccess_messageNotify(SystemInfo* sys,int messageID ,void* inMessage, void* outResult, void* pluginData)
{
	XC_ASSERT_RET_VAL(inMessage,FAIL);

	switch ( messageID )
	{
	case MESSAGE_FROM_MASTER:
		{
			StruProCommData *pstMasterReqMsg = (StruProCommData*)inMessage;
			INSTANCE(CDevicePluginAccess).HandleRequest(pstMasterReqMsg);
		}
		break;

	default:
		{

		}
		break;
	}
	return SUCCEED;
}

static Int32 mod_devicepluginaccess_stop(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_devicepluginaccess_stop...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CDevicePluginAccess).Stop();

	return SUCCEED;
}


static Int32 mod_devicepluginaccess_cleanup(SystemInfo* sys,void* pluginData)
{
	TRACE_LOG("enter mod_devicepluginaccess_cleanup...", LOGGER_LEVEL_DEBUG,false);
	INSTANCE(CDevicePluginAccess).Uninit();

	return SUCCEED;
}

static Int32 mod_devicepluginaccess_eventNotify(SystemInfo* sys,ENUM_EVENT_TYPE eventType,void* pluginData)
{
	switch ( eventType )
	{
	case EVENT_CONNECTED:
		xc_printf("mod_devicepluginaccess: receive connected event.\n");
		break;
	case EVENT_DISCONNECTED:
		xc_printf("mod_devicepluginaccess: receive disconnected event.\n");
		break;
	case EVENT_PUBLISH_FAIL:
		xc_printf("mod_devicepluginaccess: receive publish fail event.\n");
		break;
	default:
		xc_printf("mod_devicepluginaccess: receive unknown event: %d .\n", eventType);
		break;
	}

	return SUCCEED;
}

static Int32 mod_devicepluginaccess_httpQuery(SystemInfo* sys,HttpQueryInfos* pHttpQueryInfos,void* pluginData)
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


MOD_DEVICEPLUGINACCESS_API int PluginInit(struct _PluginInfo* pInfo)
{

	pInfo->version = MOD_VERSION_DEVICEPLUGINACCESS;
	xc_strncpy(pInfo->name, MOD_NAME_DEVICEPLUGINACCESS,strlen(MOD_NAME_DEVICEPLUGINACCESS));
	pInfo->init = mod_devicepluginaccess_init;
	pInfo->start = mod_devicepluginaccess_start;
	pInfo->handleMsg = mod_devicepluginaccess_handleMsg;
	pInfo->stop = mod_devicepluginaccess_stop;
	pInfo->cleanup = mod_devicepluginaccess_cleanup;
	pInfo->eventNotify = mod_devicepluginaccess_eventNotify;
	pInfo->httpQuery = mod_devicepluginaccess_httpQuery;
	pInfo->modMessageNotify = mod_devicepluginaccess_messageNotify;


	return 0;
}