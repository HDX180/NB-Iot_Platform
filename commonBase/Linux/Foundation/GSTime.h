/*
******************************************
Copyright (C): GOSUN
Filename : GSTIME.H
Author :  zouyx
Description: 
********************************************
*/

#ifndef _GS_H_GSTIME_H_
#define _GS_H_GSTIME_H_
#include "GSFdtDefs.h"
#include <time.h>
#include "GSMutex.h"



//星期枚举定义, 0 表示星期日， 6 表示星期六
typedef enum 
/// Symbolic names for week day numbers (0 to 6).
{
	SUNDAY = 0,
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6
}DaysOfWeek;


/*
********************************************************************
类注释
类名    :    GSDateTime
作者    :    zouyx
类描述  :    时间日期操作类
*********************************************************************
*/


typedef enum
{
	//日期的格式化格式
	DT_FORMAT_D = 0,		//   yyyy-mm-dd   补零		
	DT_FORMAT_DF = 1,	    //   mm/dd/yyyy
	DT_FORMAT_DN = 2,       //yyyymmdd
	DT_FORMAT_DM = 3,		//  mm-dd-yyyy 补零
	DT_FORMAT_DMF = 4,		//  mm/dd/yyyy 补零



	//时间的个数化格式
	DT_FORMAT_T = 50,	 //   hh24:mm:ss
	DT_FORMAT_TM = 51,	 //   hh24:mm:ss.millisecond
	DT_FORMAT_TN = 52,	 //   hh24miss

}EnumDTFormat;


typedef Int64 GSTimeVal;


class GSFDT_CLASS GSDateTime
{
protected :
	int m_year;  //年 0 to 9999
	int m_month; //月 1 - 12
	int m_dayOfMonth;   //日 1 - 31
	int m_hour;  //时  24小时制 0 - 23
	int m_minute; //分  0-59
	int m_second;  //秒 0-59
	int m_millisecond; //毫秒 0-999
	int m_dayOfWeek; // 星期 0-6   0:星期日, 1:星期一 ... 6:星期六
//	GSTimeVal m_epochTv; //本地的 Epoch UTC 时间 
	char m_buf[64];
public :	
	GSDateTime(void);
	GSDateTime(const time_t &tv);
	GSDateTime(const GSDateTime &csDest);
	GSDateTime(int year, int month, int day, 
				int hour = 0, int minute = 0, int second = 0, int millisecond=0);

	~GSDateTime(void);


	bool Assign(int year, int month, int day, int hour = 0,
		        int minute = 0, int second = 0, int millisecond = 0);

	INLINE int Year(void) const
	{
		return m_year;
	}

	INLINE int Month(void) const
	{
		return m_month;
	}

	INLINE int MonthDay(void) const
	{
		return m_dayOfMonth;
	}

	INLINE int Hour(void) const
	{
		return m_hour;
	}

	INLINE int Minute(void) const
	{
		return m_minute;
	}

	INLINE int Second(void) const
	{
		return m_second;
	}

	INLINE int Millisecond(void) const
	{
		return m_millisecond;
	}

	INLINE int WeekDay(void) const
	{
		return m_dayOfWeek;
	}


	bool IsValid(void) const;   //判断时间是否有效

	bool GetLocalTime(void); //加载系统本地时间
	bool SetLocalTime(void); //设置系统本地时间


	time_t Timestamp(void) const;

	GSTimeVal GetEpochTime(void) const; //返回UTC 时间，自 1970-01-01 00:00:00 的秒数 等于 Timestamp
	bool   SetEpochTime(GSTimeVal tv);  //使用UTC 时间初始化结构

	bool AddYears( int nYears);	//加年
	bool AddMonths( int nMonths); //加月
	bool AddDays( int nDays);     //加天
	bool AddHours( int nHours);   //加小时
	bool AddSeconds( int nSeconds); //加秒
	bool AddMilliseconds( int nMilliseconds); //加毫秒

	bool SubYears( int nYears);	//减年
	bool SubMonths( int nMonths); //减月
	bool SubDays( int nDays);    //减天
	bool SubHours( int nHours);  //减小时
	bool SubSeconds( int nSeconds); //减秒
	bool SubMilliseconds( int nMilliseconds); //减毫秒

	
	bool operator<(const GSDateTime &dest) const;
	bool operator>(const GSDateTime &dest) const;
	bool operator==(const GSDateTime &dest) const;
	bool operator!=(const GSDateTime &dest) const;


	bool IsAM() const;
	/// Returns true if hour < 12;

	bool IsPM() const;
	/// Returns true if hour >= 12.


	
	//返回 yyyy-mm-dd hh24:mi:ss
	const char *Format(char *buf=NULL, int bufsize = 0) const;
	//返回格式化字符串,  delimiters 中间的分割符
	const char *Format( EnumDTFormat fmt, char *buf, int bufsize = 0 ) const;
	const char *Format( EnumDTFormat fmtfirst, EnumDTFormat fmtsecond, const char *delimiters=" ",
						char *buf = NULL, int bufsize = 0 ) const;

	//由 yyyy-mm-dd hh24:mi:ss
	bool SetFromFormat(const char *datetime );
	//由格式化值设置字符串
	bool SetFromFormat( const char *datetime, EnumDTFormat fmt );
	bool SetFromFormat( const char *datetime, EnumDTFormat fmtfirst,EnumDTFormat fmtsecond,
						const char *delimiters = " " );


	static UInt32   GetTickCount(void); //获取从操作系统启动到现在所经过的毫秒数，返回从操作系统启动到现在所经过的毫秒数, 49.7 天会循环

	static UInt64  GetPerformanceCounter(void);    // 高精度时间获取，返回毫秒数

	static UInt32   CountElapsed(UInt32 nowTick, UInt32 oldTick); //返回两个TICK 的时间流逝， 返回毫秒

	//返回系统 UTC 时间，自 1970-01-01 00:00:00 的时间流逝的毫秒数, 错误返回 MAX_UINT64
	static UInt64   GetEpochMilliseconds(void);

};


//计时器
class GSFDT_CLASS GSTimeMeter
{
private :
	UInt64 m_lasttv;
public :
	GSTimeMeter(void);
	~GSTimeMeter(void);

	//重置计时器
	void Reset(void);
	void Reset(UInt64 iLastTv);
	UInt64 GetElapsed(void) const; //返回流逝的毫秒数
};

//定时器

typedef struct _StruGSTimerRunThread StruGSTimerRunThread;
class GSTimerGlobal;

class GSFDT_CLASS GSTimer
{
public :
	//定时器回调
	typedef void (GS_CALLBACK *FunPtrTimerCallback)(GSTimer &timer, void *pTimerParam);
private:
	friend class GSTimerGlobal;
	long m_timerID;	
	bool m_isStarted;
	int m_interval;
	FunPtrTimerCallback m_fncb;
	void *m_pTimerParam;
	GSMutex m_mutex;
	StruGSTimerRunThread *m_pRunThread;
	GSTimeMeter m_csMeter;
	
public :
	//初始化定时器模块, nThreadNums 线程数， 如果为 1 所有timer只使用一个线程回调， 否则多个timer 使用的是多个线程并发回调
	static bool ModuleInit( int nThreadNums = 0);
	static void ModuleUninit(void);


	//nInterval 间隔， 单位毫秒
	GSTimer(long timerID);
	~GSTimer(void);

	//初始化， 只能初始化一次
	bool Init(int nInterval, GSTimer::FunPtrTimerCallback fnCb, void *pTimerParam);

	long GetTimerID(void) const;


	//开始回调 ， isRunImmediately 是否立刻执行一次
	bool Start( bool isRunImmediately );
	//bWait 如果正在调度， 是否等待当前任务结束
	void Stop(bool bWait = true);

private :
	void RunCallback(void);
};



#endif //end _GS_H_GSTIME_H_
