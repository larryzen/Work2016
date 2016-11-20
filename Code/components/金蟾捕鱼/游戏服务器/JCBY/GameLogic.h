#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "mtdll.h"
#include "YJAlgorithm.h"
//#include "ISQliteOperation.h"
#include "SQLiteOperate.h"

#define MAX_ERROR			5000//发送MAX_ERROR次错误则踢玩家下线

#define	MAX_ENERGY			5
#define ENERGY_TIME			20

#define DEFFALUT_ROOM_MAX_COST	1000
#define DEFFALUT_UP_SCORE	10000//一次上分数值

#define DEFFALUT_FACTOR		1.0f
#define DEFFALUT_LEVEL		3	

#ifdef _DEBUG
#define ONE_TABLE_DELAY		0.1f
#else
#define ONE_TABLE_DELAY		3.0f	//单个桌子更新时间 = MaxTable*ONE_TABLE_DELAY
#endif // _DEBUG


#define NAME_LEN	32

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CTableFrameSink;

struct SGamePlayer
{
	SPlayer		alData;
	DWORD		uid;
	BYTE		cannonType;
	bool		serverAcceptEnergy;
	int			errorCount;
	float		energyTimer;
	bool		bActive;
	bool		isAI;
	int			lostScore;
	DWORD		IP;
	char		szName[NAME_LEN];
	LONGLONG	lscore;//总积分
	LONGLONG	lscoreLogin;//登陆时的总积分
};



//游戏逻辑
class CGameLogic : public IErrorCallBack
{
public:
	//static		char		dataBasePath[MAX_PATH];

private:
	static		SRoomData	roomData;
	static		int			roomPort;
	static		char		roomName[256];
	//变量定义
protected:
	SGamePlayer	player[GAME_PLAYER];
	float		saveDataTimer;
	float		saveTimeDelay;
	//SMonster	monsterteam[MAX_FISH_TYPE];//鱼类表

	
	int			realUserCount;
	int			totalUserCount;

	CTableFrameSink* pTable;

	//CsLock		csSaveLock;
	//CsLock		csReadLock;
	//CsLock		csTickLock;

	SQLiteOperate sql_;

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	void SetTable(CTableFrameSink* table);
public:
	void ReadRoomData(const char* roomName,int roomPort_);

	bool FishGoldByStyle(int chairID, int wCost, int nFishStyleOrRatio, bool autoRatio);

	int RandomTeamNumber()const;

	void Tick();

	int GetRatioByTpye(int type);

private:
	void _ReadRoomData(const char* roomName, SQLiteOperate& sql,int roomPort_);

	bool _WriteAllDataToDataBase();
	bool _ReadPlayerDataFromDatabase(DWORD uid, int chairID);
	bool _SavePlayerDataToDatabase(DWORD uid, int chairID);
	bool _ReadPlayerDataFromDatabase(DWORD uid, int chairID, SQLiteOperate& sql, bool* bAdd = NULL);
	bool _SavePlayerDataToDatabase(DWORD uid, int chairID, SQLiteOperate& sql);
	void _AcountScore(int chairID);//结算分数
	//bool _FishGoldByRmSystem(int chairID, int nFishStyle, int bulletCost);
	bool _FishGoldByRmSystemEx(int chairID, int ratio, int bulletCost);
	void _AddScoreByAlgorithm(int chairID, int score);
	int _GetMinGunScore();
	void _UpdateRoomData();

	void _dberror(const char* msg);
public:
	SGamePlayer* GetData(int chairID){return &player[chairID];}
	SPlayer* GetAlData(int chairID){return &player[chairID].alData;}

	void InitChair(DWORD uid, DWORD IP, const char* name, int chairID,
		bool bAI, LONGLONG lscore,
		SQLiteOperate& sql);
	bool AccountChair(DWORD uid, int chairID, SQLiteOperate& sql);
	bool ClearChair(DWORD uid, int chairID, SQLiteOperate& sql);

	int GetBulletCost(int chairID)const {return player[chairID].alData.playerGunScore;}
	int GetNowScore(int chairID)const {return player[chairID].alData.playerNowScore;}
	int GetFireCount(int chairID)const{return player[chairID].alData.playerShootingTime;}
	BYTE GetCannonType(int chairID)const{return player[chairID].cannonType;}
	int GetRoomMaxCost()const{return roomData.roomMaxCost;};
	DWORD GetUID(int chairID)const{return player[chairID].uid;}
	int GetRoomOnceUpScore()const{return roomData.onceUpScore;};
	int GetRealUserCount()const{return realUserCount;}
	int GetTotalUserCount()const{return totalUserCount;}
	int GetRegulation(int chairID)const{return player[chairID].alData.playerRegulation;}
	int GetLostScore(int chairID)const{return player[chairID].lostScore;}
	LONGLONG GetlScore(int chairID)const{return player[chairID].lscore;}
	LONGLONG GetAccountScore(int chairID){return player[chairID].lscore-player[chairID].lscoreLogin;}

	bool MaxError(int chairID)const{return player[chairID].errorCount >= MAX_ERROR;}
	void ClearError(int chairID){player[chairID].errorCount = 0;}

	void AddScore(int chairID, int score, bool upscore = false);//用于上下分以及子弹发射
	void AddLostScore(int chairID, int score){player[chairID].lostScore += score;}
	void AddPlayerShoot(int chairID){player[chairID].alData.playerShootingTime++;}
	void AddError(int chairID){player[chairID].errorCount++;}

	void AddlScore(int chairID, LONGLONG value){player[chairID].lscore += value;}

	void SetlScore(int chairID, LONGLONG lscore){player[chairID].lscore = player[chairID].lscoreLogin = lscore;}
	bool SetBulletCost(int chairID, int cost){
		if(cost > roomData.roomMaxCost)return false;
		player[chairID].alData.playerGunScore = cost;
		return true;
	}
	bool SetCannonTpye(int chairID, BYTE type);
	void SetCannonTypeByServer(int chairID, BYTE type, bool accept){player[chairID].cannonType = type;player[chairID].serverAcceptEnergy = accept;}

	bool IsChairHasUser(int chairID)const{return player[chairID].bActive;}
	bool IsChairAI(int chairID)const{return player[chairID].isAI;}
	bool IsEnergyGun(int chairID)const{return player[chairID].cannonType == 1;}
	bool HasNoneAIUser()const{return realUserCount!=0;}//是否存在非机器人用户

	virtual void LogError( const char* msg ){_dberror(msg);}

};

inline int CGameLogic::_GetMinGunScore()
{
	if (roomData.roomMaxCost <= 99)
	{
		return 1;
	}
	if (roomData.roomMaxCost <= 1000)
	{
		return 10;
	}
	if (roomData.roomMaxCost <= 9900)
	{
		return 100;
	}
	if (roomData.roomMaxCost <= 99000)
	{
		return 100;
	}
	return 10;
}

inline int CGameLogic::RandomTeamNumber() const
{
	int n = rand()%10;
	switch(n)
	{
	case 1:
	case 2:
		return 7;
	case 3:
	case 4:
	case 5:
	case 6:
		return 4;
	default:
		return 3;
	}
}

//////////////////////////////////////////////////////////////////////////
inline void DBPD_To_SP(const SDataBasePlayerData* in, SPlayer* out)
{
	ASSERT(in);
	ASSERT(out);
	out->playerNowScore = in->nowScore;
	out->playerNowPutScore = in->nowPutScore;
	out->playerPutScore = in->putScore;
	out->playerGetScore = in->getScore;
	out->playerShootingTime = in->shootingTime;
	out->playerDeadTime = in->deadTime;
	out->playerGunScore = in->gunScore;
	out->playerRegulation = in->playerRegulation;
	out->playerMaxScore = in->maxScore;
}

inline void SP_To_DBPD(const SPlayer* in,  SDataBasePlayerData* out)
{
	ASSERT(in);
	ASSERT(out);
	out->nowScore = in->playerNowScore;
	out->nowPutScore = in->playerNowPutScore;
	out->putScore = in->playerPutScore;
	out->getScore = in->playerGetScore;
	out->shootingTime = in->playerShootingTime;
	out->deadTime = in->playerDeadTime;
	out->gunScore = in->playerGunScore;
	out->playerRegulation = in->playerRegulation;
	out->maxScore = in->playerMaxScore;
}

inline void CheckPlayerData(SPlayer* in_out)
{
	int* tmp = (int*)in_out;
	int size = sizeof(SPlayer)/sizeof(int);
	size--;
	if(size<=0)return;
	for (int i=0;i<size;i++)
	{
		if (tmp[i] < 0)
		{
			tmp[i] = 0;
		}
	}
}

inline void CheckAndModifyPlayerData(SPlayer* out)
{
	int maxValue = max(out->playerPutScore, out->playerGetScore);
	if (maxValue > 1000000000)
	{
		int minValue = min(out->playerPutScore, out->playerGetScore);
		out->playerPutScore -= minValue;
		out->playerGetScore -= minValue;
	}
}

#endif