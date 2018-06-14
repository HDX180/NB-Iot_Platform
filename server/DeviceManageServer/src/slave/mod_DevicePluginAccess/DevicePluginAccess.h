/*******************************************************************************
  Copyright (C), 2015-2015, GOSUNCN 
  File name   : DEVICEPLUGINACCESS.H      
  Author      : hudaixin      
  Version     : 1.0.0        
  DateTime    : 2018/5/16 10:33
  Description : �豸�������ʵ����
*******************************************************************************/
#ifndef DEVICEPLUGINACCESS_DEF_H
#define DEVICEPLUGINACCESS_DEF_H

#include "CommonHead.h"
#include <queue>
#include <vector>

namespace XC
{

	struct StruDevData 
	{
		int iDevCodeid;
		int iTemprature;
		//GSMutex cache_row_mutex;

		StruDevData()
		{
			
		}
		//	GSTimeMeter upDateTime;
	};

	
	class CDevicePluginAccess
	{
	public:
		CDevicePluginAccess( void );

		~CDevicePluginAccess( void );

		// ��ʼ����Դ
		EnumErrorCode Init( SystemInfo* sys );

		// �ͷ���Դ
		void Uninit( void );

		// ģ������
		EnumErrorCode Start( void );

		// ģ��ֹͣ
		void Stop( void );

		// ���͵�����ģ��
		EnumErrorCode NotifyMod( const XCString& strDstModName, Int32 iCmd, void *pData );

		void HandleRequest( StruProCommData *pRequest );
	public:

		// ��־���
		void TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen );

		// ������ʱ��
		EnumErrorCode StartTimer( Int32 nIntervalMS, FunPtrTimerCallback fnCB, void *pTimerParam, TimerHandle *pTimerID );

		// ֹͣ��ʱ��
		void StopTimer( TimerHandle pTimerID );

	private:

		SystemInfo* m_sys;

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

		XCString m_strSlaveName;

		//---��ʱ���»���
		TimerHandle m_pTimerID;
		static void OnTimeUpdateFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam );
		void UpdateCacheData(void);


		//--����Master�����߳�
		GSThread m_ThreadHandleReq;
		static void CALLBACK ThreadHandleRequestFunc( GSThread &thread, void *pThreadData );
		void HandleMasterMsg( void );

		GSThread m_ThreadUploadDBData;
		static void CALLBACK ThreadUploadDBDataFunc( GSThread &thread, void *pThreadData );
		void UploadDevDataToDB( void );

		//���»���
		//GSThread m_ThreadUpdateCache;
		//static void CALLBACK OnTimeUpdateFunc( GSThread &thread, void *pThreadData );
		//void UpdateCacheData( void );

		//--������������г�ʱ
		//GSThread m_ThreadCmdTimeOut;
		//static void CALLBACK ThreadHandleCmdTimeOutFunc( GSThread &thread, void *pThreadData );
		//void TestCmdTimeout( void );

		static bool GreaterSort(StruDevData* a,StruDevData* b);

		//--ʵʱ����
		std::vector<StruDevData*> m_devCacheData;

		//--ʱ��㻺��
		std::vector<StruDevData*> m_devCacheTimeData;
		GSMutex m_csMutexDevTimeDataList;

		bool IsDevCacheListEmpty(void)
		{
			GSAutoMutex csAuto(m_csMutexDevTimeDataList);
			return m_devCacheTimeData.empty();
		}

		//-----------------
		std::queue<StruProCommData> m_MasterRequstList;
		GSMutex m_csMutexData;

		void AddAsyncCmd( void *pData , Int32 iLen )
		{
			GSAutoMutex csAuto(m_csMutexData);
			StruProCommData stMyData;
			stMyData.iLen = iLen;
			stMyData.pData = ::malloc(iLen);
			::memcpy(stMyData.pData,pData,iLen);
			m_MasterRequstList.push(stMyData);
		}

		bool PopCmd( void **pData, Int32 &iLen )
		{
			GSAutoMutex csAuto(m_csMutexData);
			if ( m_MasterRequstList.empty() )
				return false;

			StruProCommData stMyData = m_MasterRequstList.front();
			m_MasterRequstList.pop();

			*pData = stMyData.pData;
			iLen = stMyData.iLen;
			return true;
		}

		void FreeCmd( void *pData )
		{
			::free(pData);
		}

		bool IsReqListEmpty( void )
		{
			GSAutoMutex csAuto(m_csMutexData);
			return m_MasterRequstList.empty();
		}

	private:
		int iCurrTime;//��ǰСʱ��

		void AddDevDataTimeCache(const StruDevData& _strDevData);

		inline int DevCodeToIndex(int iDevCodeID);

	};

#include <sstream>

#define TRACE_LOG(msg, level, screen) {\
	std::ostringstream oss; \
	oss <<"[mod_DevicePluginAccess] "<<msg;\
	INSTANCE(CDevicePluginAccess).TraceOut(oss.str(), level, true); }   

#define LOG_FUNCTION(msg)	TRACE_LOG("[FUNCTION] " << msg, LOGGER_LEVEL_INFO, false)

#define LOG_RESULT(msg, screen) TRACE_LOG("[RESULT] " << msg, LOGGER_LEVEL_INFO, screen)

#define HANDLE_MESSAGE(Message)	 #Message << "=" << Message
}

#endif // DEVICEPLUGINACCESS_DEF_H