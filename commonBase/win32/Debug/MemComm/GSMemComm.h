#ifndef GSMEMCOMM_DEF_H
#define GSMEMCOMM_DEF_H


#ifdef _WIN32

#ifdef GSMEMCOMM_STATIC
#define GS_MEMCOMM_API 
#else

#ifdef GSMEMCOMM_EXPORTS
#define GS_MEMCOMM_API extern "C" __declspec(dllexport)
#else
#define GS_MEMCOMM_API extern "C" __declspec(dllimport)
#endif

#define GS_MEMCOMM_CALLBACK
#endif

#elif _LINUX

#define GS_MEMCOMM_API extern "C" 
#define GS_MEMCOMM_CALLBACK
#endif


// 错误码定义
#define GSMEMCOMM_SUCCESS 0
#define GSMEMCOMM_FAIL   -1
#define GSMEMCOMM_PARAM_ERROR   -2
#define GSMEMCOMM_UNINITIALIZED   -3

typedef void* GSMemCommHandle;


// 事件回调 0 连接成功 1断开连接 2异常错误
typedef void (GS_MEMCOMM_CALLBACK *FuncEventCallback)(GSMemCommHandle hChannel, int iStatus, void *pUserData );

// 消息回调
typedef void (GS_MEMCOMM_CALLBACK *FuncMessageCallback)( GSMemCommHandle hChannel,												  
														void *pData,
														int iLen,
														void *pUserData );

// 初始化
GS_MEMCOMM_API int GSMemComm_Init( FuncEventCallback cbEvent, 
								  void* pEventUserData,
								  FuncMessageCallback cbMessage,
								  void *pMsgUserData );

// 资源释放
GS_MEMCOMM_API void GSMemComm_Cleanup();


// szChnName: 通道名,唯一,并且服务端与客户端保持一致
// iMemSize: 内存空间大小,且服务端与客户端保持一致
// 创建通道(服务端)
GS_MEMCOMM_API int GSMemComm_CreateChannel( GSMemCommHandle *hChannel,
										   const char *szChnName, 
										   int iMemSize=512*1024 );

// 打开通道(客户端)
GS_MEMCOMM_API int GSMemComm_OpenChannel( GSMemCommHandle *hChannel,
										 const char *szChnName,
										 int iMemSize=512*1024 );

// 销毁通道
GS_MEMCOMM_API void GSMemComm_DestroyChannel( GSMemCommHandle hChannel );

// 发送消息
GS_MEMCOMM_API int GSMemComm_SendMsg( GSMemCommHandle hChannel, void *pData, int iLen );


#endif // GSMEMCOMM_DEF_H