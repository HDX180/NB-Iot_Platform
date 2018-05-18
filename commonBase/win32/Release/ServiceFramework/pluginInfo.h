#ifndef _GS_PLUGIN_INFO_H_
#define _GS_PLUGIN_INFO_H_

#include<string>
#include"GSTypes.h"



//ģ�鵼���ĳ�ʼ����������
#define MOD_EXPORT_INIT_FUNC_NAME  "PluginInit"
//ģ�鵼���ĳ�ʼ������ԭ��
typedef int (*Plugin_Init)(struct _PluginInfo* pInfo);



//�������ֵ �ɹ�0 ;ʧ��-1
#define SUCCEED  0
#define FAIL    -1


//��Ϣ����������ݽṹ
#include "message.h"


//##��־�ȼ�( 1:������־   2:��Ϣ��־   3:������־  4:������־  5:������־ )
#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 3
#define LOGGER_LEVEL_ERROR 4
#define LOGGER_LEVEL_FATAL 5


#define MAX_INFO_NUM 100


/********���¶�����ǿ�ܿ����ṩ�����ģ��ʹ�õĸ��ֺ���ָ��,ͨ��_SystemInfo�ṹ�崫�ݸ����ʹ��**********/
//��־����
typedef int (*LogFunc)(const char* msg,Int32 severity,bool condition);


//��ȡ���ú���
typedef char const*  (*GetConfigFunc)(char const* section, char const* key,char const* notfound);


//������Ϣ����
typedef int (*SendMessageFunc)(char* modName,const MessageInfo &msg);

//��ѯ��Ϣ�ͻ����ⲿ���ӵ�ַ
typedef int (*QueryConnectionFunc)(char* modName,char const* address, char *content, int content_size);


//�����������
typedef void (*AddHeartBeatFunc)(const char* serverName,pHeartBeatCallback pHeartBeatCB,int intervalSecond);

typedef void * TimerHandle;
#define INVALID_TIMER_HANDLE NULL

//��ʱ���ص�����,timerID����StartTimer���ص�ID
typedef void (*FunPtrTimerCallback)(struct _SystemInfo* sys, TimerHandle timerID, void *pTimerParam);
//��ʼ��ʱ����������λ:����
typedef TimerHandle (*StartTimerFunc)(int nIntervalMS, FunPtrTimerCallback fnCb, void *pTimerParam);


//ֹͣ��ʱ����timerID����StartTimer���ص�ID
typedef void (*StopTimerFunc)(TimerHandle timerID);


class GSIConnection;
//��ȡ���ݿ����ӣ�ÿ��GetDBConnectionʹ�ú����ǵ�ReleaseDBConnection.
//GetDBConnection���ش�����.0��ʾ�ɹ���������ο�EnumGSDBApiRet����
typedef int  (*GetDBConnectionFunc)(GSIConnection** pConnection);
typedef void (*ReleaseDBConnectionFunc)(GSIConnection** pConnection);


//��ȡ����ģ������˽�����ݣ����ģ�����Ҫ�������ݣ�����ͨ���˺�����ȡ
//��������Ϊģ��initʱ������pluginData
//pluginNameΪģ��initʱ����PluginInfo�����name
typedef void* (*GetPluginInfoFunc)(char* pluginName);


//��ȡ���������б�(��ʽ:subject1/subject2/....)
typedef char const*  (*GetSubscriptionsFunc)(void);
//׷�����ⶩ��
typedef void  (*AppendSubscriptionFunc)(const char* szSubject);


//֪ͨ����ģ�鴦����Ϣ,destModNameΪNULL��ʱ������ģ��(�����Լ�)�����յ�֪ͨ��Ϣ����ʱ���뱣֤�������outResultҲ��ΪNULL
typedef int (*NotifyModMessageFunc)(char* destModName,int messageID,void* inMessage,void* outResult);


//��ȡ����ģ�麯��ָ��,ʧ�ܷ���NULL.
//���е�destModName��Ϊ��,Ϊ��ʱ��ÿһ��ģ�鶼�������funcName,��Ϊ��ʱ,destModName��ָPluginInfo����ģ���ṩ��nameֵ
typedef void* (*GetModFunc)(char* destModName, char* funcName);

//��ȡ�û�����
//�û���CFrameWork::Init()�ӿ��������û����ݣ�����ģ��ͨ���ýӿڻ�ȡ�û�����
typedef void (*GetUserDataFunc)(void **pUserData, Int32* pUserDataLen);


// ��ȡ���֤�е�У����, �� "validdate":"2017-12-31"    "videonum":"60"
// ����ֵ0�ɹ��������ο�EnumCheckResult
typedef int (*GetLicenceValueFunc)(const char* szKey, char *szValBuf, int iBufLen);

typedef struct _SystemInfo
{

	//������Щ����������Ա��棬��ʱ����
	LogFunc log;

	//������Ϣ����
	SendMessageFunc sendMsg;

	//��ѯ��Ϣ�ͻ����ⲿ��ַ
	QueryConnectionFunc queryConn;

	//��ȡ���ú���
	GetConfigFunc getConfig;

	//�����������
	AddHeartBeatFunc addHeartBeat;

	//��ʱ������
	StartTimerFunc   startTimer;
	StopTimerFunc    stopTimer;


	//��ȡ���ͷ����ݿ����Ӻ���
	GetDBConnectionFunc        getDBConnection;
	ReleaseDBConnectionFunc    releaseDBConnection;


	//��ȡ����ģ������˽�����ݣ����ģ�����Ҫ�������ݣ�����ͨ���˺�����ȡ
	GetPluginInfoFunc          getPluginInfo;

	//��ȡ���ĵ�����
	GetSubscriptionsFunc	  getSubscriptions;
	//׷�����ⶩ��
	AppendSubscriptionFunc		appendSubscription;

	//֪ͨ����ģ����Ϣ����
	NotifyModMessageFunc           notifyModMessage;

	//��ȡ����ģ��ĺ���ָ��
	GetModFunc                    getModFunc;

	//��ȡ�û�����
	//�û���CFrameWork::Init()�ӿ��������û����ݣ�����ģ��ͨ���ýӿڻ�ȡ�û�����
	GetUserDataFunc				  getUserDataFunc;

	// ��ȡ���֤�е�У����
	GetLicenceValueFunc      getLicenceValueFunc;

    char serviceName[64];//��������
	char ServiceID[64];//����ID,����Ϣ���ߵ�ͨ������
	char DomainID[64];//����ID,��ʶ��һ����Ϣ���ߵ�Ԫ�Ĳ�ͬ�����ID

	_SystemInfo():log(NULL),sendMsg(NULL),getConfig(NULL),addHeartBeat(NULL),startTimer(NULL),stopTimer(NULL),getDBConnection(NULL),releaseDBConnection(NULL),getPluginInfo(NULL),getSubscriptions(NULL),appendSubscription(NULL),notifyModMessage(NULL),getModFunc(NULL),getUserDataFunc(NULL),getLicenceValueFunc(NULL)
	{
		memset(serviceName,0,sizeof(serviceName));
		memset(ServiceID,0,sizeof(ServiceID));
		memset(DomainID,0,sizeof(DomainID));
	}

}SystemInfo;


//����key-value��ʽ����Ϣ
typedef struct _KV_Info
{
	char key[128];
	char value[512];

} KV_Info;


//http������Ҫ��ѯ��ʾ����Ϣ
typedef struct _HttpQueryInfos
{
	int      infoCnt;
	KV_Info  info[MAX_INFO_NUM];

	_HttpQueryInfos():infoCnt(0)
	{
		memset(info,0,sizeof(info));
	}

} HttpQueryInfos;


//����ģ��Ҫ�ṩ������ģ����õĺ���ָ����Ϣ
typedef struct _Func_Info
{
	char  name[128]; //��������
	void* func;   //����ָ��
	_Func_Info()
	{
		memset(name,0,sizeof(name));
		func = NULL;
	}

} Func_Info;
#define MAX_FUNC_INFO 16


//�����Ϣ�ṹ�壬����Ҫ����ṩ����ܵĸ�����Ϣ
typedef struct _PluginInfo
{

	size_t version; //����汾��
	char name[64]; //�������

	//������붨��ĳ�ʼ����������ܳ�ʼ����ʱ�����øú�����ɲ����ʼ��
	Int32 (* init)(SystemInfo* sys,void** pluginData);


	//������붨��Ŀ�ʼ��������ܿ�ʼ��ʱ�����øú�����ɲ������
	Int32 (* start)(SystemInfo* sys,void* pluginData);

	
	//������붨�����Ϣ��������������е�ʱ�����øú��������Ϣ����
	Int32 (*handleMsg)(SystemInfo* sys,const MessageInfo& msg,void* pluginData);


	//������붨���ֹͣ���������ֹͣ��ʱ�����øú���ֹͣ���
	Int32 (* stop)(SystemInfo* sys,void* pluginData);


	//������붨��������������ֹͣ��ʱ�����øú�����ɲ������
	Int32 (*cleanup)(SystemInfo* sys,void* pluginData);


	//���������¼�֪ͨ�������������¼�(���������ӶϿ�)��ͨ���˽ӿ�֪ͨ���
	Int32 (*eventNotify)(SystemInfo* sys,ENUM_EVENT_TYPE eventType,void* pluginData);


	//��������http��ѯ�ӿڣ�����http�����ܵ�ʱ�򣬿�ܵ��øýӿڣ����ͨ��pHttpQueryInfos�������infoչ�ֵ�ҳ��
	Int32 (*httpQuery)(SystemInfo* sys,HttpQueryInfos* pHttpQueryInfos,void* pluginData);


	//��������ģ����Ϣ֪ͨ�ӿڣ�������ģ�����������Ϣ֪ͨ
	Int32 (*modMessageNotify)(SystemInfo* sys,int messageID ,void* inMesssage, void* outResult, void* pluginData);


	//ģ����Ҫ�ṩ������ģ����õ���������ָ����Ϣ(����Ҫ��Ч�������ݵĳ��Ͽ���ͨ������ĳЩ����ʵ�ֿ������ݴ��ݣ�һ������²�����ʹ�ø÷�ʽͨ��)
	Func_Info  otherFuncInfo[MAX_FUNC_INFO];


    //���ʹ�ó�Ա,ģ��������ʹ��
	void* lib;
	void* pd;


	_PluginInfo():version(0),init(NULL),start(NULL),handleMsg(NULL),stop(NULL),cleanup(NULL),eventNotify(NULL),httpQuery(NULL),lib(NULL),pd(NULL),modMessageNotify(NULL)
	{

		memset(name,0,sizeof(name));
		

	}
	
} PluginInfo;




#endif
