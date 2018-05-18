/*******************************************************************************
  Copyright (C), XC
  File name   : MOD_SLAVEMANAGE.H   
  Author      : hudaixin    
  Description : slave¹ÜÀíÄ£¿é
*******************************************************************************/
#ifndef MOD_SLAVEMANAGE_DEF_H
#define MOD_SLAVEMANAGE_DEF_H

#include "pluginInfo.h" 

#ifdef WIN32

#ifdef MOD_SLAVEMANAGE_EXPORTS
#define MOD_SLAVEMANAGE_API __declspec(dllexport)
#else
#define MOD_SLAVEMANAGE_API __declspec(dllimport)
#endif

#else
#define MOD_SLAVEMANAGE_API	
#endif

#ifdef  __cplusplus   
extern "C" {
#endif 

	struct _pluginInfo;
	MOD_SLAVEMANAGE_API int PluginInit(struct _PluginInfo* pInfo);

#ifdef  __cplusplus
}
#endif


#endif // MOD_SLAVEMANAGE_DEF_H