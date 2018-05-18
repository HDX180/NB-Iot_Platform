/*******************************************************************************
  Copyright (C), XC
  File name   : MOD_SCHEDULING.H      
  Author      : hudaixin     
  Version     : 1.0.0        
  DateTime    : 2018/5/15 10:46
  Description : µ÷¶ÈÄ£¿é
*******************************************************************************/
#ifndef MOD_SCHEDULING_DEF_H
#define MOD_SCHEDULING_DEF_H

#include "pluginInfo.h" 

#ifdef WIN32

#ifdef MOD_SCHEDULING_EXPORTS
#define MOD_SCHEDULING_API __declspec(dllexport)
#else
#define MOD_SCHEDULING_API __declspec(dllimport)
#endif

#else
#define MOD_SCHEDULING_API	

#endif

#ifdef  __cplusplus   
extern "C" {
#endif 

	struct _pluginInfo;
	MOD_SCHEDULING_API int PluginInit(struct _PluginInfo* pInfo);

#ifdef  __cplusplus
}
#endif


#endif // MOD_SCHEDULING_DEF_H