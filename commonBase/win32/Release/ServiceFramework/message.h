#ifndef _GS_MESSAGE_H_
#define _GS_MESSAGE_H_

#include<string.h>
#include"GSDefs.h"
#include "GSStrUtil.hpp"

typedef struct _MessageInfo
{
	_MessageInfo();
	_MessageInfo(const struct _MessageInfo &msg);
	_MessageInfo& operator=(const _MessageInfo&);
	~_MessageInfo();

	bool IsValid();

	//同SetSubject()，两者用其一即可
	void SetAddress(const char* szAddress);
	const char* GetAddress() const;


	//源主题(即回复地址)
	void SetReplyTo(const char* szReplyTo);
	const char* GetReplyTo() const;


	//消息主题(即目标地址)，可表示单个或多主题。多个主题可通过“.”隔开，例如，“主题1.主题2.主题3”。点对点或订阅/发布都适用
	void SetSubject(const char* szSubject);
	const char* GetSubject() const;

	//消息序列号sn
	void SetSeq(const char* szSeq);
	const char* GetSeq() const;

	//消息生存期，如果消息缓存在broker超过一定时间没有读取，会自动丢弃，"0"为一直缓存直到被取走或订阅者下线
	void SetTTL(UInt64 milliseconds);

	//用户自定义数据，key-value形式
	void SetUserData(const char* szKey, const char* szValue);
	const char* GetUserData(const char* szKey) const;

	//获取所有自定义数据，返回的格式为："key1=value1;key2=value2;key3=value3...keyN=valueN"
	const char* GetAllUserData() const;


	//消息体
	void SetContent(const char* content, size_t length);
	const char* GetContent() const;
	size_t GetContentSize() const;

private:

	friend class CMessageBus;
	void * m_Impl;

}MessageInfo;


typedef struct _HeartBeatInfo
{
	char						info[256];//允许携带的心跳信息
	int                         length;


	_HeartBeatInfo():length(0)
	{
		memset(info,0,sizeof(info));

	}


}HeartBeatInfo;

typedef void  (*pHeartBeatCallback)(const char* serverName, HeartBeatInfo* pHeartBeatInfo);

typedef enum 
{
	EVENT_CONNECTED,//连接成功
	EVENT_DISCONNECTED,//断开连接
	EVENT_PUBLISH_FAIL,//发送失败
	EVENT_RECEIVE_FAIL, //接收失败
	EVENT_AUTH_FAIL, //认证失败
	EVENT_ADRESS_INVALID, //地址解释错误
	EVENT_INPUT_CMD	//控制台命令输入

}ENUM_EVENT_TYPE;

#endif