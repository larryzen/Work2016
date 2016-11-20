
#pragma once

#include "Stdafx.h"
//#include "lua.hpp"
#include "GameLogic.h"
#include "FishTable.h"
#include <list>
#include <vector>
#include <sstream>
#include "SQLiteOperate.h"
//#include "LuaScript.h"

//////////////////////////////////////////////////////////////////////////
#define YJALGORITHM					0//�Ƿ�ʹ���㷨

#define IDI_TIMER_TICK					1//	����������
#define IDI_TIMER_TICK2				2

#define TIMER_TICK_RATE				1000//�������


//////////////////////////////////////////////////////////////////////////


#ifdef  FINAL_VERSION
#define CHANGE_SCENE_TIME_DELAY		500//�����л�ʱ��
#else
#define CHANGE_SCENE_TIME_DELAY		30//�����л�ʱ��
#endif

#define CHANGE_SCENE_TIME_WAIT		8//�����л��ȴ�ʱ��
#define NORMAL_FISH_TIME_DELAY		3//��ͨ�������
#define SPECAIL_FISH_TIME_DELAY		10//�����������
#define GROUP_FISH_TIME_DELAY		25//����ʱ��
#define LOCK_SCREEN_TIME_DELAY		5//����ը��5��

//��
struct Fish_Object
{
    WORD wID;//ID
    WORD wRoundID;//��ȺID
    WORD wType; //·������
    float time;//��ļ���ʱ���
	bool notKillByBomb;
};

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction, public IErrorCallBack
{
private:
	//static STableDBStatus allTableStatus[100];
	//CSqlite3Logging							m_logging;

protected:
	int m_id;
	DWORD									m_tableStartTime;
    BYTE                                    m_cbScene;							

    int										m_nFishCount; // �����
	int										m_maxFishID;
    WORD                                    m_wSceneSendFishCount;

	typedef std::list<int> CostList;
	CostList								m_bulletList[GAME_PLAYER];
	//CostList								m_bombList[GAME_PLAYER];

    Fish_Object                         m_FishObjects[MAX_FISH_OBJECT];

	//////////////////////////////////////////////////////////////////////////
	//int										m_nowUserCount;


	bool									m_tickTimerStop;

	Fish_Time								m_normalFishTimeTable[MAX_NORMAL_FISH_TYPE];
	Fish_Time								m_spFishTimeTable[MAX_SP_FISH_TYPE];

	typedef std::vector<int> TypeList;
	TypeList								m_typeList;

	TypeList								m_spHasIDList;
	TypeList								m_teamTypeList;
	TypeList								m_spTeamTypeList;
	TypeList								m_typeBombList;
	TypeList								m_waterPlantList;

	enum TableState
	{
		eNormal,
		eWaitChangeScene,
		eGroup,
	};
	TableState								m_tableState;

	float									m_sceneStartTime;//�����л�ʱ���
	float									m_waitChangSceneTime;//�����л��ȴ�ʱ���(�ͻ����л�������Ҫʱ��)
	float									m_normalFishTime;//��ͨ��ʱ���
	float									m_specialFishTime;//������ʱ���
	float									m_groupTime;//����ʱ���

	CMD_S_Fish_Group						m_groupToSendWhenUserIn;//������������ڼ����ʱ���ʹ���Ϣ

	
	CGameLogic						        m_GameLogic;//��Ϸ�߼�
	//lua_State*									m_pLua;


	//CThreadEvent							m_TimerEvent;
	//CThreadEvent							m_TableEvent;

	SQLiteOperate sql;
	CsLock									m_csLock;

private:
	int				script_CheckAndSendRewordNotice;

    //�������
protected:		
    ITableFrame						         * m_pITableFrame;				    //��ܽӿ�
    ITableFrameControl				         * m_pITableFrameControl;			//��ܽӿ�
    ITableFrameManager						 * m_pITableFrameManager;			//��ܹ���ӿ�
    const tagGameServiceOption		         * m_pGameServiceOption;			//���ò���

public:
    CTableFrameSink();
    virtual ~CTableFrameSink();

	//////////////////////////////////////////////////////////////////////////
	//IUnknownEx 
    //�ͷŶ���
    virtual VOID __cdecl Release() { delete this;}
    //�ӿڲ�ѯ
    virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//IUnknownEx
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//ITableFrameSink
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink(){}

	//��Ϣ�ӿ�
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode(){return enStartMode_TimeControl;}
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

    //��Ϸ�¼�
    //��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart(){return true;}
    //��Ϸ����
    virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //���ͳ���
    virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

    //�¼��ӿ�
    //��ʱ���¼�
    virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
    //��Ϸ��Ϣ����
    virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //�����Ϣ����
    virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//ITableFrameSink
	//////////////////////////////////////////////////////////////////////////
   
	//////////////////////////////////////////////////////////////////////////
	//ITableUserAction �û������¼�
    //�û�����
    virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) {return true;}
    //�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;}
    //�û�����
    virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //�û�����
    virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //�û�ͬ��
    virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }
	//ITableUserAction
	//////////////////////////////////////////////////////////////////////////
protected:
    bool OnBuyBullet(IServerUserItem * pIServerUserItem, CMD_C_Buy_Bullet *pBuyBullet);
    bool OnFire(IServerUserItem * pIServerUserItem, CMD_C_Fire *pFire);
	bool OnLockFire(IServerUserItem * pIServerUserItem, CMD_C_Fire_Lock *pFire);
    bool OnChangeCannon(IServerUserItem * pIServerUserItem, CMD_C_Change_Cannon *pChangeCannon);
    bool OnAccount(IServerUserItem * pIServerUserItem, CMD_C_Account *pAccount);
	bool OnCastNet(IServerUserItem * pIServerUserItem, CMD_C_Cast_Net_dte *pCastNet);
	bool OnHitTypeBomb(IServerUserItem * pIServerUserItem, CMD_C_TpyeBomb_dte *pBomb);
	bool OnHitScreenBomb(IServerUserItem * pIServerUserItem, CMD_C_Hit_Screen_Bomb *pBomb);
	bool OnHitTeam(IServerUserItem * pIServerUserItem, CMD_C_HitTeam_dte* pTeam);
	bool OnHitBoat(IServerUserItem * pIServerUserItem, CMD_C_HitBoat* pBoat);

protected:
	void OneUserLeave(int chairId);
	void KillTimer();
	void WriteUserScore(WORD wChairID, IServerUserItem * pIServerUserItem);

	void OnTimerTick();

	void OnChangeScene();
	void OnNormalFish(float delta);
	void CreateSpecialFish(float delta);
	void CreateNormalFish(float delta);
	void OnGroupStart();

    bool SendNormalFish(const TypeList& list);
    bool SendSpecialFish(int type, const TypeList& list);
	bool SendGroup();//����

	void CheckAndSendRewordNotice(WORD wChairID, IServerUserItem * pIServerUserItem, DWORD addScore);

	bool SendGroupWhenUserIn(WORD wChairID);

public:
	bool SendRewordNotice(WORD wChairID, int type, DWORD value);
	
protected:
    int GetFreeFishID();
    WORD GetNextFishID();

    void CheckFishDestroy();
	void DesroyAllFish();

	void ActiveGroupFish(int number);
	inline void SetGroupFish(int from, int to, int type);
	void InitGroupFish1();
	void InitGroupFish2();
	void InitGroupFish3();
	void InitGroupFish4();
	void InitGroupFish5();
	void InitGroupFish6();
	void InitGroupFish7();
	void InitGroupFish8();
	void InitGroupFish9();
	void InitGroupFish10_11();//10��11������ͬ

	//bool CheckFishID(CMD_C_Fish_Net_Object* list, int count);
	void InitFish(int fishID);
	void AliveFish( int fishID, WORD type, float time);
	bool KillFish(WORD fishID);
	void SetFishNotKillByBomb(int fishID, bool b){m_FishObjects[fishID].notKillByBomb = b;}
	bool IsFishAlive(WORD fishID)const{return m_FishObjects[fishID].wID != INVALID_WORD;}
	bool IsFishInValid(WORD wFishID, WORD wFishType)const;//�Ƿ�����Ч��
	bool IsFishNotKillByBomb(WORD wFishID)const{return m_FishObjects[wFishID].notKillByBomb;}
	WORD GetFishID(WORD wFishID)const{return m_FishObjects[wFishID].wID;}
	WORD GetFishType(WORD wFishID)const{return m_FishObjects[wFishID].wType;}
	float GetFishTime(WORD wFishID)const{return m_FishObjects[wFishID].time;}
public:
	float getTime()const{return (GetTickCount()-m_tableStartTime)/1000.0f;}
	bool SendChangeCannon(WORD wChairID, WORD cost, BYTE cbType);
	bool SendChangeEnergy(WORD wChairID, BYTE cbEnergy);
	bool IsFireListEmpty(int chairID)const{return m_bulletList[chairID].size()==0;}
	bool SendTableData(WORD chairId, WORD subId, void* pData, WORD size);
	int GetID()const{return m_id;}
	int GetMaxTable()const{return m_pGameServiceOption->wTableCount;}
	bool KickUser(IServerUserItem * pIServerUserItem){return m_pITableFrameManager->PerformStandUpAction(pIServerUserItem);}
	bool SetUserFree(IServerUserItem * pIServerUserItem);//������ҿ��У�������Ҳ����뿪����
	bool SendRoomGolobalMessage(const std::string& msg,int type);
	bool SendRoomGolobalMessage(IServerUserItem * pIServerUserItem,const std::string& msg,int type);

	virtual void LogError( const char* msg ){/*m_logging.Log(msg);*/}

	void thread_func();
	HANDLE hThread;
};

//////////////////////////////////////////////////////////////////////////
inline void CTableFrameSink::SetGroupFish( int from, int to, int type )
{
	for (int i=from;i<=to;i++)
		m_FishObjects[i].wType = type;
}

inline bool CTableFrameSink::KillFish( WORD fishID )
{
	//ASSERT(IsFishAlive(fishID));
	if (!IsFishAlive(fishID))
	{
		return false;
	}
	m_FishObjects[fishID].wID = INVALID_WORD;
	m_nFishCount--;
	return true;
}

inline void CTableFrameSink::AliveFish( int fishID, WORD type, float time)
{
	m_FishObjects[fishID].wID = fishID;
	m_FishObjects[fishID].wType = type;
	m_FishObjects[fishID].time = time;
	m_nFishCount++;
}	

inline bool CTableFrameSink::IsFishInValid( WORD wFishID, WORD wFishType ) const
{
	return m_FishObjects[wFishID].wID!=wFishID || m_FishObjects[wFishID].wType!=wFishType;
}

inline void CTableFrameSink::ActiveGroupFish( int number )
{
	float time = getTime();
	for (int i=0;i<number;i++)//����number����
	{
		m_FishObjects[i].wID = i;
		m_FishObjects[i].time = time;
		AliveFish(i, 1, time);
	}
}

inline void CTableFrameSink::InitFish( int fishID )
{
	m_FishObjects[fishID].wID = INVALID_WORD;
	m_FishObjects[fishID].wRoundID= 0;
	m_FishObjects[fishID].wType = 0;
	m_FishObjects[fishID].time = 0;
	m_FishObjects[fishID].notKillByBomb = false;
}

//////////////////////////////////////////////////////////////////////////
