#ifndef VIDEO_SERVICE_HEAD_FILE
#define VIDEO_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\template\Template.h"
#include "..\..\share\GlobalDef.h"

//��Ϣ����
#include "..\..\msg\CMD_Video.h"

//���ͷ�ļ�
#include "..\..\template\Module.h"
#include "..\SkinControls\SkinControls.h"
#include "..\GameFrame\GameFrame.h"
#include "..\PropertyModule\FlowerEffectcontrol.h"
//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef VIDEO_SERVICE_CLASS
	#ifdef  VIDEO_SERVICE_DLL
		#define VIDEO_SERVICE_CLASS _declspec(dllexport)
	#else
		#define VIDEO_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoService.dll")			//��� DLL ����
#else
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoService.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagVideoNetwordInfo
{
	WORD							wNatPort;							//����˿�
	WORD							wLocalPort;							//����˿�
	DWORD							dwNatAddr;							//�����ַ
	DWORD							dwLocalAddr;						//���ص�ַ
};

//////////////////////////////////////////////////////////////////////////

//������ͷ�ļ�

#include "VideoServiceControl.h"

//////////////////////////////////////////////////////////////////////////

#endif
