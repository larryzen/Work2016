// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//组件头文件
#include "..\..\消息定义\CMD_Fish.h"
#include "..\..\..\..\server\GameService\GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////
//#pragma comment(lib, "MTDLL.lib")

#define LOG_FILE_NAME		TEXT("Logs/QPJCBY.db")

struct ILock
{
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

class CsLock : public ILock
{
public:
	CsLock()	{
		::InitializeCriticalSection(&cs);
	}

	~CsLock() {
		::DeleteCriticalSection(&cs);
	}

	//锁定
	void Lock() {
		::EnterCriticalSection(&cs);
	}

	//解锁
	void UnLock() {
		::LeaveCriticalSection(&cs);
	}

private:
	CRITICAL_SECTION cs;
};



class CThreadEvent : public ILock
{
public:
	CThreadEvent(){hEvent = CreateEvent(0,TRUE,TRUE,NULL);}
	~CThreadEvent(){CloseHandle(hEvent);}

	void Lock(){ResetEvent(hEvent);}
	void UnLock(){SetEvent(hEvent);}

	operator HANDLE(){return hEvent;}

private:
	HANDLE hEvent;
};

class CAutoLock
{
public:
	CAutoLock(ILock* lock):lock_(lock){lock_->Lock();}
	~CAutoLock(){lock_->UnLock();}
private:
	ILock* lock_; 
};

#ifdef _DEBUG
#define DEBUG_PRINT(format,...) printf(format##"\n",__VA_ARGS__)
#else
#define DEBUG_PRINT(format,...)
#endif

#define SMT_CH_GAME_TO_ROOM 1
#define SMT_CH_GAME_TO_KIND 2
#define SMT_CH_GAME_TO_ALL 3

#define Random(x) (rand() % x) //通过取余取得指定范围的随机数