#ifndef SLAVESERVER_DEF_H 
#define SLAVESERVER_DEF_H

#include "CommonHead.h"
#include "GSMemComm.h"


namespace XC
{
	typedef enum
	{
		SLAVE_INIT = 0,
		SLAVE_ONLINE,
		SLAVE_OFFLINE,
		SLAVE_EXCEPTION
	} EnumSlaveStatus;

	class CSlave 
	{
	public:
		CSlave(XCString strSlaveName);

		~CSlave(void);

		EnumErrorCode Init();

		void UnInit();

		inline BOOL IsOnline( void );

		EnumErrorCode SendMsg( void *pData, Int32 iLen );

		void SetSlaveStatus( EnumSlaveStatus eStatus );

		static void TestCloseProcess( long& lProcessID );

		XCString m_strSlaveName;

		GSMemCommHandle m_hChn;

	private:

		long m_lProcessID;

		EnumSlaveStatus m_eStatus;

	private:
		bool OpenSlaveProcess( const XCString& strSlaveName );

		void CloseSlaveProcess();

	};


}

#endif //SLAVESERVER_DEF_H
