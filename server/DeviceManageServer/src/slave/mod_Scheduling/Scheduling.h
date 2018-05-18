/*******************************************************************************
  Copyright (C), 2018-2018, XC
  File name   : SCHEDULING.H      
  Author      : hudaixin     
  Version     : 1.0.0        
  DateTime    : 2018/5/15 13:41
  Description : ����ʵ����
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

		// ��ʼ����Դ
		EnumErrorCode Init( SystemInfo* sys );

		// �ͷ���Դ
		void Uninit( void );

		// ģ������
		EnumErrorCode Start( void );

		// ģ��ֹͣ
		void Stop( void );

		// ������
		//EnumErrorCode HandleRequest( _StruProCommData *pstProCommData );

		//// �ظ�����
		//EnumErrorCode HandleResponse( _StruModResult *pResult );

		//// ������Ϣ����
		//EnumErrorCode UploadMessage( _StruModRequest *pstRequest );

		// ������
		EnumErrorCode SendMsgToMaster( StruProCommData *pstProCommData );

		// ��־���
		void TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen );

		// ��ȡ����
	//	const char* GetConfig( const XCString& strSection, const XCString& strKey, const XCString& strDefualt );

		// ������ʱ��
		EnumErrorCode StartTimer( Int32 nIntervalMS, FunPtrTimerCallback fnCB, void *pTimerParam, TimerHandle *pTimerID );

		// ֹͣ��ʱ��
		void StopTimer( TimerHandle pTimerID );


	private:
		// ��־
		LogFunc m_pLog;

		// ��Ϣ�ص�
		SendMessageFunc m_pSendMsg;

		// ����
		GetConfigFunc m_pGetConfig;

		// ��ʱ������
		StartTimerFunc m_pStartTimer;
		StopTimerFunc m_pStopTimer;

		// ֪ͨ����ģ�鴦����Ϣ
		NotifyModMessageFunc m_pNotifyModMsg;

		// ϵͳ�˳�
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
	INSTANCE(CScheduling).TraceOut(oss.str(), level, true); }   // ����־������������Ļ����

#define LOG_FUNCTION(msg)	TRACE_LOG("[FUNCTION] " << msg, LOGGER_LEVEL_INFO, false)

#define LOG_RESULT(msg, screen) TRACE_LOG("[RESULT] " << msg, LOGGER_LEVEL_INFO, screen)

#define HANDLE_MESSAGE(Message)	 #Message << "=" << Message
}

#endif // SCHEDULING_DEF_H