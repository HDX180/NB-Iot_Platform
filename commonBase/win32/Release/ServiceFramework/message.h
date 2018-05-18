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

	//ͬSetSubject()����������һ����
	void SetAddress(const char* szAddress);
	const char* GetAddress() const;


	//Դ����(���ظ���ַ)
	void SetReplyTo(const char* szReplyTo);
	const char* GetReplyTo() const;


	//��Ϣ����(��Ŀ���ַ)���ɱ�ʾ����������⡣��������ͨ����.�����������磬������1.����2.����3������Ե����/����������
	void SetSubject(const char* szSubject);
	const char* GetSubject() const;

	//��Ϣ���к�sn
	void SetSeq(const char* szSeq);
	const char* GetSeq() const;

	//��Ϣ�����ڣ������Ϣ������broker����һ��ʱ��û�ж�ȡ�����Զ�������"0"Ϊһֱ����ֱ����ȡ�߻���������
	void SetTTL(UInt64 milliseconds);

	//�û��Զ������ݣ�key-value��ʽ
	void SetUserData(const char* szKey, const char* szValue);
	const char* GetUserData(const char* szKey) const;

	//��ȡ�����Զ������ݣ����صĸ�ʽΪ��"key1=value1;key2=value2;key3=value3...keyN=valueN"
	const char* GetAllUserData() const;


	//��Ϣ��
	void SetContent(const char* content, size_t length);
	const char* GetContent() const;
	size_t GetContentSize() const;

private:

	friend class CMessageBus;
	void * m_Impl;

}MessageInfo;


typedef struct _HeartBeatInfo
{
	char						info[256];//����Я����������Ϣ
	int                         length;


	_HeartBeatInfo():length(0)
	{
		memset(info,0,sizeof(info));

	}


}HeartBeatInfo;

typedef void  (*pHeartBeatCallback)(const char* serverName, HeartBeatInfo* pHeartBeatInfo);

typedef enum 
{
	EVENT_CONNECTED,//���ӳɹ�
	EVENT_DISCONNECTED,//�Ͽ�����
	EVENT_PUBLISH_FAIL,//����ʧ��
	EVENT_RECEIVE_FAIL, //����ʧ��
	EVENT_AUTH_FAIL, //��֤ʧ��
	EVENT_ADRESS_INVALID, //��ַ���ʹ���
	EVENT_INPUT_CMD	//����̨��������

}ENUM_EVENT_TYPE;

#endif