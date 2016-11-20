
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
#define YJALGORITHM					0//是否使用算法

#define IDI_TIMER_TICK					1//	服务器心跳
#define IDI_TIMER_TICK2				2

#define TIMER_TICK_RATE				1000//心跳间隔


//////////////////////////////////////////////////////////////////////////


#ifdef  FINAL_VERSION
#define CHANGE_SCENE_TIME_DELAY		500//场景切换时间
#else
#define CHANGE_SCENE_TIME_DELAY		30//场景切换时间
#endif

#define CHANGE_SCENE_TIME_WAIT		8//场景切换等待时间
#define NORMAL_FISH_TIME_DELAY		3//普通鱼出鱼间隔
#define SPECAIL_FISH_TIME_DELAY		10//特殊鱼出鱼间隔
#define GROUP_FISH_TIME_DELAY		25//阵型时间
#define LOCK_SCREEN_TIME_DELAY		5//定屏炸弹5秒

//鱼
struct Fish_Object
{
    WORD wID;//ID
    WORD wRoundID;//组群ID
    WORD wType; //路线类型
    float time;//鱼的激活时间戳
	bool notKillByBomb;
};

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction, public IErrorCallBack
{
private:
	//static STableDBStatus allTableStatus[100];
	//CSqlite3Logging							m_logging;

protected:
	int m_id;
	DWORD									m_tableStartTime;
    BYTE                                    m_cbScene;							

    int										m_nFishCount; // 鱼记数
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

	float									m_sceneStartTime;//场景切换时间戳
	float									m_waitChangSceneTime;//场景切换等待时间戳(客户端切换场景需要时间)
	float									m_normalFishTime;//普通鱼时间戳
	float									m_specialFishTime;//特殊鱼时间戳
	float									m_groupTime;//阵型时间戳

	CMD_S_Fish_Group						m_groupToSendWhenUserIn;//当玩家在阵型期间进入时发送此消息

	
	CGameLogic						        m_GameLogic;//游戏逻辑
	//lua_State*									m_pLua;


	//CThreadEvent							m_TimerEvent;
	//CThreadEvent							m_TableEvent;

	SQLiteOperate sql;
	CsLock									m_csLock;

private:
	int				script_CheckAndSendRewordNotice;

    //组件变量
protected:		
    ITableFrame						         * m_pITableFrame;				    //框架接口
    ITableFrameControl				         * m_pITableFrameControl;			//框架接口
    ITableFrameManager						 * m_pITableFrameManager;			//框架管理接口
    const tagGameServiceOption		         * m_pGameServiceOption;			//配置参数

public:
    CTableFrameSink();
    virtual ~CTableFrameSink();

	//////////////////////////////////////////////////////////////////////////
	//IUnknownEx 
    //释放对象
    virtual VOID __cdecl Release() { delete this;}
    //接口查询
    virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//IUnknownEx
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//ITableFrameSink
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void __cdecl RepositTableFrameSink(){}

	//信息接口
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode(){return enStartMode_TimeControl;}
	//游戏状态
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

    //游戏事件
    //游戏开始
	virtual bool __cdecl OnEventGameStart(){return true;}
    //游戏结束
    virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //发送场景
    virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

    //事件接口
    //定时器事件
    virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
    //游戏消息处理
    virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //框架消息处理
    virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//ITableFrameSink
	//////////////////////////////////////////////////////////////////////////
   
	//////////////////////////////////////////////////////////////////////////
	//ITableUserAction 用户动作事件
    //用户断线
    virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) {return true;}
    //用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;}
    //用户坐下
    virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //用户起来
    virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //用户同意
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
	bool SendGroup();//阵型

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
	void InitGroupFish10_11();//10和11的鱼相同

	//bool CheckFishID(CMD_C_Fish_Net_Object* list, int count);
	void InitFish(int fishID);
	void AliveFish( int fishID, WORD type, float time);
	bool KillFish(WORD fishID);
	void SetFishNotKillByBomb(int fishID, bool b){m_FishObjects[fishID].notKillByBomb = b;}
	bool IsFishAlive(WORD fishID)const{return m_FishObjects[fishID].wID != INVALID_WORD;}
	bool IsFishInValid(WORD wFishID, WORD wFishType)const;//是否是无效鱼
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
	bool SetUserFree(IServerUserItem * pIServerUserItem);//设置玩家空闲，否则玩家不能离开桌子
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
	for (int i=0;i<number;i++)//激活number条鱼
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
