#ifndef _GS_PLUGIN_INFO_H_
#define _GS_PLUGIN_INFO_H_

#include<string>
#include"XCTypes.h"



//模块导出的初始化函数名称
#define MOD_EXPORT_INIT_FUNC_NAME  "PluginInit"
//模块导出的初始化函数原型
typedef int (*Plugin_Init)(struct _PluginInfo* pInfo);



//插件返回值 成功0 ;失败-1
#define SUCCEED  0
#define FAIL    -1


//消息总线相关数据结构
#include "message.h"


//##日志等级( 1:调试日志   2:信息日志   3:警告日志  4:错误日志  5:崩溃日志 )
#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 3
#define LOGGER_LEVEL_ERROR 4
#define LOGGER_LEVEL_FATAL 5


#define MAX_INFO_NUM 100


/********以下定义的是框架可以提供给插件模块使用的各种函数指针,通过_SystemInfo结构体传递给插件使用**********/
//日志函数
typedef int (*LogFunc)(const char* msg,Int32 severity,bool condition);


//获取配置函数
typedef char const*  (*GetConfigFunc)(char const* section, char const* key,char const* notfound);


//发送消息函数
typedef int (*SendMessageFunc)(char* modName,const MessageInfo &msg);

//查询消息客户端外部连接地址
typedef int (*QueryConnectionFunc)(char* modName,char const* address, char *content, int content_size);


//添加心跳函数
typedef void (*AddHeartBeatFunc)(const char* serverName,pHeartBeatCallback pHeartBeatCB,int intervalSecond);

typedef void * TimerHandle;
#define INVALID_TIMER_HANDLE NULL

//定时器回调函数,timerID是由StartTimer返回的ID
typedef void (*FunPtrTimerCallback)(struct _SystemInfo* sys, TimerHandle timerID, void *pTimerParam);
//开始定时器函数，单位:毫秒
typedef TimerHandle (*StartTimerFunc)(int nIntervalMS, FunPtrTimerCallback fnCb, void *pTimerParam);


//停止定时器，timerID是由StartTimer返回的ID
typedef void (*StopTimerFunc)(TimerHandle timerID);


class GSIConnection;
//获取数据库连接，每次GetDBConnection使用后必须记得ReleaseDBConnection.
//GetDBConnection返回错误码.0表示成功，其余请参考EnumGSDBApiRet定义
typedef int  (*GetDBConnectionFunc)(GSIConnection** pConnection);
typedef void (*ReleaseDBConnectionFunc)(GSIConnection** pConnection);


//获取其他模块插件的私有数据，如果模块间需要共享数据，可以通过此函数获取
//返回数据为模块init时创建的pluginData
//pluginName为模块init时填入PluginInfo里面的name
typedef void* (*GetPluginInfoFunc)(char* pluginName);


//获取订阅主题列表(格式:subject1/subject2/....)
typedef char const*  (*GetSubscriptionsFunc)(void);
//追加主题订阅
typedef void  (*AppendSubscriptionFunc)(const char* szSubject);


//通知其他模块处理消息,destModName为NULL的时候所有模块(包括自己)都会收到通知信息，此时必须保证传入参数outResult也是为NULL
typedef int (*NotifyModMessageFunc)(char* destModName,int messageID,void* inMessage,void* outResult);


//获取其他模块函数指针,失败返回NULL.
//其中的destModName可为空,为空时对每一个模块都查找相关funcName,不为空时,destModName是指PluginInfo中由模块提供的name值
typedef void* (*GetModFunc)(char* destModName, char* funcName);

//获取用户数据
//用户在CFrameWork::Init()接口中设置用户数据，其他模块通过该接口获取用户数据
typedef void (*GetUserDataFunc)(void **pUserData, Int32* pUserDataLen);


// 获取许可证中的校验项
// 返回值0成功，其他参考EnumCheckResult
typedef int (*GetLicenceValueFunc)(const char* szKey, char *szValBuf, int iBufLen);

typedef struct _SystemInfo
{

	//以下这些函数插件可以保存，随时调用
	LogFunc log;

	//发送消息函数
	SendMessageFunc sendMsg;

	//查询消息客户端外部地址
	QueryConnectionFunc queryConn;

	//获取配置函数
	GetConfigFunc getConfig;

	//添加心跳函数
	AddHeartBeatFunc addHeartBeat;

	//定时器函数
	StartTimerFunc   startTimer;
	StopTimerFunc    stopTimer;


	//获取和释放数据库连接函数
	GetDBConnectionFunc        getDBConnection;
	ReleaseDBConnectionFunc    releaseDBConnection;


	//获取其他模块插件的私有数据，如果模块间需要共享数据，可以通过此函数获取
	GetPluginInfoFunc          getPluginInfo;

	//获取订阅的主题
	GetSubscriptionsFunc	  getSubscriptions;
	//追加主题订阅
	AppendSubscriptionFunc		appendSubscription;

	//通知其他模块消息处理
	NotifyModMessageFunc           notifyModMessage;

	//获取其他模块的函数指针
	GetModFunc                    getModFunc;

	//获取用户数据
	//用户在CFrameWork::Init()接口中设置用户数据，其他模块通过该接口获取用户数据
	GetUserDataFunc				  getUserDataFunc;

	// 获取许可证中的校验项
	GetLicenceValueFunc      getLicenceValueFunc;

    char serviceName[64];//服务名称
	char ServiceID[64];//服务ID,是消息总线的通信依据
	char DomainID[64];//区域ID,标识以一条消息总线单元的不同区域的ID

	_SystemInfo():log(NULL),sendMsg(NULL),getConfig(NULL),addHeartBeat(NULL),startTimer(NULL),stopTimer(NULL),getDBConnection(NULL),releaseDBConnection(NULL),getPluginInfo(NULL),getSubscriptions(NULL),appendSubscription(NULL),notifyModMessage(NULL),getModFunc(NULL),getUserDataFunc(NULL),getLicenceValueFunc(NULL)
	{
		memset(serviceName,0,sizeof(serviceName));
		memset(ServiceID,0,sizeof(ServiceID));
		memset(DomainID,0,sizeof(DomainID));
	}

}SystemInfo;


//基于key-value形式的信息
typedef struct _KV_Info
{
	char key[128];
	char value[512];

} KV_Info;


//http请求需要查询显示的信息
typedef struct _HttpQueryInfos
{
	int      infoCnt;
	KV_Info  info[MAX_INFO_NUM];

	_HttpQueryInfos():infoCnt(0)
	{
		memset(info,0,sizeof(info));
	}

} HttpQueryInfos;


//定义模块要提供给其他模块调用的函数指针信息
typedef struct _Func_Info
{
	char  name[128]; //函数名称
	void* func;   //函数指针
	_Func_Info()
	{
		memset(name,0,sizeof(name));
		func = NULL;
	}

} Func_Info;
#define MAX_FUNC_INFO 16


//插件信息结构体，是需要插件提供给框架的各种信息
typedef struct _PluginInfo
{

	size_t version; //插件版本号
	char name[64]; //插件名称

	//插件必须定义的初始化函数，框架初始化的时候会调用该函数完成插件初始化
	Int32 (* init)(SystemInfo* sys,void** pluginData);


	//插件必须定义的开始函数，框架开始的时候会调用该函数完成插件运行
	Int32 (* start)(SystemInfo* sys,void* pluginData);

	
	//插件必须定义的消息处理函数，框架运行的时候会调用该函数完成消息处理
	Int32 (*handleMsg)(SystemInfo* sys,const MessageInfo& msg,void* pluginData);


	//插件必须定义的停止函数，框架停止的时候会调用该函数停止插件
	Int32 (* stop)(SystemInfo* sys,void* pluginData);


	//插件必须定义的清理函数，框架停止的时候会调用该函数完成插件清理
	Int32 (*cleanup)(SystemInfo* sys,void* pluginData);


	//插件定义的事件通知函数，框架相关事件(如网络连接断开)会通过此接口通知插件
	Int32 (*eventNotify)(SystemInfo* sys,ENUM_EVENT_TYPE eventType,void* pluginData);


	//插件定义的http查询接口，当有http请求框架的时候，框架调用该接口，插件通过pHttpQueryInfos传递相关info展现到页面
	Int32 (*httpQuery)(SystemInfo* sys,HttpQueryInfos* pHttpQueryInfos,void* pluginData);


	//插件定义的模块消息通知接口，供其他模块如果进行消息通知
	Int32 (*modMessageNotify)(SystemInfo* sys,int messageID ,void* inMesssage, void* outResult, void* pluginData);


	//模块需要提供给其他模块调用的其他函数指针信息(在需要高效传递数据的场合可以通过定义某些函数实现快速数据传递，一般情况下不建议使用该方式通信)
	Func_Info  otherFuncInfo[MAX_FUNC_INFO];


    //框架使用成员,模块插件无需使用
	void* lib;
	void* pd;


	_PluginInfo():version(0),init(NULL),start(NULL),handleMsg(NULL),stop(NULL),cleanup(NULL),eventNotify(NULL),httpQuery(NULL),lib(NULL),pd(NULL),modMessageNotify(NULL)
	{

		memset(name,0,sizeof(name));
		

	}
	
} PluginInfo;




#endif
