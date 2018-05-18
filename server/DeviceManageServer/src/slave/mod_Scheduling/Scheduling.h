/*******************************************************************************
  Copyright (C), 2018-2018, XC
  File name   : SCHEDULING.H      
  Author      : hudaixin     
  Version     : 1.0.0        
  DateTime    : 2018/5/15 13:41
  Description : 调度实现类
*******************************************************************************/
#ifndef SCHEDULING_DEF_H
#define SCHEDULING_DEF_H

#include "CommonHead.h"
#include "GSMemComm.h"


namespace XC
{

	class CScheduling
	{
	public:
		CScheduling( void );

		~CScheduling( void );

		// 初始化资源
		EnumErrorCode Init( SystemInfo* sys );

		// 释放资源
		void Uninit( void );

		// 模块启动
		EnumErrorCode Start( void );

		// 模块停止
		void Stop( void );

		// 请求处理
		//EnumErrorCode HandleRequest( _StruProCommData *pstProCommData );

		//// 回复处理
		//EnumErrorCode HandleResponse( _StruModResult *pResult );

		//// 上送消息处理
		//EnumErrorCode UploadMessage( _StruModRequest *pstRequest );

		// 请求处理
		EnumErrorCode SendMsgToMaster( StruProCommData *pstProCommData );

		// 日志输出
		void TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen );

		// 获取配置
	//	const char* GetConfig( const XCString& strSection, const XCString& strKey, const XCString& strDefualt );

		// 启动定时器
		EnumErrorCode StartTimer( Int32 nIntervalMS, FunPtrTimerCallback fnCB, void *pTimerParam, TimerHandle *pTimerID );

		// 停止定时器
		void StopTimer( TimerHandle pTimerID );


	private:
		// 日志
		LogFunc m_pLog;

		// 消息回调
		SendMessageFunc m_pSendMsg;

		// 配置
		GetConfigFunc m_pGetConfig;

		// 定时器函数
		StartTimerFunc m_pStartTimer;
		StopTimerFunc m_pStopTimer;

		// 通知其他模块处理消息
		NotifyModMessageFunc m_pNotifyModMsg;

		// 系统退出
		BOOL m_bShutDown;


		char *m_pXmlBuf;
		Int32 m_iXmlBufLen;
		TimerHandle m_pTimerID;

		TimerHandle m_pTimerIDAccessInfo;


//-------------
		GSMemCommHandle m_hChn;

		XCString m_strSlaveName;

	private:
		static void GS_MEMCOMM_CALLBACK EventCallback( GSMemCommHandle hChannel, int iStatus, void *pUserData );

		void OnMessageCBFun( GSMemCommHandle hChannel, void *pData, int iLen );

		static void GS_MEMCOMM_CALLBACK MessageCallback( GSMemCommHandle hChannel,	void *pData, int iLen, void *pUserData );

		
	};

#include <sstream>

#define TRACE_LOG(msg, level, screen) {\
	std::ostringstream oss; \
	oss <<"[mod_Scheduling] "<<msg;\
	INSTANCE(CScheduling).TraceOut(oss.str(), level, true); }   // 待日志组件修正输出屏幕控制

#define LOG_FUNCTION(msg)	TRACE_LOG("[FUNCTION] " << msg, LOGGER_LEVEL_INFO, false)

#define LOG_RESULT(msg, screen) TRACE_LOG("[RESULT] " << msg, LOGGER_LEVEL_INFO, screen)

#define HANDLE_MESSAGE(Message)	 #Message << "=" << Message
}

#endif // SCHEDULING_DEF_H