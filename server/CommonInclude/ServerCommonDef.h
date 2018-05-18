#ifndef SERVICECOMMON_DEF_H
#define SERVICECOMMON_DEF_H


namespace XC
{
	typedef enum
	{
		ERR_SUCCESS = 0,                   // �ɹ�
		ERR_INIT_FAIL,                     // ��ʼ��ʧ��
		ERR_MEMORY_EXCEPTION,              // �ڴ��쳣
		ERR_CALL_INTERFACE_FAIL,           // ���ýӿ�ʧ��

		ERR_USERNAME_PASSWORD_ERROR = 60,       // �û����������
		ERR_OPERNOPERMIT,                  // ��Ȩ��
		ERR_OVER_MAXLINK,                  // �������������
		ERR_VERSIONNOMATCH,                // �汾��ƥ��
		ERR_PARAMETER_ERROR,               // ��������	

		ERR_NOSUPPORT,                     // ��������֧��
		ERR_SERVER_BUSY,                   // ������æ
		ERR_DEVICE_OFFLINE,                // �豸������
		ERR_SYSTEM_SHUTDOWN,               // ����ر�
		ERR_NOT_LOGIN,                     // δע��
		ERR_NOT_DEVICE,					   //�豸������
		ERR_NORESOURCE,					   //û����Դ
		ERR_NOT_FUNC,					   //û�иù��� 
		ERR_OPER_PLUGIN,				   //�������ʧ��
		ERR_PROTOCOL_INVALID,			   //Э�����
		ERR_OPEN_FILE_FAIL,				   //��¼���ļ�ʧ��
		ERR_PARSER_FAIL,				   //��������Э��ʧ��
		ERR_HAS_EXIST,					   // �Ѵ���
		ERR_CONNECT_FAIL,				   // ����ʧ��
		ERR_REQUEST_TIMEOUT,			   // ����ʱ
		ERR_NETWORK_ANOMALY,			   // �����쳣
		ERR_FILE_NOT_FOUND,              // û���ҵ���Ӧ���ļ�

		ERR_CLIENT_REJECT,					// �ͻ��˾ܾ��Խ�
		ERR_CLIENT_IS_TALKING,              // �ͻ������ڶԽ�

		ERR_CHN_NOT_STORAGE_POLICY,				//ͨ��û�����ô洢����


	} EnumErrorCode;
}



#endif