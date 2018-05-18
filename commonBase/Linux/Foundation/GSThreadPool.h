/*
******************************************
Copyright (C): GOSUN
Filename : GSTHREADPOOL.H
Author :  zouyx
Description: 线程池
********************************************
*/

#ifndef _GS_H_GSTHREADPOOL_H_
#define _GS_H_GSTHREADPOOL_H_
#include "GSThread.h"
#include <list>
#include <vector>
#include <set>
#include "GxxList.hpp"

typedef  void  (GS_CALLBACK *FunPtrThreadPoolCallback)( void *pUserParam  );

class GSFDT_CLASS GSThreadPool
{
private :
	typedef struct _StruTask
	{
		FunPtrThreadPoolCallback fnOnCallback;
		void *pUserParam;
	}StruTask;
	int m_minCapacity;
	int m_maxCapacity;
	int m_idleTime; //单位秒
	GSMutex m_mutex;
	GSCond m_cond;
	std::vector<GSThread *> *m_threads;
	std::list<StruTask> *m_tasks;
	bool m_isrun;
	int m_idles; //空闲线程
	int m_nCapacity;
public:
	/*
	minCapacity 线程池空闲时最小保留的线程数
	maxCapacity 线程池最大启动线程数
	idleTime 线程空闲多久后会销毁， 单位 秒 , <1 表示不退出
	*/
	GSThreadPool(int minCapacity = 2,int maxCapacity = 16,int idleTime = 60);
	~GSThreadPool(void);

	//增加或减少线程数，是否需要？
	void AddCapacity(int n);


	int GetCapacity(void) const;//返回线程池当前线程数


	void StopAll(void); //停止线程池


	void JoinAll(void);//等待所有线程结束


	bool Start(FunPtrThreadPoolCallback fnOnCallback,void *pUserParam);

	bool IsSelfThread( long threaID);

	//返回等待调度的线程数
	int GetWaits(void);

private :
	static  void GS_CALLBACK ThreadProxy(GSThread &thread, void *data);
	void ThreadEntry(GSThread &thread);

	void AddThread(void);
	GS_CLASS_NOCOPY(GSThreadPool)
};


class GSTaskPool;
class GSTaskPoolCore;

//引用线程池 pUserParam 线程调用数据由 start 传入, lThreadID 线程ID
typedef  void  (GS_CALLBACK *FunPtrRefThreadPoolCallback)( void *pUserParam, long lThreadID  );

//引用线程池
class GSFDT_CLASS GSRefThreadPool
{
private :	
	friend class GSTaskPool;
	friend class GSTaskPoolCore;
	class GSFDT_CLASS MyThread : public GSThread
	{
	public :
		MyThread(void);
		virtual ~MyThread(void);
		GSCond &GetCond(void);
		void Signal(void);
	};
	typedef struct GSFDT_CLASS _StruRefTask
	{
		FunPtrRefThreadPoolCallback fnOnCallback;
		void *pUserParam;
		GSTaskPoolCore *pCore;
		_StruRefTask() : fnOnCallback(NULL),pUserParam(NULL),pCore(NULL)
		{

		}
	}StruRefTask;

	typedef GxxListInner<StruRefTask> ListTask;
	typedef ListTask::MyListNode  MyTaskNode;


	int m_minCapacity;
	int m_maxCapacity;
	int m_iIdleTime; //单位秒
	GSMutex m_mutex;
	GxxListInner<MyThread*> m_listThreads;
	GxxListInner<MyThread*> m_listIdleThreads;
	ListTask m_listTask;
	ListTask m_listTaskCache;
	bool m_isRun;	
	int m_nCapacity;
	GSAtomic m_nRefs;
private :
	//以下线程池使用
	typedef GxxListInner<void *> ListTPLTask;
	typedef ListTPLTask::MyListNode  ListTPLTaskNode;

	GSMutex m_mutexTPCache;
	ListTPLTask m_listTPLCache;


	


public :
	//建立线程池 maxThread 最大线程数 0 由系统分配
	//idleTime 最大空闲时间 单位秒, 如果线程在最大空闲时间内没有任务， 该线程将退出
	static GSRefThreadPool *Create( int maxThread = 0 , int idleTime=60);
	//返回线程池当前线程数
	int GetCapacity(void) const;
	void StopAll(void); //停止线程池
	void JoinAll(void);//等待所有线程结束
	bool Start(FunPtrRefThreadPoolCallback fnOnCallback,void *pUserParam);	
	//返回等待调度的线程数
	int GetWaits(void);
	//减少引用, 没人引用时对象将释放
	void Unref(void);
	//增加引用
	void Ref(void);

	static GSRefThreadPool &Global(void);
protected :
	GSRefThreadPool(int minCapacity,int maxCapacity, int idleTime );
	~GSRefThreadPool(void);


	void Clean(GSTaskPoolCore *pCore, ListTask &listObj);

	void FreeCleanResult(ListTask &listObj);


	bool Start(FunPtrRefThreadPoolCallback fnOnCallback,void *pUserParam, GSTaskPoolCore *pCore);	

	static  void GS_CALLBACK ThreadProxy(GSThread &thread, void *data);
	void ThreadEntry(GSThread &thread);

	void AddThread(void);

	static MyTaskNode *CallocTaskNodeFromCache(void *pUserData);
	static void FreeTaskNodeToCache(MyTaskNode *p,void *pUserData);

	static MyTaskNode *NewTaskNode(void *pUserData);
	static void DelTaskNode(MyTaskNode *p,void *pUserData);


	static ListTPLTaskNode *NewTPLTaskNode(void *pUserData);
	static void DelTPLTaskNode(ListTPLTaskNode *p,void *pUserData);

	ListTPLTaskNode *GetTPLTaskNode(void);
	void FreeTPLTaskNode(ListTPLTaskNode *pNode);


	GS_CLASS_NOCOPY(GSRefThreadPool)
};


class GSTaskPool;

//任务队列回调 pTaskData 任务数据，pUserData 用户数据透传init 的用户数据
typedef void (GS_CALLBACK *FunPtrTaskPoolCallback)(void *pTaskData, void *pUserData);

//任务队列
class GSFDT_CLASS GSTaskPool
{
public :
	 enum
	 {
		eRetNo_SUCCESS = 0,
		eRetNo_EFAILURE = -1,
		eRetNo_ENINIT = -2, //没有初始化
		eRetNo_EFLOWOUT = -3, //溢出,达到设定的最大等待任务数
		eRetNo_EPAUSE = -4,//在暂停
		eRetNo_EINVALID = -5,
	 }EnumRetNo;
private :
	GSTaskPoolCore *m_pCore;
public :
	// lMaxWaitTasks 最大等待任务数据 0 不限定
	// iThreadNums 并发执行的线程数 > 0 
	GSTaskPool(long lMaxWaitTasks = 0, int iThreadNums =  1);
	~GSTaskPool(void);

	// Init 只能调用一次, Init 和 Unint 配对使用
	// pRefThreadPool 使用的线程池， 如果 为 NULL 将使用 GSRefThreadPool::Global 线程池
	// fnTaskCb 任务回调不能为NULL
	// fnFreeTaskDataCb 释放数据回调，所有成功添加的没有被fnTaskCb(如果Clean/或对象释放)任务，
	//   将调用该函数释放对象
	// pUserData 透传给回调的用户参数
	// 成功返回 0 ， 错误返回 EnumRetNo
	int Init(GSRefThreadPool *pRefThreadPool,
			 FunPtrTaskPoolCallback fnTaskCb,
			 FunPtrTaskPoolCallback fnFreeTaskDataCb,
			 void *pUserData);

	// 该函数进行线程同步， 等待所有的回调停止后才返回
	// bWaitComplete 如果为true 将等待所有任务操作完成后返回
	// 否则 没执行的任务数据将调用释放回调（如果有设置) 后返回
	void Uninit( bool bWaitComplete = false );

	//判断是否有初始化
	bool IsInit(void);
	
	//判断是否可以试用， 如果暂停、没初始化或已经卸载返回 false
	bool IsReady(void);

	//添加任务 	
	// pTaskData 任务数据
	// bPriority 释放有效执行
	// 如果等待任务数达到设定的最大值（如果有设置）将返回 eRetNo_EFLOWOUT 
	// 成功返回 0 ， 错误返回 EnumRetNo
	int Task(void *pTaskData, bool bPriority=false);

	// 该函数进行线程同步， 等待所有的回调停止后才返回
	// bWaitComplete 参考 Uninit
	void Pause( bool bWaitComplete = false);
	
	// 继续执行
	// 成功返回 0 ， 错误返回 EnumRetNo
	int Resume(void);

	//清理等待的任务数据
	//如果有设置fnFreeTaskDataCb 回调， 用户数据将调用回调释放
	void Clean(void);

	//返回等待执行的任务数据
	long GetWaitTaskCount(void);
	 
private :
	GS_CLASS_NOCOPY(GSTaskPool)
};


//类对象回调模式 任务池  T 类名
template <class T>
class CObjectTaskPool : public GSTaskPool
{
public :
	//对象任务回调
	// csTaskPoo 任务队列对象
	//TaskData 任务数据
#ifdef _MSWINDOWS
	typedef typename  void (T::*FunPtrObjectCallback)( CObjectTaskPool<T> &csTaskPoo, void *TaskData );
#else
	typedef  void (T::*FunPtrObjectCallback)( CObjectTaskPool<T> &csTaskPoo, void *TaskData );
#endif
private :
	T *m_pFunOwner;
	FunPtrObjectCallback m_fnTaskCb;
	FunPtrObjectCallback m_fnFreeTaskDataCb;
	void *m_pUserData;
public :
	//参考GSTaskPool
	CObjectTaskPool(long iMaxWaitTasks = 0, int iThreadNums =  1)
		: GSTaskPool(iMaxWaitTasks, iThreadNums)
		,m_pFunOwner(NULL)
		,m_fnTaskCb(NULL)
		,m_fnFreeTaskDataCb(NULL)
		,m_pUserData(NULL)
	{

	}

	virtual ~CObjectTaskPool(void)
	{
		Uninit(false);
	}

	// pRefThreadPool 参考 GSTaskPool::Init
	//  pFunOwner 回调对象指针
	// fnTaskCb   任务回调
	// fnFreeTaskDataCb  释放任务数据回调 参考 GSTaskPool::Init
	int Init(GSRefThreadPool *pRefThreadPool,
			T *pFunOwner,
			FunPtrObjectCallback fnTaskCb,
			FunPtrObjectCallback fnFreeTaskDataCb)
	{
		m_pFunOwner = pFunOwner;
		m_fnTaskCb = fnTaskCb;
		m_fnFreeTaskDataCb = fnFreeTaskDataCb;
		return GSTaskPool::Init(pRefThreadPool,DoTaskEventProxy,m_fnFreeTaskDataCb ? DoFreeTaskDataEventProxy : NULL, this );
	}


	//设置用户私有数据 
	void SetUserData(void *pData)
	{
		m_pUserData = pData;
	}
	//返回  SetUserData 设定值
	void *GetUserData(void)
	{
		return m_pUserData;
	}
private :
	static void GS_CALLBACK DoTaskEventProxy(void *pTaskData, void *pUserData)
	{
		CObjectTaskPool *p = (CObjectTaskPool*)pUserData;
		(p->m_pFunOwner->*(p->m_fnTaskCb))(*p, pTaskData );
	}
	static void GS_CALLBACK DoFreeTaskDataEventProxy(void *pTaskData, void *pUserData)
	{
		CObjectTaskPool *p = (CObjectTaskPool*)pUserData;
		if( p->m_fnFreeTaskDataCb )
		(p->m_pFunOwner->*(p->m_fnFreeTaskDataCb))(*p, pTaskData );
	}
};

/*
*********************************************************************
*
*@brief : 使用了线程池功能的程序， 在主进程退出前掉资源释放
*
*********************************************************************
*/
GSFDT_API void GSThreadPoolGlobalCleanup(void);

/*
显示初始化线程池功能
*/
GSFDT_API void GSThreadPoolGlobalInit(void);

#endif //end _GS_H_GSTHREADPOOL_H_
