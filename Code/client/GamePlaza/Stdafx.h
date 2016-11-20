#pragma once

//////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h> 
#include <afxext.h> 
#include <afxdisp.h>

#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

//////////////////////////////////////////////////////////////////////////

//ϵͳͷ�ļ�
#include "Nb30.h"
#include "Afxmt.h"
#include "AfxHtml.h"
#include "AfxInet.h"

//ȫ��ͷ�ļ�
#include "..\..\share\Constant.h"
#include "..\..\share\GlobalDef.h"
#include "..\..\share\GlobalField.h"
#include "..\..\share\GlobalFrame.h"
#include "..\..\share\GlobalRight.h"

//����ͷ�ļ�
#include "..\..\msg\CMD_Game.h"
#include "..\..\msg\CMD_Plaza.h"
#include "..\..\msg\CMD_Video.h"

//ģ���
#include "..\..\template\Template.h"

//���ͷ�ļ�
#include "..\UserFace\UserFace.h"
#include "..\DownLoad\DownLoad.h"
#include "..\GameRank\GameRank.h"
#include "..\Companion\Companion.h"
#include "..\ChannelModule\ChannelModule.h"
#include "..\ClientShare\ClientShare.h"
#include "..\PropertyModule\PropertyModule.h"
#include "..\SkinControls\SkinControls.h"
#include "..\SkinResource\SkinResourceModule.h"
#include "..\..\common\NetworkService\NetworkServiceHead.h"
#include "..\..\common\ComService\ComService.h"

//////////////////////////////////////////////////////////////////////////
