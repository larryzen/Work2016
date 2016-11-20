// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//���ͷ�ļ�
#include "..\..\��Ϣ����\CMD_Fish.h"
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

	//����
	void Lock() {
		::EnterCriticalSection(&cs);
	}

	//����
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

#define Random(x) (rand() % x) //ͨ��ȡ��ȡ��ָ����Χ�������