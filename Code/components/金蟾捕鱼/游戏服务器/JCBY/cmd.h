#ifndef CMD_HEAD_FILE
#define CMD_HEAD_FILE
#include "stdafx.h"
#define WM_DWRSERVET_MESSAGE (WM_USER+100) 

#define ROOMTABLE 1
#define USERINFOTABLE 0

//struct SPlayer
//{
//	int playerID;//���ID
//	int playerNowScore;//���Ŀǰ����
//	int playerNowPutScore;//��ҵ����Ϸ���
//	int playerPutScore;//������Ϸ�ֵ
//	int playerGetScore;//������·�ֵ
//	int playerShootingTime;//���������
//	int playerMaxScore;//�������ֵ
//	int playerDeadTime;//�����������
//	int playerGunScore;//���ÿ�ڷ���
//	int playerRegulation;//�ֶ��ٿ�Ӱ��ֵ
//
//};

typedef struct _info
{
	CListCtrl * m_list;
	int col;
	bool asc;
}INFO;



//#pragma pack()

#endif