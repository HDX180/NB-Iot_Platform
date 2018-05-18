/*
******************************************
Copyright (C): GOSUN
Filename : GSMEMORYPOOL.H
Author :  zouyx
Description: 内存池实现
********************************************
*/

#ifndef _GS_H_GSMEMORYPOOL_H_
#define _GS_H_GSMEMORYPOOL_H_

#include "GSFdtDefs.h"
#include "GxxList.hpp"

/*
*********************************************************************
*
*@brief : 内存池功能类， 分配固定大小的内存片，或固定大小倍数的内存片
*note :   本类非线程安全
*********************************************************************
*/
class GSFDT_CLASS GSMemoryPool
{
private :
	unsigned int m_nPartitionSize;
	unsigned int m_nSliceSize;
	unsigned int m_nNextCount;
	unsigned int m_nMaxCount;
	bool m_bAbleOrderedMalloc;
	unsigned int m_nCalloced; //已经分配的内存
	
	GxxListInner<char *> m_listCalloced; //分配的内存	
	UInt32 m_nFree;
	void *m_pFreeFirst;
public :
	// nSliceSize 内存池每个内存片的大小(Byte)，即 malloc 返回的大小
	// nNextCount 内存池每次增加的内存片个数, 最佳值小于等于连续分配内存片的大小
	// nMaxCount 内存池最大分配的内存片个数, 如果达到分配个数 malloc将返回NULL, 0 不限定
	// bAbleOrderedMalloc 是否使用分配多片连续内存片功能，如果明确不使用禁止该项，可以加快分配速度，默认使用
	GSMemoryPool(UInt32 nSliceSize,  UInt32 nNextCount = 32
				, UInt32 nMaxCount = 0,bool bAbleOrderedMalloc = true );
	~GSMemoryPool(void);

	//收缩内存， 释放掉内存池中空闲的内存
	//  nSliceReserveCounts 保留不释放的内存片个数, 0 全部释放
	void ShrinkMemory(UInt32 nReserveCounts = 32);


	UInt32 GetSliceSize(void) const;
	UInt32 GetNextCount(void) const;
	UInt32 GetMaxCount(void) const;
	void SetMaxCount(UInt32 nMaxCount);

	//返回空闲的内存片个数
	UInt32 GetFreeCount(void) const;

	//返回总的片数， 空闲+malloc的个数
	UInt32 GetTotalCount(void) const;


	//分配连续内存, 返回的内存大小等于nSliceSize*nCount
	void *Malloc(UInt16 nCount = 1);

	//分配连续内存, 返回 的内存大小nSize
	void *MallocEx(unsigned int nSize);


	// 释放 Malloc的返回值
	void Free(void *data);
	
	//是否任意由 内存池分配的内存， *建议少使用， 且只能在分配内存的同一dll 内使用
	static void FreeAny( void *data);
	
private :
	void *CallocNext( UInt16 nCounts );
	void AddOrdered(void *pPartition, unsigned int nPartition, void *pChunk);

	void *MallocN(UInt16 n);
	void *TryMallocN(void * & start, UInt16 n);

	 void * FindPrev(void *const pFreeFirst, void * pPartition);	
	 void *Segregate(void *pPartition, UInt16 nPartition, void *pEnd);

	 void *ChunckMallocSliceN(void *pChunk,UInt16 n);

	XC_CLASS_NOCOPY(GSMemoryPool);
};

#endif //end _GS_H_GSMEMORYPOOL_H_
