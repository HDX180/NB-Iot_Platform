/*
******************************************
Copyright (C): XC
Filename : XCSTRUTIL.HPP
Author :  hudaixin
Description: 字符串操作函数
********************************************
*/

#ifndef _XC_H_XCSTRUTIL_H_
#define _XC_H_XCSTRUTIL_H_


#include "XCTypes.h"
#include "XCDefs.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

#include <iomanip>

#include "XCDefs.h"

#ifndef _MSWINDOWS_
#include <stdarg.h>
#include <string.h>
#endif


#define XCString std::string

#define XCStringStream std::stringstream

/*
*********************************************************************
*
*@brief : 字符串操作功能
*
*********************************************************************
*/



namespace  XCStrUtil
{


	// 删除字符串左边空白
	INLINE static XCString  TrimLeft(const XCString& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		return t;
	}

	// 删除字符串右边空白
	INLINE static XCString TrimRight(const XCString& str)
	{
		std::string t = str;
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	}

	// 删除字符串左右两边空白
	INLINE static XCString Trim(const XCString& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	}

	// 转换为小写字符串
	INLINE static XCString ToLower(const XCString& str) 
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), ::tolower);
		return t;
	}

	// 转换为大写字符串
	INLINE static XCString ToUpper(const XCString& str) 
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), ::toupper);
		return t;
	}

	// 分解字符串
	INLINE static  void Split(std::vector<XCString> &csResult, const XCString& str, const XCString& delimiters)
	{
		std::string::size_type iOffset = 0;
		XCString strToken;
		for(;;)
		{

			std::string::size_type i = str.find_first_not_of(delimiters, iOffset);
			if (i == std::string::npos) {
				iOffset = str.length();
				return;
			}

			// 查找标识结束位置
			std::string::size_type j = str.find_first_of(delimiters, i);
			if (j == std::string::npos) {
				strToken = str.substr(i);
				iOffset = str.length();
				csResult.push_back(strToken);
			}
			else
			{
				strToken = str.substr(i, j - i);
				iOffset = j;
				csResult.push_back(strToken);
			}
		}
	}

	//不分大小写的比较
	INLINE static bool EqualsIgnoreCase( const XCString& strSrc, const XCString& strDest)
	{
		return ToLower(strSrc) == ToLower(strDest);
	}


	// 字符串类型转换模板函数
	// 字符串类型转换模板函数
	template<class T> T ToNumber(const XCString& str);
	template<class T> T ToHexNumber(const XCString& str);
	template<class T> XCString ToString(const T value);
	template<class T> XCString ToString(const T value, int fixedPrecision);
	template<class T> XCString ToHexString(const T value);



	/*
	*********************************************
	Function : ToNumber
	DateTime : 2011/1/20 15:23
	Description : 将十进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T> 
	T ToNumber(const XCString& str)
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> value;
		return value;
	}

	/*
	*********************************************
	Function :
	DateTime : 2011/1/20 15:23
	Description : 将十六进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T> 
	T ToHexNumber(const XCString& str) 
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> std::hex >> value;
		return value;
	}


	template<class T> 
	XCString ToString(const T value) 
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	template<class T> 
	XCString ToString(const T value, int fixedPrecision) 
	{
		std::ostringstream oss;
		oss <<std::fixed<<std::setprecision(fixedPrecision)<< value;
		return oss.str();
	}


	template<class T> 
	XCString ToHexString(const T value) 
	{
		std::ostringstream oss;
		oss << "0x" << std::hex << value;
		return oss.str();
	}



	template<class T> 
	int CheckValueRange(T &vValue, const T vMin, const T vMax)
	{
		if( vValue<vMin )
		{
			vValue = vMin;
			return -1;
		}
		else if( vValue > vMax )
		{
			vValue = vMax;
			return 1;
		}
		return 0;
	}

#define MAX_STRING_LENGTH  (100<<20)

	INLINE static  bool VFormat(XCString &strOString,const char *czFormat, va_list ap )
	{
		char sBuffer[256];
		va_list apStart;
		char *pBuffer;
		int n, size = 256; 


		strOString.clear();
#ifdef _MSWINDOWS_
		apStart = ap;       
#else
		va_copy(apStart,ap);
#endif
		pBuffer = sBuffer;
		while (pBuffer) {           
#ifdef _MSWINDOWS_
			ap = apStart;
#else
			va_copy(ap,apStart);
#endif
			n = xc_vsnprintf(pBuffer, size, czFormat, ap);          
			if (n > -1 && n < size )
			{
				//成功格式化
				//pBuffer[n] = '\0';
				strOString =  pBuffer;
				if( pBuffer!=sBuffer )
				{      
					::free(pBuffer);
				}
				return true;
			}
			if( pBuffer!=sBuffer )
			{      
				::free(pBuffer);
			}
			pBuffer = NULL;
			size *= 2;
			if( size>MAX_STRING_LENGTH )
			{
				XCAssert(0);
				return false;
			}

			pBuffer = (char*) ::malloc(size);
			XCAbort(pBuffer!=NULL);
		}
		if( pBuffer && pBuffer!=sBuffer  )
		{
			::free(pBuffer);
		}
		return false;
	}



	INLINE static  bool Format(XCString &strOString, const char* czFormat, ...)
	{
		bool bRet;
		va_list ap;
		va_start(ap, czFormat);
		bRet = XCStrUtil::VFormat(strOString, czFormat, ap);
		va_end(ap);
		return  bRet;

	}

	INLINE static  bool AppendWithFormat(XCString &strIOString, const char* czFormat, ...)
	{
		bool bRet; 
		XCString strTemp;
		va_list ap;
		va_start(ap, czFormat);
		bRet = XCStrUtil::VFormat(strTemp, czFormat, ap);
		va_end(ap);

		if( bRet )
		{
			strIOString += strTemp;
		}
		return bRet;
	}



	INLINE static void FilenameFormat( XCString &strFilename )
	{
		//格式化文件名
#ifdef _MSWINDOWS_
		const char cDelimiters =  '\\';
#else
		const char cDelimiters =  '/';
#endif
		XCString strTemp = strFilename;
		char *pStr = (char*)strTemp.c_str();
		char *p = pStr;;
		while( *p!='\0' )
		{
			if( *p == '\\' || *p == '/' )
			{
				*p = cDelimiters;
			}
			p++;
		}
		p = pStr;
		while(p[0]!='\0' && p[1] !='\0' )
		{
			if( p[0]==cDelimiters && p[1]==cDelimiters )
			{
				for( int i = 1; p[i] !='\0' ; i++ )
				{
					p[i] = p[i+1];
				}
			}
			else
			{
				p++;
			}
		}
		strFilename = pStr;
	}

	INLINE static void FilenameFormatLinux( XCString &strFilename )
	{
		//格式化文件名
		const char cDelimiters =  '/';
		XCString strTemp = strFilename;
		char *pStr = (char*)strTemp.c_str();
		char *p = pStr;;
		while( *p!='\0' )
		{
			if( *p == '\\' || *p == '/' )
			{
				*p = cDelimiters;
			}
			p++;
		}
		p = pStr;
		while(p[0]!='\0' && p[1] !='\0' )
		{
			if( p[0]==cDelimiters && p[1]==cDelimiters )
			{
				for( int i = 1; p[i] !='\0' ; i++ )
				{
					p[i] = p[i+1];
				}
			}
			else
			{
				p++;
			}
		}
		strFilename = pStr;
	}

	// 字符串替换
	INLINE static bool StringReplace( XCString& strMain, const XCString& strFrom, const XCString& strTo )
	{
		if ( strFrom.empty() )
			return false;

		XCString::size_type pos = 0;
		XCString::size_type fromLen = strFrom.size();
		XCString::size_type toLen = strTo.size();

		pos = strMain.find(strFrom, pos); 

		while ((pos != XCString::npos))
		{
			strMain.replace(pos, fromLen, strTo);
			pos=strMain.find(strFrom, (pos+toLen));
		}

		return true;
	}
};


#endif //end _XC_H_XCSTRUTIL_H_
