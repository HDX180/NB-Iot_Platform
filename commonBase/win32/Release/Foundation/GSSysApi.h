/*
******************************************
Copyright (C): GOSUN
Filename : GSSYSAPI.H
Author :  zouyx
Description: 平台相关函数
********************************************
*/

#ifndef _GS_H_GSSYSAPI_H_
#define _GS_H_GSSYSAPI_H_

#ifndef _FILE_OFFSET_BITS 
#define _FILE_OFFSET_BITS 64
#endif
#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif
#include <stdio.h>

#include "GSFdtDefs.h"




typedef enum 
{
	OS_SUCCESS = 0,
	OS_FAIL = -1,
	OS_E_EXIST = -2, //已经存在
	OS_NOT_EXIST = -3, //对象不存在
	OS_NOT_SUPPORT = -4, //不支持的功能
	OS_E_BREAK = -5, //操作给用户中断
	OS_E_INVALID = -6, //参数错误
	OS_E_IO = -7, //IO 错误
}EnumGSOSErrno;











/*
*********************************************************************
*
*@brief : 
*
*********************************************************************
*/

// 休眠
GSFDT_API void GSSleep( int iMilSeconds );

//调用线程让出CPU 时间
GSFDT_API void GSThreadYield(void);

//返回系统的总核数
GSFDT_API int GSGetCPUProcessorNumber(void);

//返回程序目录, 带结尾 /
GSFDT_API const char *GSGetApplicationPath(void);

// 获取当前进程内存状态, 单位 KByte
GSFDT_API void GSGetAppMemoryStatus( unsigned long& iWorkingSetSize, 
								 unsigned long& iPagefileUsage );


//原子变量操作
GSFDT_API void GSAtomicSet(GSAtomic &iAtomic, long iVal);

GSFDT_API long GSAtomicGet(GSAtomic &iAtomic);

// 对*pAtomic 进行递增操作， 返回增加后的值
GSFDT_API long GSAtomicInc(GSAtomic &iAtomic);
// 对*pAtomic 进行递减操作， 返回递减后的值
GSFDT_API long GSAtomicDec(GSAtomic &iAtomic);
//比较iAtomic 是否与 iOldVal 相等， 如果相等，把 iAtomic 设定为 iNewVal, 并返回TRUE， 否则返回FALSE
GSFDT_API bool GSAtomicCompareExchange(GSAtomic &iAtomic, 
							const long iOldVal, const long iNewVal);

//返回 and 前的值
GSFDT_API long GSAtomicAnd(GSAtomic &iAtomic, long iVal);

//返回 or 前的值
GSFDT_API long GSAtomicOr(GSAtomic &iAtomic, long iVal);


GSFDT_API void GSAtomicPointerSet(void * volatile *pAtomic, 
								  const void *pNewVal);

//比较pAtomic 是否与 pOldVal 相等， 如果相等，把 pAtomic 设定为 pNewVal, 并返回TRUE， 否则返回FALSE
GSFDT_API bool GSAtomicCompareExchangePointer(void * volatile *pAtomic, 
									void *pOldVal, void *pNewVal);
/*
*********************************************************************
*
*@brief : 文件相关函数
*
*********************************************************************
*/

// 获取空间大小, 单位 Bytes
GSFDT_API EnumGSOSErrno GSGetSpaceSize( const char* szDirName,
									 unsigned long long& lTotal,
									 unsigned long long& lUsed,
									 unsigned long long& lFree );


// 创建目录 返回 SO_E_EXIST 表示目录已经存在
GSFDT_API EnumGSOSErrno GSMakeDir( const char* szPath );
// 递归创建目录
GSFDT_API EnumGSOSErrno GSRecursiveMakeDir( const char* szPath );

// 移除目录
GSFDT_API EnumGSOSErrno GSRemoveDir( const char* szFileName );
// 强制移除目录（先删除改目录所有文件,再移除目录）
GSFDT_API EnumGSOSErrno GSForcedRemoveDir( const char* szFileName );

// 文件夹是否空
GSFDT_API bool GSIsFolderEmpty( const char* szDir );

// 是否为文件夹,OS_SUCCESS 表示是文件夹， OS_FAIL 表示非文件夹, 返回 OS_NOT_EXIST 表示文件不存在，OS_E_IO 系统检测函数失败 
GSFDT_API EnumGSOSErrno GSIsFolder( const char* szPathname );

// 是否为普通文件
GSFDT_API EnumGSOSErrno GSIsFile( const char* szFilename );


//磁盘类型
typedef enum
{
	GS_DISK_UNKNOWN  = 0, //未知类型
	GS_DISK_FIXED = 1,    //本地硬盘
	GS_DISK_CDROM = 2,  
	GS_DISK_REMOVEAABLE = 3,  //可热插拔存储  如 usb
	GS_DISK_REMOTE = 4,   //网络硬盘
	GS_DISK_NOROOTDIR = 5,  //没有卷加载的目录
	GS_DISK_RAMDISK = 6,   //内存盘
} GSDiskType;

typedef struct _GSStruDiskInfo
{
	const char *szName;  //结尾不带目录分割符 "/或\"
	GSDiskType eType;	
}GSStruDiskInfo;

//返回 false 表示中断继续， true 表示继续, 返回 OS_E_BREAK
typedef bool (*GSFunPtrDiskCallback)( const GSStruDiskInfo &info, void *pUserData);

//枚举所有磁盘信息
GSFDT_API EnumGSOSErrno GSFetchDiskInfo(GSFunPtrDiskCallback fnFetchCallback,
										void *pUserData);




//返回 false 表示中断继续， true 表示继续, 返回 OS_E_BREAK
typedef enum
{
	GS_IFOTHER = 0, //其他文件 
	GS_IFREG =  1, // 普通文件
	GS_IFDIR = 2, //目录文件
	GS_IFDISK = 3, //磁盘	
}GSFileType;

typedef struct _GSStruFileInfo
{
	const char *szName;
	GSFileType eType;	
}GSStruFileInfo;
typedef bool (*GSFunPtrFileCallback)( const GSStruFileInfo &info, void *pUserData);




//检索目录文件  
GSFDT_API EnumGSOSErrno GSFetchDir( const char* szDir, 
									GSFunPtrFileCallback fnFetchCallback,
									void *pUserData);


// 获取空文件夹列表
GSFDT_API EnumGSOSErrno GSGetEmptyFolderList( const char* szDir, 
											 GSFunPtrFileCallback fnEmptyFolderCallback,
											 void *pUserData);

// 获取磁盘列表
GSFDT_API EnumGSOSErrno GSGetDiskList( GSFunPtrFileCallback fnDiskCallback,
									  void *pUserData);

// 文件是否存在, 返回 OS_SUCCESS, OS_FAIL, OS_NOT_EXIST
GSFDT_API EnumGSOSErrno GSIsFileExist( const char* szFileName );

// 移除文件
GSFDT_API EnumGSOSErrno GSRemove( const char* szFileName );

// 获取文件大小 返回 < 0 表示失败, 表示错误号 EnumGSOSErrno
GSFDT_API Int64 GSGetFileSize( const char* szFileName );

// 获取文件最后修改时间, 返回 <0  表示失败, 表示错误号 EnumGSOSErrno, 
// 其他返回 Epoch UTC, 自 1970-01-01 00:00:00 的秒数
GSFDT_API Int64 GSGetLastModifiedTime( const char* szFileName );

//获取文件信息， 等同于 GSGetFileSize、GSGetLastModifiedTime
GSFDT_API EnumGSOSErrno GSGetFileInfo(const char* szFileName, UInt64 *pLastModifiedTime, UInt64 *pFileSize );


//获取本地IP 列表
#define GS_MAX_IP_LEN 128
typedef struct _StruGSIfAddr
{
	bool isIPV6;
	char czIP[GS_MAX_IP_LEN];
	char czSubnetMartk[GS_MAX_IP_LEN];
	char czGateway[GS_MAX_IP_LEN];	
}StruGSIfAddr;

//失败返回 <0， 其他表示 ppAddr 个数, ppAddr返回的结果集， 成功后调用 GSFreeIFAddrLists释放
GSFDT_API int GSGetIFAddrLists( StruGSIfAddr **ppAddr );
GSFDT_API void GSFreeIFAddrLists( StruGSIfAddr *pAddr);
//判断是否在同一个子网
GSFDT_API bool GSIsSubnet(const char *czIpSrc,const char *czIpDst, const char *czSubnetMark);

//为远程客户IP=czPeerIp 的机器提供本机的连接IP
// czPeerIp 对端IP， 
// czLocalp 返回本地使用的IP
GSFDT_API bool GSGuessLocalIp( const char *czPeerIp, char czLocalp[GS_MAX_IP_LEN]);

//生成UUID，用户唯一识别字符串
GSFDT_API const char *GSGenerateUUID(char (&buffer)[64]);

//打开进程,bShowWindow:是否后台执行    // 失败返回-1,成功返回进程ID
GSFDT_API long GSOpenProcess( const char* szProcessPath , bool bShowWindow,
							 int argc,const char *argv[] );

//关闭进程,millisecond:等待进程退出的超时时间，单位为毫秒,-1则等待直到进程退出为止
GSFDT_API bool GSCloseProcess( long lProcessID,int millisecond = 3000);

//终止进程
GSFDT_API bool GSTerminateProcess( long lProcessID );

//进程是否存在
GSFDT_API bool GSIsProcessAlive( long lProcessID );


// 枚举指定路径的进程
//szProcessPath:指定路径
//lProcessID:保存进程ID的数组，由函数写入相应进程ID
//iProcessNum:传入进程ID数组的大小，函数执行后填入指定路径下进程的实际个数
GSFDT_API bool GSGetAliveProcessList( const char* szProcessPath, long *lProcessID, int &iProcessNum );

//动态库动态加载

//加载动态库， 失败返回 NULL， 成功需要调用 GSFreeLibrary释放
// czDllFilename  动态库文件名（包括路径）
GSFDT_API void *GSLoadLibrary(const char *czDllFilename);
//释放动态库 pModule GSLoadLibrary返回值 
GSFDT_API void GSFreeLibrary(void *pModule);
//获取动态库函数, 失败返回 NULL
//pModule 动态库句柄
//czFounctionName  函数名
GSFDT_API void *GSGetProcAddress(void *pModule, const char *czFounctionName);

//获取当前路径
//szPathBuf  返回的目录缓冲, 不能为NULL
//nBufSize  szPathBuf 的大小
//失败返回 NULL， 成功返回指向 szPathBuf 的指针
GSFDT_API const char * GSGetCurrentDir( char *szPathBuf, int nBufSize );

//设置当前路径
GSFDT_API bool GSSetCurrentDir( const char *czPath);

//获取动态库搜索的路径
//szPathBuf  返回的目录缓冲, 不能为NULL
//nBufSize  szPathBuf 的大小
//失败返回 NULL， 成功返回指向 szPathBuf 的指针
GSFDT_API const char *GSGetDllDir(char *szPathBuf, int nBufSize);

//设置动态库搜索的路径
GSFDT_API bool GSSetDllDir(const char *czPath);


//获取本地MAC 列表
#define GS_MAX_MAC_LEN 128
typedef struct _StruGSMacAddr
{
	bool isPhysicalAdapter;
	char czMacAddr[GS_MAX_MAC_LEN];
}StruGSMacAddr;

//失败返回 <0， 其他表示 MacAddr 个数, 
//ppAddr:返回的结果集,成功后调用 GSFreeMacAddrLists释放
GSFDT_API int GSGetMacAddrLists(StruGSMacAddr **ppAddr);
GSFDT_API void GSFreeMacAddrLists( StruGSMacAddr *pAddr);


//获取CPU版本信息
GSFDT_API const char *GSGetCPUVersion(void);

//获取当前进程CPU使用率，成功返回使用率(百分比)，失败返回-1
GSFDT_API int GSGetCurProcessCpuUsage(void);

//获取当前进程名称
GSFDT_API const char *GSGetCurProcessName(void);

//获取当前进程的线程数目，成功返回线程数目，失败返回-1
GSFDT_API int GSGetCurProcessThreadNum(void);

//获取当前进程IO统计信息，成功返回0,失败返回-1
GSFDT_API int GSGetCurProcessDiskIOBytes(unsigned long long* read_bytes, unsigned long long* write_bytes);

//获取操作系统版本信息
GSFDT_API const char *GSGetOSVersion(void);

//获取系统物理内存大小,成功返回物理内存大小,失败返回-1
GSFDT_API unsigned long long GSGetTotalPhysicalMemory(void);



// IO 操作函数  , 支持64 位，具体用法参考 c   stream i/o

#define gsfile FILE
#define gsfpos_t  fpos_t
#define gsoff_t Int64

#define GS_FSEEK_CUR SEEK_CUR
#define GS_FSEEK_END SEEK_END
#define GS_FSEEK_SET SEEK_SET


#define gsfopen(filename, mode) fopen(filename,mode)
#define gsfclose(stream) fclose(stream)
#define gsfread(buffer,size,count,stream) fread(buffer,size,count,stream)
#define gsfwrite(buffer,size,count,stream) fwrite(buffer,size,count,stream)
#define gsferror(stream)  ferror(stream)
#define gsfeof(stream) feof(stream)
#define gsrewind(stream) rewind(stream)
#define gsfflush(stream) fflush(stream)
#define gsfgetc(stream)  fgetc(stream)
#define gsfputc(c, stream)	fputc(c, stream)
#define gsfputs(str, stream)	fputs(str, stream)
#define gsfgets(strbuf,bufsize,stream) fgets(strbuf,bufsize,stream)
#define gsfgetpos(stream, posptr) fgetpos( stream, posptr)
#define gsfsetpos(stream, pposptr) fsetpos( stream, pposptr)
#define gsfscanf  fscanf





#ifdef _MSWINDOWS_

#define gsfseek(stream, offset,origin) _fseeki64(stream, offset,origin )
#define gsftell(stream ) _ftelli64(stream)
#define gsclearerr(stream)  clearerr_s(stream)
   
#else

#define gsfseek(stream, offset,origin) fseeko(stream, offset,origin )
#define gsftell(stream ) ftello(stream)
#define gsclearerr(stream)  clearerr(stream)

#endif


#endif //end _GS_H_GSSYSAPI_H_
