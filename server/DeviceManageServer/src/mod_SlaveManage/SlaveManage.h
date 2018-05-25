#ifndef SLAVEMANAGE_DEF_H
#define SLAVEMANAGE_DEF_H

#include "SlaveServer.h"

#include <vector>
#include <queue>

namespace XC
{

	typedef std::vector<CSlave*> SlaveList;
	class CSlaveManage
	{
	public:

		CSlaveManage(void);

		~CSlaveManage(void);

		// 初始化资源
		EnumErrorCode Init( SystemInfo* sys );

		// 释放资源
		void Uninit( void );

		// 模块启动
		EnumErrorCode Start( void );

		// 模块停止
		void Stop( void );

		// 请求处理
		EnumErrorCode HandleRequest( const MessageInfo& msg );

		// 日志输出
		void TraceOut( const XCString& strMsg, Int32 iLevel, bool bOnScreen );

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
		GSMutex m_slaveVecMux;
		SlaveList m_vecSlaveHandle;
		CSlave* GetSlavePtr( GSMemCommHandle hHandle );


		//---------------------

		static void GS_MEMCOMM_CALLBACK EventCallback( GSMemCommHandle hChannel, int iStatus, void *pUserData );

		static void GS_MEMCOMM_CALLBACK MessageCallback( GSMemCommHandle hChannel,	void *pData, int iLen, void *pUserData );

		static void TimerFunc( struct _SystemInfo* sys,TimerHandle timerID, void *pTimerParam );

		// 加载slave
		void LoadSlave(int iSlaveNum);

		// 卸载slave
		void UnloadSlave( void );

		// 清理活动的Slave
		void CleanupAliveSlave( void );

		// 检测Slave状态
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