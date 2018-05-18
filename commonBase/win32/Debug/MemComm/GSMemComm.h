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


// �����붨��
#define GSMEMCOMM_SUCCESS 0
#define GSMEMCOMM_FAIL   -1
#define GSMEMCOMM_PARAM_ERROR   -2
#define GSMEMCOMM_UNINITIALIZED   -3

typedef void* GSMemCommHandle;


// �¼��ص� 0 ���ӳɹ� 1�Ͽ����� 2�쳣����
typedef void (GS_MEMCOMM_CALLBACK *FuncEventCallback)(GSMemCommHandle hChannel, int iStatus, void *pUserData );

// ��Ϣ�ص�
typedef void (GS_MEMCOMM_CALLBACK *FuncMessageCallback)( GSMemCommHandle hChannel,												  
														void *pData,
														int iLen,
														void *pUserData );

// ��ʼ��
GS_MEMCOMM_API int GSMemComm_Init( FuncEventCallback cbEvent, 
								  void* pEventUserData,
								  FuncMessageCallback cbMessage,
								  void *pMsgUserData );

// ��Դ�ͷ�
GS_MEMCOMM_API void GSMemComm_Cleanup();


// szChnName: ͨ����,Ψһ,���ҷ������ͻ��˱���һ��
// iMemSize: �ڴ�ռ��С,�ҷ������ͻ��˱���һ��
// ����ͨ��(�����)
GS_MEMCOMM_API int GSMemComm_CreateChannel( GSMemCommHandle *hChannel,
										   const char *szChnName, 
										   int iMemSize=512*1024 );

// ��ͨ��(�ͻ���)
GS_MEMCOMM_API int GSMemComm_OpenChannel( GSMemCommHandle *hChannel,
										 const char *szChnName,
										 int iMemSize=512*1024 );

// ����ͨ��
GS_MEMCOMM_API void GSMemComm_DestroyChannel( GSMemCommHandle hChannel );

// ������Ϣ
GS_MEMCOMM_API int GSMemComm_SendMsg( GSMemCommHandle hChannel, void *pData, int iLen );


#endif // GSMEMCOMM_DEF_H