#ifndef _GS_FRAMEWORK_H_
#define _GS_FRAMEWORK_H_


struct _FrameWorkInfo;

//**************************************************
//ȫ�ֵ���,����GetInstanceʹ�ã�ReleaseInstances�ͷ�
//**************************************************
class CFrameWork
{
public:
	static CFrameWork* GetInstance(); //��ȡ����
	static void 	ReleaseInstance();	//�ͷ�

	const char*  GetVersion();//��ȡ�汾��

	int Init(const char* configFileName, void *pUserData=NULL, int iUserDataLen=0, bool bLicence=false, bool bSingleton=false);//��ʼ�����

	int Start();//��ʼ���
	
	bool PassCmdParam(const char* cmdParam);

	void Stop();//ֹͣ���

	void Cleanup();//������

	void GetDump();//��ȡ����dump
	
	const char* GetExeName(); // ��ȡ������
	
private:
	int InitLog();
	int InitCofing();
	int InitPlugin();
	int InitHttpServer();
	int InitDBConnection();
	int InitLicence();
	bool IsProgramRun();

private:
	// ����
	static	CFrameWork*	 m_pInstance;//ȫ��Ψһ
	_FrameWorkInfo  *m_pImpl; //pimpl���÷�,�������ݳ�Ա

	char m_szExeName[256];

private : 
	CFrameWork();
	~CFrameWork();
	CFrameWork( const CFrameWork &dest ); 
	CFrameWork &operator= (const  CFrameWork &dest);

};


#endif