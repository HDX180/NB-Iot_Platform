#ifndef SLAVEMANAGE_DEF_H
#define SLAVEMANAGE_DEF_H

#include "SlaveServer.h"

#include <map>
#include <queue>

namespace XC
{
	//class CSlave;
	typedef std::map<GSMemCommHandle,CSlave*> SlaveList;
	class CSlaveManage
	{
	public:

		CSlaveManage(void);

		~CSlaveManage(void);

		// ��ʼ����Դ
		EnumErrorCode Init( SystemInfo* sys );

		// �ͷ���Դ
		void Uninit( void );

		// ģ������
		EnumErrorCode Start( void );

		// ģ��ֹͣ
		void Stop( void );

		// ������
		EnumErrorCode HandleRequest( const MessageInfo& msg );

		// ��־���
		void TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen );

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

		TimerHandle m_pTimerID;

		//---------------
		GSThread m_csThreadRecv;
		static void CALLBACK ThreadFunc( GSThread &thread, void *pThreadData );
		void HandleSlaveMsg( void );

//--------------About Slave Message Queue ---------------------
		std::queue<StruProCommData> m_DataList;
		GSMutex m_csMutexData;

		void AddData( void *pData , Int32 iLen )
		{
			GSAutoMutex csAuto(m_csMutexData);
			StruProCommData stMyData;
			stMyData.iLen = iLen;
			stMyData.pData = ::malloc(iLen);
			::memcpy(stMyData.pData,pData,iLen);
			m_DataList.push(stMyData);
		}

		bool PopData( void **pData, Int32 &iLen )
		{
			GSAutoMutex csAuto(m_csMutexData);
			if ( m_DataList.empty() )
				return false;

			StruProCommData stMyData = m_DataList.front();
			m_DataList.pop();

			*pData = stMyData.pData;
			iLen = stMyData.iLen;
			return true;
		}

		void FreeData( void *pData )
		{
			::free(pData);
		}

		bool IsListEmpty( void )
		{
			GSAutoMutex csAuto(m_csMutexData);
			return m_DataList.empty();
		}

		//-------------------------
		GSMutex m_slaveMapMux;
		SlaveList m_mapSlaveHandle;
		CSlave* GetSlavePtr( GSMemCommHandle hChannel );


		//---------------------

		static void GS_MEMCOMM_CALLBACK EventCallback( GSMemCommHandle hChannel, int iStatus, void *pUserData );

		static void GS_MEMCOMM_CALLBACK MessageCallback( GSMemCommHandle hChannel,	void *pData, int iLen, void *pUserData );

		static void TimerFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam );

		// ����slave
		void LoadSlave(void);

		// ж��slave
		void UnloadSlave( void );

		// ������Slave
		void CleanupAliveSlave( void );

		// ���Slave״̬
		void TestSlaveStatus(void);

		EnumErrorCode GetDevNumFromDB(int &SlaveNum);

	};

#include <sstream>

#define TRACE_LOG(msg, level, screen) {\
	std::ostringstream oss; \
	oss <<"[mod_SlaveManage] "<<msg;\
	INSTANCE(CSlaveManage).TraceOut(oss.str(), level, true); }

#define LOG_FUNCTION(msg)	TRACE_LOG("[FUNCTION] " << msg, LOGGER_LEVEL_INFO, false)

#define LOG_RESULT(msg, screen) TRACE_LOG("[RESULT] " << msg, LOGGER_LEVEL_INFO, screen)
}

#endif