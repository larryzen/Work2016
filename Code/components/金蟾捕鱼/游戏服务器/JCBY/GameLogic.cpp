#include "StdAfx.h"
#include "GameLogic.h"

#include <assert.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include "TableFrameSink.h"
#include <stdio.h>



#define Print(str)
#define Warnning(str) 
#define LogError(str) 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
SRoomData CGameLogic::roomData;
char CGameLogic::roomName[256] = {0};
int CGameLogic::roomPort = 0;
//char CGameLogic::dataBasePath[MAX_PATH] = {0};

//���캯��
CGameLogic::CGameLogic():
pTable(NULL)
{
	memset(player,0,sizeof(player));
	
	realUserCount = 0;
	totalUserCount = 0;

	//sql_.SetErrorCallBack(this);
}

//��������
CGameLogic::~CGameLogic()
{
	
}

bool CGameLogic::FishGoldByStyle(int chairID, int wCost, int nFishStyleOrRatio, bool autoRatio)
{ 
	int ratio = nFishStyleOrRatio;
	if (IsEnergyGun(chairID))
	{
		ratio *= 2;
	}
	return _FishGoldByRmSystemEx(chairID, ratio, wCost);
}

bool CGameLogic::_FishGoldByRmSystemEx( int chairID, int ratio, int bulletCost )
{
	SMonster monster;
	ratio = max(ratio, 2);
	ratio = min(ratio, 2000);
	SPMonster(&monster, ratio, &roomData);

	int ret = RMSystem(bulletCost, roomData.roomMaxCost, &monster, &player[chairID].alData, PlayerLevel);
	assert(ret>=0);
	_AddScoreByAlgorithm(chairID, ret);
	return ret>0?true:false;
}

void CGameLogic::Tick()
{
	//csTickLock.Lock();
	float time = pTable->getTime();
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (IsChairHasUser(i))
		{
			if (GetCannonType(i)==1)
			{
				if (time - player[i].energyTimer > ENERGY_TIME)
				{
					SetCannonTypeByServer(i, 0, false);
					pTable->SendChangeCannon(i, GetBulletCost(i), GetCannonType(i));
				}
			}
			else
			{
				player[i].energyTimer = time;
			}
		}
	}
	//csTickLock.UnLock();

	if (time - saveDataTimer > saveTimeDelay)
	{
		_WriteAllDataToDataBase();
		_UpdateRoomData();
		saveDataTimer = time;
	}
}

bool CGameLogic::_WriteAllDataToDataBase()
{
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (IsChairHasUser(i))
		{
			_SavePlayerDataToDatabase(GetUID(i), i);
		}
	}
	return true;
}

bool CGameLogic::_ReadPlayerDataFromDatabase( DWORD uid, int chairID )
{
	return _ReadPlayerDataFromDatabase(uid, chairID, this->sql_);
}

bool CGameLogic::_ReadPlayerDataFromDatabase( DWORD uid, int chairID, SQLiteOperate& sql, bool* bAdd )
{
	bool b = false;

	b = sql.SearchPlayerUID(uid);
	if (b)
	{
		SDataBasePlayerData dbpd;
		memset(&dbpd,0,sizeof(dbpd));
		b=sql.ReadDataBasePlayerData(uid, &dbpd);
		
		if (!b)
		{
			LogError("��ȡ�������ʧ�� uid:"<<uid);
		}
		else
		{
			DBPD_To_SP(&dbpd, &player[chairID].alData);
			player[chairID].lostScore = dbpd.lostScore;
		}
	}
	else
	{
		SDataBasePlayerData data;
		memset(&data,0,sizeof(data));
		data.IP = player[chairID].IP;
		strcpy_s(data.name,32,player[chairID].szName);
		b=sql.AddDataBasePlayerData(uid, &data);
		if (!b)
		{
			LogError("����������ʧ��");
		}
		else
		{
			if (bAdd)
			{
				*bAdd = true;
			}
		}
	}

	if (player[chairID].alData.playerRegulation > 2)
	{
		LogError("���ݿ���ֵ�쳣 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -3;
	}
	if (player[chairID].alData.playerRegulation < -4)
	{
		LogError("���ݿ���ֵ�쳣 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -4;
	}

	if (!b)
	{
		LogError("����д��ʧ�ܣ�uid:"<<uid);
		return false;
	}	

	CheckPlayerData(&player[chairID].alData);

	return b;
}

bool CGameLogic::_SavePlayerDataToDatabase( DWORD uid, int chairID )
{
	return _SavePlayerDataToDatabase(uid, chairID, this->sql_);
}

bool CGameLogic::_SavePlayerDataToDatabase( DWORD uid, int chairID, SQLiteOperate& sql )
{
	if (IsChairAI(chairID))return true;

	if (player[chairID].alData.playerRegulation > 2)
	{
		LogError("���ݿ���ֵ�쳣 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -3;
	}
	if (player[chairID].alData.playerRegulation < -4)
	{
		LogError("���ݿ���ֵ�쳣 playerRegulation:"<<player[chairID].alData.playerRegulation<<" uid:"<<uid);
		player[chairID].alData.playerRegulation = -4;
	}

	bool b = false;

	SDataBasePlayerData dbpd;
	memset(&dbpd,0,sizeof(dbpd));
	b=sql.ReadDataBasePlayerData(uid, &dbpd);
	if(b)player[chairID].alData.playerRegulation = dbpd.playerRegulation;
	if (!b)
	{
		Warnning("��ȡ�����ֵʧ�ܣ�������Ӹ���� uid:"<<uid);
		memset(&dbpd,0,sizeof(dbpd));
		dbpd.IP = player[chairID].IP;
		strcpy_s(dbpd.name,32,player[chairID].szName);
		sql.AddDataBasePlayerData(uid, &dbpd);

	}

	memset(&dbpd,0,sizeof(dbpd));

	SP_To_DBPD(&player[chairID].alData, &dbpd);
	dbpd.lostScore = player[chairID].lostScore;
	dbpd.IP = player[chairID].IP;

	memcpy(dbpd.name,player[chairID].szName,32);

	b = sql.UpdateDataBasePlayerData(uid, &dbpd);

	if (!b)
	{
		dbpd.IP = player[chairID].IP;
		strcpy_s(dbpd.name,32,player[chairID].szName);
		b = sql.AddDataBasePlayerData(uid, &dbpd);

		if (!b)
		{
			LogError("����д��ʧ�ܣ�uid:"<<uid);
		}
		
		return false;
	}	
	return b;
}

bool CGameLogic::SetCannonTpye( int chairID, BYTE type )
{
	if(type>=1){
		if (!player[chairID].serverAcceptEnergy)return false;
	}	
	player[chairID].cannonType = type;
	return true;
}

void CGameLogic::InitChair( DWORD uid, DWORD IP, const char* name,
						   int chairID, bool bAI,  LONGLONG lscore,
						   SQLiteOperate& sql)
{
	totalUserCount++;
	bool bAdd = false;
	if(!bAI)
	{
		saveDataTimer -= 1;
		_ReadPlayerDataFromDatabase(uid, chairID, sql, &bAdd);
		realUserCount ++;
		ASSERT(realUserCount<=GAME_PLAYER);
	}
	else
	{//���ݿ���û�л�������Ϣ
		memset(&player[chairID].alData,0,sizeof(SPlayer));
		player[chairID].alData.playerRegulation = 5;//����Ѷ�
	}
	
	player[chairID].isAI=bAI;
	player[chairID].uid=uid;
	player[chairID].alData.playerID = uid;
	player[chairID].alData.playerGunScore = _GetMinGunScore();
	player[chairID].cannonType = 0;
	player[chairID].serverAcceptEnergy = 0;
	player[chairID].bActive = true;
	player[chairID].IP = IP;
	player[chairID].lscore = lscore;
	player[chairID].lscoreLogin = lscore;

	memcpy(player[chairID].szName, name, NAME_LEN);
	player[chairID].szName[NAME_LEN-1] = 0;


	if (bAdd)
	{
		_SavePlayerDataToDatabase(uid, chairID, sql);
	}

	//Print(
	//	"���ID:"<<player[chairID].alData.playerID<<"\n"
	//	"���Ŀǰ����:"<<player[chairID].alData.playerNowScore<<"\n"
	//	"��ҵ����Ϸ���:"<<player[chairID].alData.playerNowPutScore<<"\n"
	//	"������Ϸ�ֵ:"<<player[chairID].alData.playerPutScore<<"\n"
	//	"������·�ֵ:"<<player[chairID].alData.playerGetScore<<"\n"
	//	"���������:"<<player[chairID].alData.playerShootingTime<<"\n"
	//	"�������ֵ:"<<player[chairID].alData.playerMaxScore<<"\n"
	//	"�����������:"<<player[chairID].alData.playerDeadTime<<"\n"
	//	"���ÿ�ڷ���:"<<player[chairID].alData.playerGunScore<<"\n"
	//	"�ֶ��ٿ�Ӱ��ֵ:"<<player[chairID].alData.playerRegulation<<"\n"
	//	"�����ʧ����:"<<player[chairID].lostScore<<"\n"
	//	"�������:"<<player[chairID].szName<<"\n"
	//	);
}

bool CGameLogic::ClearChair( DWORD uid, int chairID, SQLiteOperate& sql )
{
	ASSERT(IsChairHasUser(chairID));
	totalUserCount--;
	if(!IsChairAI(chairID))
	{
		realUserCount--;
		ASSERT(realUserCount>=0);
		realUserCount = max(0,realUserCount);
	}

	bool ret = AccountChair(uid, chairID, sql);
	if (ret==false)
	{
		for (int i=0;i<10;i++)
		{
			ret = AccountChair(uid, chairID, sql);
			if(ret)break;
			Sleep(1);
		}
	}
	memset(&player[chairID].alData, 0, sizeof(SPlayer));
	player[chairID].uid = INVALID_DWORD;
	player[chairID].errorCount = 0;
	player[chairID].bActive = false;
	player[chairID].lostScore = 0;
	return ret;
}

void CGameLogic::_AcountScore( int chairID )
{
	player[chairID].alData.playerGetScore += player[chairID].alData.playerNowScore;
	player[chairID].alData.playerNowScore = 0;
	player[chairID].alData.playerNowPutScore = 0;
	CheckAndModifyPlayerData(&player[chairID].alData);
}

bool CGameLogic::AccountChair( DWORD uid, int chairID, SQLiteOperate& sql)
{
	_AcountScore(chairID);
	if(IsChairAI(chairID))return true;
	return _SavePlayerDataToDatabase(uid, chairID, sql);
}

void CGameLogic::AddScore( int chairID, int score, bool upscore )
{
	player[chairID].alData.playerNowScore += score;
	if(upscore)
	{//�Ϸ�
		player[chairID].alData.playerPutScore += score;
		player[chairID].alData.playerNowPutScore += score;
		CheckAndModifyPlayerData(&player[chairID].alData);
	}

	player[chairID].alData.playerMaxScore = max(player[chairID].alData.playerMaxScore, 
		player[chairID].alData.playerNowScore);
}

void CGameLogic::_AddScoreByAlgorithm( int chairID, int score )
{
	player[chairID].alData.playerNowScore += score;
	if (player[chairID].alData.playerNowScore == 0 && pTable->IsFireListEmpty(chairID))
	{
		player[chairID].alData.playerDeadTime++;
		Print("��ҷ������� uid:"<<GetUID(chairID));
	}
}

void CGameLogic::SetTable( CTableFrameSink* table )
{
	pTable = table;
	
	saveTimeDelay = (float)pTable->GetMaxTable()*ONE_TABLE_DELAY;
	saveDataTimer = (float)pTable->GetID()*(saveTimeDelay/pTable->GetMaxTable());
}

void CGameLogic::ReadRoomData( const char* roomName_ ,int roomPort_)
{
	_ReadRoomData(roomName_, this->sql_,roomPort_);
}

void CGameLogic::_ReadRoomData( const char* roomName_, SQLiteOperate& sql ,int roomPort_)
{
	bool b = false;
	if (roomName_ == NULL)return;
	if (pTable->GetID()!=1)return;
	roomPort = roomPort_;
	strcpy_s(roomName,sizeof(roomName), roomName_);

	b = sql.ReadRoomData(roomName, roomData,roomPort_);

	if (!b)
	{
		MessageBox(NULL,"��ȡ����ʧ��","����",0);
		throw std::exception("��ȡ����ʧ��");

		LogError("��ȡ����ʧ�� ��������:"<<roomName);
		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
		roomData.onceUpScore = DEFFALUT_UP_SCORE;

		roomData.factor = DEFFALUT_FACTOR;
		roomData.level = DEFFALUT_LEVEL;
		return;
	}

	if (roomData.roomMaxCost <= 10)
	{
		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
		LogError("roomName:"<<roomName<<" ��ȡʧ�ܣ�����Ĭ��ֵ"<<DEFFALUT_ROOM_MAX_COST);
	}
	if (roomData.onceUpScore <= 10)
	{
		roomData.onceUpScore = DEFFALUT_UP_SCORE;
		LogError("�����Ϸ���������! onceUpScore:"<<roomData.onceUpScore<<" ʹ��Ĭ��ֵ"<<DEFFALUT_UP_SCORE);
	}

	if (roomData.level > 5 || roomData.level < 1)
	{
		roomData.level = DEFFALUT_LEVEL;
		LogError("roomData.level ����");
	}

	if (roomData.factor < 0.1f || roomData.factor > 2.f)
	{
		roomData.factor = DEFFALUT_FACTOR;
		LogError("roomData.factor ����");
	}

	if (b)
	{
		//_Print_("��ȡ���ķ�����Ϣ onceUpScore:"<<roomData.onceUpScore<<
		//	" roomMaxCost:"<<roomData.roomMaxCost<<
		//	" factor:"<<roomData.factor<<
		//	" level:"<<roomData.level
		//	);
	}
}

void CGameLogic::_UpdateRoomData()
{
	//CAutoLock lock(&csSaveLock);
	bool b= false;
	//if (pTable->GetID()!=1)return;

	SRoomData lastData = roomData;
	b = sql_.ReadRoomData(roomName, roomData,roomPort);

	if (lastData.onceUpScore != roomData.onceUpScore ||
		lastData.roomMaxCost != roomData.roomMaxCost ||
		lastData.factor != roomData.factor ||
		lastData.level != roomData.level)
	{
		//_Print_("���·�����Ϣ onceUpScore:"<<roomData.onceUpScore<<
		//	" roomMaxCost:"<<roomData.roomMaxCost<<
		//	" factor:"<<roomData.factor<<
		//	" level:"<<roomData.level
		//	);
	}

	if (!b)
	{
		roomData = lastData;
		return;
	}

	if (roomData.roomMaxCost <= 0)
	{
		roomData.roomMaxCost = DEFFALUT_ROOM_MAX_COST;
		LogError("roomName:"<<roomName<<" ��ȡʧ�ܣ�����Ĭ��ֵ"<<DEFFALUT_ROOM_MAX_COST);
	}
	if (roomData.onceUpScore <= 0)
	{
		roomData.onceUpScore = DEFFALUT_UP_SCORE;
		LogError("�����Ϸ���������! onceUpScore:"<<roomData.onceUpScore<<" ʹ��Ĭ��ֵ"<<DEFFALUT_UP_SCORE);
	}

	if (roomData.level > 5 || roomData.level < 0)
	{
		roomData.level = DEFFALUT_LEVEL;
		LogError("roomData.level ����");
	}

	if (roomData.factor < 0 || roomData.factor > 2.f)
	{
		roomData.factor = DEFFALUT_FACTOR;
		LogError("roomData.factor ����");
	}
}

void CGameLogic::_dberror( const char* msg )
{
	//CAutoLock lock(&csSaveLock);
	LogError(msg);
}

int CGameLogic::GetRatioByTpye( int type )
{

	switch(type)
	{
	case 1:
	case 2:return 2;
	case 3:return 3;
	case 4:return 4;
	case 5:return 5;
	case 6:return 6;
	case 7:return 7;
	case 8:return 8;
	case 9:return 9;
	case 10:return 10;
	case 11:return 12;
	case 12:return 15;
	case 13:return 18;
	case 14:return 20;
	case 15:return 25;
	case 16:return 30;
	case 17:return 40;
	case 18:return 150;
	case 19:return 200;
	case 20:return 300;
	case 23:return 20;
	default:return 10;
	}

	if (type<1 || type>MAX_FISH_TYPE)
	{
		LogError("fish type error type:"<<type);
		return 10;
	}
}

//bool CGameLogic::HasNoneAIUser() const
//{
//	for (int i=0;i<GAME_PLAYER;i++)
//	{
//		if (IsChairHasUser(i) && !IsChairAI(i))//���û������û����ǻ�����
//		{
//			return true;
//		}
//	}
//	return false;
//}



//////////////////////////////////////////////////////////////////////////




