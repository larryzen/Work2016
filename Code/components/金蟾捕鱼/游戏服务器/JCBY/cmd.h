#ifndef CMD_HEAD_FILE
#define CMD_HEAD_FILE
#include "stdafx.h"
#define WM_DWRSERVET_MESSAGE (WM_USER+100) 

#define ROOMTABLE 1
#define USERINFOTABLE 0

//struct SPlayer
//{
//	int playerID;//玩家ID
//	int playerNowScore;//玩家目前分数
//	int playerNowPutScore;//玩家当次上分数
//	int playerPutScore;//玩家总上分值
//	int playerGetScore;//玩家总下分值
//	int playerShootingTime;//总射击次数
//	int playerMaxScore;//分数最大值
//	int playerDeadTime;//分数归零次数
//	int playerGunScore;//玩家每炮分数
//	int playerRegulation;//手动操控影响值
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