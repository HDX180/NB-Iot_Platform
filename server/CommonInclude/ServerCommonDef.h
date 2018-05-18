#ifndef SERVICECOMMON_DEF_H
#define SERVICECOMMON_DEF_H


namespace XC
{
	typedef enum
	{
		ERR_SUCCESS = 0,                   // 成功
		ERR_INIT_FAIL,                     // 初始化失败
		ERR_MEMORY_EXCEPTION,              // 内存异常
		ERR_CALL_INTERFACE_FAIL,           // 调用接口失败

		ERR_USERNAME_PASSWORD_ERROR = 60,       // 用户名密码错误
		ERR_OPERNOPERMIT,                  // 无权限
		ERR_OVER_MAXLINK,                  // 超过最大连接数
		ERR_VERSIONNOMATCH,                // 版本不匹配
		ERR_PARAMETER_ERROR,               // 参数错误	

		ERR_NOSUPPORT,                     // 服务器不支持
		ERR_SERVER_BUSY,                   // 服务器忙
		ERR_DEVICE_OFFLINE,                // 设备不在线
		ERR_SYSTEM_SHUTDOWN,               // 服务关闭
		ERR_NOT_LOGIN,                     // 未注册
		ERR_NOT_DEVICE,					   //设备不存在
		ERR_NORESOURCE,					   //没有资源
		ERR_NOT_FUNC,					   //没有该功能 
		ERR_OPER_PLUGIN,				   //插件调用失败
		ERR_PROTOCOL_INVALID,			   //协议错误
		ERR_OPEN_FILE_FAIL,				   //打开录像文件失败
		ERR_PARSER_FAIL,				   //解析服务协议失败
		ERR_HAS_EXIST,					   // 已存在
		ERR_CONNECT_FAIL,				   // 连接失败
		ERR_REQUEST_TIMEOUT,			   // 请求超时
		ERR_NETWORK_ANOMALY,			   // 网络异常
		ERR_FILE_NOT_FOUND,              // 没有找到相应的文件

		ERR_CLIENT_REJECT,					// 客户端拒绝对讲
		ERR_CLIENT_IS_TALKING,              // 客户端正在对讲

		ERR_CHN_NOT_STORAGE_POLICY,				//通道没有配置存储策略


	} EnumErrorCode;
}



#endif