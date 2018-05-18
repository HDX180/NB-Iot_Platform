#ifndef _GS_FRAMEWORK_H_
#define _GS_FRAMEWORK_H_


struct _FrameWorkInfo;

//**************************************************
//全局单例,调用GetInstance使用，ReleaseInstances释放
//**************************************************
class CFrameWork
{
public:
	static CFrameWork* GetInstance(); //获取单例
	static void 	ReleaseInstance();	//释放

	const char*  GetVersion();//获取版本号

	int Init(const char* configFileName, void *pUserData=NULL, int iUserDataLen=0, bool bLicence=false, bool bSingleton=false);//初始化框架

	int Start();//开始框架
	
	bool PassCmdParam(const char* cmdParam);

	void Stop();//停止框架

	void Cleanup();//清理框架

	void GetDump();//获取运行dump
	
	const char* GetExeName(); // 获取进程名
	
private:
	int InitLog();
	int InitCofing();
	int InitPlugin();
	int InitHttpServer();
	int InitDBConnection();
	int InitLicence();
	bool IsProgramRun();

private:
	// 单例
	static	CFrameWork*	 m_pInstance;//全局唯一
	_FrameWorkInfo  *m_pImpl; //pimpl惯用法,隐藏数据成员

	char m_szExeName[256];

private : 
	CFrameWork();
	~CFrameWork();
	CFrameWork( const CFrameWork &dest ); 
	CFrameWork &operator= (const  CFrameWork &dest);

};


#endif