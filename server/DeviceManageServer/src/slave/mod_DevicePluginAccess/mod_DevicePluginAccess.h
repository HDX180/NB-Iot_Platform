/*******************************************************************************
  Copyright (C), 2018-2018, GOSUNCN 
  File name   : MOD_DEVICEPLUGINACCESS.H      
  Author      : hudaixin     
  Version     : 1.0.0        
  DateTime    : 2018/5/15 17:58
  Description : 设备插件接入模块
*******************************************************************************/
#ifndef MOD_DEVICEPLUGINACCESS_DEF_H
#define MOD_DEVICEPLUGINACCESS_DEF_H

#include "pluginInfo.h" 

#ifdef WIN32

#ifdef MOD_DEVICEPLUGINACCESS_EXPORTS
#define MOD_DEVICEPLUGINACCESS_API __declspec(dllexport)
#else
#define MOD_DEVICEPLUGINACCESS_API __declspec(dllimport)
#endif

#else
#define MOD_DEVICEPLUGINACCESS_API	

#endif

#ifdef  __cplusplus   
extern "C" {
#endif 

	struct _pluginInfo;
	MOD_DEVICEPLUGINACCESS_API int PluginInit(struct _PluginInfo* pInfo);

#ifdef  __cplusplus
}
#endif


#endif // MOD_DEVICEPLUGINACCESS_DEF_H
