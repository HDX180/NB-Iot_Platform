#include "XCTypes.h"
#include "XCDefs.h"

#include "framework.h"

#include <string>
#include <iostream>


static void ShowMenu( void )
{
	printf("\n  enter 'q' to exit...  \n");
	printf("  enter 'dump' to getdump...  \n");
}

static void WaitForCommand( void )
{
	std::string strIn("");
	while ( 1 )
	{
		getline(std::cin, strIn);
#ifdef _LINUX
		if (strIn.empty()) 
		{
			std::cin.clear();
			::usleep(1000000);    // 1Ãë
			continue;
		}
#endif

		if ( strIn == "q" )
			break;
		else if ( strIn == "dump" )
			CFrameWork::GetInstance()->GetDump();
		else //if ( sIn == '?' )
			ShowMenu();
	}
}

int main( int argc, char *argv[] )
{
	bool bRet = true;
	if ( CFrameWork::GetInstance()->Init("config.ini", NULL, 0, false, true) == 0 )
	{
		if ( CFrameWork::GetInstance()->Start() != 0 )
		{
			printf( "CFrameWork::GetInstance()->Start() fail! \n");
			bRet = false;
		}
	}
	else
	{
		printf( "CFrameWork::GetInstance()->Init(config.ini) fail! \n");
		bRet = false;
	}


	if ( bRet )
	{
		ShowMenu();
		WaitForCommand();
	}


	CFrameWork::GetInstance()->Stop();
	CFrameWork::GetInstance()->Cleanup();

	return 0;
}