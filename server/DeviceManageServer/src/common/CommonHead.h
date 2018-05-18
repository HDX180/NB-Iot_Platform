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

#define	 MOD_VERSION_SCHEDULING 1
#define  MOD_NAME_SCHEDULING "mod_Scheduling"


#define	 MOD_VERSION_SLAVEMANAGE 1
#define  MOD_NAME_SLAVEMANAGE "mod_SlaveManage"


#define	 MOD_VERSION_DEVICEPLUGINACCESS 1
#define  MOD_NAME_DEVICEPLUGINACCESS "mod_DevicePluginAccess"


#define MESSAGE_FROM_MASTER     0x20000
#define MESSAGE_SEND_TO_MASTER  0x20001

	typedef struct _StruProCommData
	{
		Int32 iLen;
		void *pData;
	} StruProCommData;

}



#endif