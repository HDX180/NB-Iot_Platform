#ifndef COMMHEAD_DEF_H
#define COMMHEAD_DEF_H

#include "pluginInfo.h"
#include "ServerCommonDef.h"
#include "XCDefs.h"
#include "XCTypes.h"
#include "Singleton.hpp"
#include "GSFoundation.h"
#include "XCStrUtil.hpp"

namespace XC
{
//-----------------模块管理--------------
#define	 MOD_VERSION_SCHEDULING 1
#define  MOD_NAME_SCHEDULING "mod_Scheduling"


#define	 MOD_VERSION_SLAVEMANAGE 1
#define  MOD_NAME_SLAVEMANAGE "mod_SlaveManage"


#define	 MOD_VERSION_DEVICEPLUGINACCESS 1
#define  MOD_NAME_DEVICEPLUGINACCESS "mod_DevicePluginAccess"

//--------------模块通知--------------

#define MESSAGE_FROM_MASTER     0x20000
#define MESSAGE_SEND_TO_MASTER  0x20001

//-------------设备相关命令范围(0x30000-0x4FFFF)------------
#define DEV_COMMAND_BEGIN (0x30000)
//普通命令
#define DEV_GET_HEALTHDATA (DEV_COMMAND_BEGIN + 1)


#define MAX_SLAVE_NUM   100
#define MAX_SLAVE_DEVNUM 1000

	typedef struct _StruProCommData
	{
		Int32 iLen;
		void *pData;
	} StruProCommData;

}



#endif