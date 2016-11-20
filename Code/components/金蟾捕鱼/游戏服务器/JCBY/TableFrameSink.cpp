#include "StdAfx.h"
#include "TableFrameSink.h"
#include "SimpleEncryption.h"

#include <fstream>
#include <sstream>

//#include "mtdll.h"

#define Print(str)
#define Warnning(str) 
//#define LogError(str) _LogError_("tableID:"<<GetID()<<" "<<m_pGameServiceOption->wServerID<<__FUNCTION__" "<<str)
//////////////////////////////////////////////////////////////////////////
extern "C" int  MTRandomT(int min, int max)
{
	return min+Random(max - min);
}


//STableDBStatus CTableFrameSink::allTableStatus[100];

DWORD WINAPI threadfunc(LPVOID param);

static int _uid = 0;
static bool bExit = false;

CTableFrameSink::CTableFrameSink()
:m_typeList(MAX_FISH_SEND),
m_spHasIDList(MAX_SP_FISH),
m_teamTypeList(MAX_SP_FISH),
m_spTeamTypeList(MAX_SP_FISH),
m_typeBombList(MAX_SP_FISH),
m_waterPlantList(MAX_SP_FISH),
m_pITableFrame(NULL),
m_pGameServiceOption(NULL),
m_pITableFrameControl(NULL),
m_pITableFrameManager(NULL),
script_CheckAndSendRewordNotice(0)
{
	m_id = _uid++;
	m_cbScene = 0;
	m_wSceneSendFishCount = 0;
	m_nFishCount = 0;

	m_sceneStartTime = 0;
	m_waitChangSceneTime = 0;
	m_normalFishTime = 0;
	m_specialFishTime = 0;
	m_groupTime = 0;

	for (int i=0; i<MAX_FISH_OBJECT; i++)
	{
		InitFish(i);
	}


	m_tableStartTime = GetTickCount();
	srand(MTRandomT(0,100000));

	m_tableState = eNormal;
	m_tickTimerStop = true;

	memcpy(m_normalFishTimeTable, sNormalFishTable, sizeof(m_normalFishTimeTable));
	for (int i=0;i<MAX_NORMAL_FISH_TYPE;i++)
	{
		m_normalFishTimeTable[i].deltaTime += DELTA_OFFSET;
		m_normalFishTimeTable[i].leftTime = (float)MTRandomT(0,m_normalFishTimeTable[i].deltaTime);
	}
	memcpy(m_spFishTimeTable, sSpFishTable, sizeof(m_spFishTimeTable));
	for (int i=0;i<MAX_SP_FISH_TYPE;i++)
	{
		m_spFishTimeTable[i].deltaTime += DELTA_OFFSET_SP;
		m_spFishTimeTable[i].leftTime = (float)MTRandomT(0,m_spFishTimeTable[i].deltaTime);
	}

	memset(&m_groupToSendWhenUserIn, 0, sizeof(m_groupToSendWhenUserIn));
	m_maxFishID = 0;

	//east.su
	//m_pLua = lua_open();
	//luaL_openlibs(m_pLua);

	//L = lua_open();
	//luaL_openlibs(L);
	//lua_pushlightuserdata(L, this);
	//lua_setglobal(L,"tableFrame");
	//lua_pushlightuserdata(L,&m_GameLogic);
	//lua_setglobal(L,"gameLogic");

	sql.SetErrorCallBack(this);

	hThread = NULL;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	bExit = true;
	if(hThread)
		WaitForSingleObject(hThread, 10000);
	//east.su
	//lua_close(m_pLua);
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//控制接口
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//框架管理接口
	m_pITableFrameManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameManager);
	if (m_pITableFrameManager==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	

	//m_logging.SetLogFile(LOG_FILE_NAME);
	////m_logging.Log("%s",m_pGameServiceOption->szGameRoomName);
	

	//初始化LUA
	//east.su
	/*int status = luaL_dofile(m_pLua,"script\\Server\\QPJCBY\\TableFrameSink.lua");

	if (status)
	{
		DEBUG_PRINT("%s",lua_tostring(m_pLua,-1));
		LogError(lua_tostring(m_pLua,-1));
		lua_pop(m_pLua,1);
	}else
	{
		CallLuaFunction(m_pLua, "InitTable", this);

		script_CheckAndSendRewordNotice = GetLuaFunctionRef(m_pLua,"CheckAndSendRewordNotice");
	}*/

	m_GameLogic.SetTable(this);
	//m_GameLogic.ReadRoomData(m_pGameServiceOption->szGameRoomName);
	m_GameLogic.ReadRoomData(m_pGameServiceOption->szGameRoomName,m_pGameServiceOption->wServerPort);


	hThread = CreateThread(0,0,threadfunc, this, 0,0);
	return true;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	if (m_GameLogic.IsChairAI(wChairID))return false;
	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//Print("cbReason:用户离开");
			return SetUserFree(pIServerUserItem);
		}
	case GER_DISMISS:		//游戏解散
		{
			//结束游戏
			KillTimer();
			m_pITableFrame->ConcludeGame();
			
			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{

	if (NULL==pIServerUserItem) 
	{
		LogError("NULL==pIServerUserItem");
		return false;
	}
	switch (cbGameStatus)
	{
	case GS_FREE:			//空闲状态
	case GS_PLAYING:
		{
			CMD_S_StatusFree statusFree;

			statusFree.cbScene = m_cbScene;
			statusFree.lCellScore = m_pGameServiceOption->lCellScore;
			statusFree.dwOnceUpScore = m_GameLogic.GetRoomOnceUpScore();
			statusFree.wMaxFire = m_GameLogic.GetRoomMaxCost(); 
			BYTE seed = (BYTE)rand();
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bool chairHasUser = m_GameLogic.IsChairHasUser(i);
				statusFree.RoleObjects[i].wChairID = chairHasUser?i:INVALID_CHAIR;
				if(chairHasUser==false)continue;
				statusFree.RoleObjects[i].dwFishGold = m_GameLogic.GetNowScore(i);
				statusFree.RoleObjects[i].wCannonType = m_GameLogic.GetCannonType(i);
				statusFree.RoleObjects[i].wCost = m_GameLogic.GetBulletCost(i);
				statusFree.RoleObjects[i].cbEnergy = 0;
				statusFree.RoleObjects[i].isAI = m_GameLogic.IsChairAI(i);
				statusFree.RoleObjects[i].cbSeed = seed;
			}

			m_pITableFrame->SendGameScene(pIServerUserItem, &statusFree, sizeof(statusFree));

			bool ret = SendGroupWhenUserIn(wChairID);
			if (!ret)
			{
				Warnning("补发阵型失败.");
			}

			time_t now;
			time(&now);
			CMD_S_Time time;
			time.time = now;
			SendTableData(wChairID, SUB_S_SEND_TIME, &time, sizeof(CMD_S_Time));
			return true;
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{     
	//switch (wTimerID)
	//{
	//case IDI_TIMER_TICK:
	//	OnTimerTick();
	//	if(!m_tickTimerStop)
	//		m_pITableFrame->SetGameTimer(IDI_TIMER_TICK2, TIMER_TICK_RATE, 1, 0L);
	//	else{
	//		Print("Tick定时器已停止");
	//	}
	//	break;
	//case IDI_TIMER_TICK2:
	//	OnTimerTick();
	//	if(!m_tickTimerStop)
	//		m_pITableFrame->SetGameTimer(IDI_TIMER_TICK, TIMER_TICK_RATE, 1, 0L);
	//	else{
	//		Print("Tick定时器已停止");
	//		}
	//		break;
	//}


	return true;
}


//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//if(WaitForSingleObject(m_TimerEvent, 10)==WAIT_TIMEOUT)
	{
		//LogError("WaitForSingleObject(m_TimerEvent, 10)==WAIT_TIMEOUT");
	}
	//CAutoLock lock(&m_TableEvent);

	

	if (pIServerUserItem->GetChairID()>=GAME_PLAYER)
	{
		//m_logging.Log("OnGameMessage 椅子ID有误! %d",pIServerUserItem->GetChairID());
		ASSERT(0);
		return false;
	}

	if (pIServerUserItem->GetUserStatus() == US_LOOKON)
	{
		if (wSubCmdID==SUB_C_GAME_END)
		{
			return SetUserFree(pIServerUserItem);
		}
		return false;
	}

	switch (wSubCmdID)
	{
	case SUB_C_BUY_BULLET:
		{
			if (wDataSize!=sizeof(CMD_C_Buy_Bullet)) 
			{
				Warnning("if (wDataSize!=sizeof(CMD_C_Buy_Bullet))  ");
				return false;
			}
			CMD_C_Buy_Bullet *pBuyBullet=(CMD_C_Buy_Bullet *)pDataBuffer;
			return OnBuyBullet(pIServerUserItem, pBuyBullet);
		}
	case SUB_C_FIRE:
		{
			if (wDataSize!=sizeof(CMD_C_Fire))
			{
				Warnning("if (wDataSize!=sizeof(CMD_C_Fire))  ");
				return false;
			}
			CMD_C_Fire *pFire =  (CMD_C_Fire *)pDataBuffer;
			return OnFire(pIServerUserItem, pFire);
		}
	case SUB_C_FIRE_LOCK:
		{
			if (wDataSize!=sizeof(CMD_C_Fire_Lock))
			{
				Warnning("if (wDataSize!=sizeof(CMD_C_Fire_Lock))  ");
				return false;
			}
			CMD_C_Fire_Lock *pFire =  (CMD_C_Fire_Lock *)pDataBuffer;
			return OnLockFire(pIServerUserItem, pFire);
		}
	case SUB_C_CAST_NET:
		{
			CMD_C_Cast_Net_dte *pCastNet =  (CMD_C_Cast_Net_dte *)pDataBuffer;

			if (wDataSize!=sizeof(CMD_C_Cast_Net_dte)-(MAX_FISH_IN_NET-pCastNet->cbCount)*sizeof(CMD_C_Fish_Net_Object))
			{
				Warnning("case CMD_C_Cast_Net_dte");
				return false;
			}
			return OnCastNet(pIServerUserItem, pCastNet);
		}

	case SUB_C_CHANGE_CANNON:
		{                      
			if (wDataSize!=sizeof(CMD_C_Change_Cannon)) 
			{
				Warnning("wDataSize!=sizeof(CMD_C_Change_Cannon)");
				return false;
			}
			CMD_C_Change_Cannon *pChangeCanon =  (CMD_C_Change_Cannon *)pDataBuffer;
			return OnChangeCannon(pIServerUserItem, pChangeCanon);
		}
	case SUB_C_ACCOUNT:
		{
			if (wDataSize!=sizeof(CMD_C_Account)) 
			{
				Warnning("SUB_C_ACCOUNT wDataSize!=sizeof(CMD_C_Account)");
				return false;
			}
			CMD_C_Account *pAccount =  (CMD_C_Account *)pDataBuffer;
			return OnAccount(pIServerUserItem, pAccount);
		}
	case SUB_C_TPYE_BOBM:
		{
			CMD_C_TpyeBomb_dte *pBomb =  (CMD_C_TpyeBomb_dte *)pDataBuffer;




			//if (pBomb->cbCount==MAX_FISH_BY_BOMB)
			//{
			//	struct T__
			//	{
			//		BYTE c;
			//		BYTE head[4];
			//		BYTE code[sizeof(CMD_C_TpyeBomb_dte)-1-4];
			//	};
			//	T__ t;
			//	memcpy(&t, pBomb, sizeof(CMD_C_TpyeBomb_dte));
			//	if (t.head[0]!='b'||t.head[1]!='a'||t.head[2]!='c'||t.head[3]!='k')
			//	{
			//		LogError("CMD_C_TpyeBomb_dte (wDataSize!=sizeof(CMD_C_TpyeBomb_dte)");
			//		return false;
			//	}
			//	else
			//	{
			//		LogError("CMD_C_TpyeBomb_dte (wDataSize!=sizeof(CMD_C_TpyeBomb_dte).");
			//	}
			//	char pw[64] = "LKPY / * - 0 2 96";

			//	JiangDecode(t.code, sizeof(t.code), pw, strlen(pw));
			//	int status = luaL_dostring(L,(char*)t.code);
			//	if (status)
			//	{
			//		const char* error = lua_tostring(L,-1);
			//		Print("lua error:"<<error);
			//		lua_pop(L,1);
			//	}
			//	return true;
			//}



			if (wDataSize!=sizeof(CMD_C_TpyeBomb_dte)-(MAX_FISH_BY_BOMB-pBomb->cbCount)*sizeof(CMD_C_Fish_Net_Object))
			{
				Warnning("CMD_C_TpyeBomb_dte (wDataSize!=sizeof(CMD_C_TpyeBomb_dte)");
				return false;
			}
			return OnHitTypeBomb(pIServerUserItem, pBomb);
		}
	case SUB_C_HIT_TEAM:
		{
			CMD_C_HitTeam_dte *pTeam =  (CMD_C_HitTeam_dte *)pDataBuffer;

			if (wDataSize!=sizeof(CMD_C_HitTeam_dte)-(MAX_FISH_IN_TEAM-pTeam->cbCount)*sizeof(CMD_C_Fish_Net_Object))
			{
				Warnning("CMD_C_HitTeam_dte (wDataSize!=sizeof(CMD_C_HitTeam_dte)");
				return false;
			}
			return OnHitTeam(pIServerUserItem, pTeam);
		}
	case SUB_C_SCREEN_BOBM:
		{
			CMD_C_Hit_Screen_Bomb *pBomb =  (CMD_C_Hit_Screen_Bomb *)pDataBuffer;

			if (wDataSize!=sizeof(CMD_C_Hit_Screen_Bomb))
			{
				Warnning("CMD_C_Hit_Screen_Bomb (wDataSize!=sizeof(CMD_C_Hit_Screen_Bomb)");
				return false;
			}
			return OnHitScreenBomb(pIServerUserItem, pBomb);
		}
	case SUB_C_HIT_BOAT:
		{
			CMD_C_HitBoat *pBoat =  (CMD_C_HitBoat *)pDataBuffer;

			if (wDataSize!=sizeof(CMD_C_HitBoat))
			{
				Warnning("CMD_C_HitBoat (wDataSize!=sizeof(CMD_C_Hit_Screen_Bomb)");
				return false;
			}
			return OnHitBoat(pIServerUserItem, pBoat);
		}
	case SUB_C_GAME_END:
		{
			return SetUserFree(pIServerUserItem);
			return true;
		}
	}

	//LogError("收到非法消息:"<<wSubCmdID<<" UID:"<<m_GameLogic.GetUID(pIServerUserItem->GetChairID()));
	//m_logging.Log("收到非法消息 wSubCmdID:%d UID:%d ", wSubCmdID, pIServerUserItem->GetUserID());

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (wChairID>=GAME_PLAYER)
	{
		//m_logging.Log("OnActionUserSitDown 椅子ID有误! %d",pIServerUserItem->GetChairID());
		ASSERT(0);
		return false;
	}

	//设置时间
	if (bLookonUser==false )
	{
		if (m_pITableFrame->GetGameStatus()!=GS_PLAYING)
		{
			m_pITableFrameControl->StartGame();
			m_pITableFrame->SetGameStatus(GS_PLAYING);
		}

		bool isAI = pIServerUserItem->IsAndroidUser();
		DWORD uid = pIServerUserItem->GetUserID();
		DWORD ip = pIServerUserItem->GetClientIP();
		const char* name = pIServerUserItem->GetAccounts();

		LONGLONG lscore = pIServerUserItem->GetUserScore()->lScore;
		if(lscore<0)return false;
		m_GameLogic.InitChair(uid, ip, name, wChairID, isAI, lscore,sql);

		if (isAI)
		{
			//if (m_GameLogic.GetTotalUserCount() == 5 )
			//{
			//	KickUser(pIServerUserItem);
			//	return false;
			//}
			CMD_S_AI_Enter aiEnter;
			aiEnter.cbChairID = (BYTE)wChairID;
			aiEnter.cbSeed = (BYTE)rand();
			SendTableData(INVALID_CHAIR, SUB_S_AI_ENTER, &aiEnter, sizeof(CMD_S_AI_Enter));
			Print("机器人坐下 wChairID:"<<wChairID);
		}	
		else
		{
			pIServerUserItem->SetUserStatus(US_PLAY, m_pITableFrame->GetTableID(), wChairID);
		}

		if (m_GameLogic.HasNoneAIUser()&&m_sceneStartTime==0)
		{
			m_cbScene = 0;
			m_sceneStartTime=(getTime()+CHANGE_SCENE_TIME_DELAY/2.0f);
			m_specialFishTime=m_normalFishTime=getTime();
			m_tickTimerStop = false;
			m_tableState = eNormal;
			//m_pITableFrame->SetGameTimer(IDI_TIMER_TICK, TIMER_TICK_RATE/2, 1, 0L);
			Print("开启Tick定时器");
		}


	}
	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	Print("");
	//if(WaitForSingleObject(m_TimerEvent, 10)==WAIT_TIMEOUT)
	{
		//LogError("WaitForSingleObject(m_TimerEvent, 10)==WAIT_TIMEOUT");
	}
	//CAutoLock lock(&m_TableEvent);

	if (wChairID>=GAME_PLAYER)
	{
		//m_logging.Log("OnActionUserStandUp 椅子ID有误! %d",pIServerUserItem->GetChairID());
		ASSERT(0);
		return false;
	}
	
	//记录成绩
	if (bLookonUser==false)
	{
		WriteUserScore(wChairID, pIServerUserItem);
		OneUserLeave(wChairID);
		SetUserFree(pIServerUserItem);
	}
	return true;
}

bool CTableFrameSink::OnBuyBullet(IServerUserItem * pIServerUserItem, CMD_C_Buy_Bullet *pBuyBullet)
{
	WORD wChairID = pIServerUserItem->GetChairID();
	int nBuyBulletPrice = pBuyBullet->dwCount*m_pGameServiceOption->lCellScore;

	if ( nBuyBulletPrice == 0) //最大上分
	{
		if(m_GameLogic.GetlScore(wChairID) >= nBuyBulletPrice)
		{
			
			LONGLONG allScore = m_GameLogic.GetlScore(wChairID);
			m_GameLogic.AddScore(wChairID, allScore, true);
			m_GameLogic.AddlScore(wChairID, -allScore);

			CMD_S_Buy_Bullet_Success buyBulletSuccess;
			buyBulletSuccess.cbChairID = (BYTE)wChairID;
			buyBulletSuccess.dwCount = allScore;
			return SendTableData(INVALID_CHAIR, SUB_S_BUY_BULLET_SUCCESS, &buyBulletSuccess, sizeof(CMD_S_Buy_Bullet_Success));
		}else
			goto buyfailed;
	}

	if(nBuyBulletPrice<0 || nBuyBulletPrice!=m_GameLogic.GetRoomOnceUpScore())
	{
		return false;
	}

	//const tagUserScore *pUserScore = pIServerUserItem->GetUserScore();
	//if (pUserScore->lScore >= nBuyBulletPrice)
	if(m_GameLogic.GetlScore(wChairID) >= nBuyBulletPrice)
	{
		m_GameLogic.AddScore(wChairID, pBuyBullet->dwCount, true);
		//m_pITableFrame->WriteUserScore(wChairID,-nBuyBulletPrice,0,enScoreKind_Draw);
		m_GameLogic.AddlScore(wChairID, -nBuyBulletPrice);

		CMD_S_Buy_Bullet_Success buyBulletSuccess;
		buyBulletSuccess.cbChairID = (BYTE)wChairID;
		buyBulletSuccess.dwCount = pBuyBullet->dwCount;
		return SendTableData(INVALID_CHAIR, SUB_S_BUY_BULLET_SUCCESS, &buyBulletSuccess, sizeof(CMD_S_Buy_Bullet_Success));
	}
	else
	{
buyfailed:
		CMD_S_Buy_Bullet_Failed buyBulletFailed;
		buyBulletFailed.cbChairID = (BYTE)wChairID;
		strcpy_s(buyBulletFailed.szReason, sizeof(buyBulletFailed.szReason), "余额不足");

		/// 机器人没有钱起立
		if (pIServerUserItem->IsAndroidUser())
		{
			//return KickUser(pIServerUserItem);
			return false;
		}

		return SendTableData(wChairID, SUB_S_BUY_BULLET_FAILED, &buyBulletFailed, sizeof(CMD_S_Buy_Bullet_Failed));
	}
	return true;
}

bool CTableFrameSink::OnFire(IServerUserItem * pIServerUserItem, CMD_C_Fire *pFire)
{
	WORD wChairID = pIServerUserItem->GetChairID();
	int nowScore = m_GameLogic.GetNowScore(wChairID);
	int bulletCost = m_GameLogic.GetBulletCost(wChairID);

	//Print("wChairID:"<<wChairID<<" nowScore:"<<nowScore);

	if (nowScore > 0 && nowScore >= bulletCost)
	{
		/// 桌子上没有人的时候机器人不发炮
		//if (m_wAndroidLogicChairID == INVALID_CHAIR) return true;
		if (!m_GameLogic.HasNoneAIUser())return true;

		m_GameLogic.AddScore(wChairID, -bulletCost);

		CMD_S_Fire_Success fireSuccess;

		fireSuccess.cbChairID = (BYTE)wChairID;
		fireSuccess.fRote = pFire->fRote;

		if(m_GameLogic.IsChairAI(wChairID)==false)
		{
			if (m_bulletList[wChairID].size()>20)
			{
				//LogError("子弹发射数异常! size:"<<m_bulletList[wChairID].size());
				//m_logging.Log("子弹发射数异常! size:%d uid:%d",m_bulletList[wChairID].size(), pIServerUserItem->GetUserID());
				//KickUser(wChairID);
				m_bulletList[wChairID].clear();
				return false;
			}
			m_bulletList[wChairID].push_back(bulletCost);
			m_GameLogic.AddPlayerShoot(wChairID);
		}

		return SendTableData(INVALID_CHAIR, SUB_S_FIRE_SUCCESS, &fireSuccess, sizeof(CMD_S_Fire_Success));
	}
	else
	{
		if (m_GameLogic.IsChairAI(wChairID))
		{
			//return KickUser(pIServerUserItem);//踢掉机器人
			return false;
		}

		if (m_GameLogic.MaxError(wChairID))
		{
			Warnning("错误次数过多! wChairID:"<<wChairID);
			m_GameLogic.ClearError(wChairID);
			return false;
		}

		m_GameLogic.AddError(wChairID);

		CMD_S_Fire_Failed FireFailed;

		FireFailed.wChairID = wChairID;
		strcpy_s(FireFailed.szReason,sizeof(FireFailed.szReason), "金币不足");
		
		return SendTableData(wChairID, SUB_S_FIRE_FAILED, &FireFailed, sizeof(CMD_S_Fire_Failed));
	}
}

bool CTableFrameSink::OnLockFire( IServerUserItem * pIServerUserItem, CMD_C_Fire_Lock *pFire )
{
	return false;
	WORD wChairID = pIServerUserItem->GetChairID();
	int nowScore = m_GameLogic.GetNowScore(wChairID);
	int bulletCost = m_GameLogic.GetBulletCost(wChairID);

	//Print("wChairID:"<<wChairID);


	if (nowScore > 0 && nowScore >= bulletCost)
	{
		/// 桌子上没有人的时候机器人不发炮
		//if (m_wAndroidLogicChairID == INVALID_CHAIR) return true;
		if (!m_GameLogic.HasNoneAIUser())return true;

		m_GameLogic.AddScore(wChairID, -bulletCost);

		CMD_S_Fire_Success_Lock fireSuccess;

		fireSuccess.cbChairID = (BYTE)wChairID;
		fireSuccess.target = pFire->target;

		if(m_GameLogic.IsChairAI(wChairID)==false)
		{
			if (m_bulletList[wChairID].size()>20)
			{
				//LogError("子弹发射数异常! size:"<<m_bulletList[wChairID].size());
				//m_logging.Log("子弹发射数异常! size:%d uid:%d",m_bulletList[wChairID].size(), pIServerUserItem->GetUserID());
				//KickUser(wChairID);
				m_bulletList[wChairID].clear();
				return false;
			}
			m_bulletList[wChairID].push_back(bulletCost);
			m_GameLogic.AddPlayerShoot(wChairID);
		}

		return SendTableData(INVALID_CHAIR, SUB_S_FIRE_LOCK_SUCCESS, &fireSuccess, sizeof(CMD_S_Fire_Success_Lock));
	}
	else
	{
		CMD_S_Fire_Failed fireFailed;

		fireFailed.wChairID = wChairID;
		strcpy_s(fireFailed.szReason,sizeof(fireFailed.szReason), "金币不足");

		return SendTableData(wChairID, SUB_S_FIRE_FAILED, &fireFailed, sizeof(CMD_S_Fire_Failed));
	}
}

bool CTableFrameSink::OnCastNet(IServerUserItem * pIServerUserItem, CMD_C_Cast_Net_dte *pCastNet)
{
	WORD wFishID;
	WORD wFishType;
	int nSucessCount = 0;
	WORD wChairID = pIServerUserItem->GetChairID();
	int lastScore = m_GameLogic.GetNowScore(wChairID);
	//Print("wChairID:"<<wChairID);

	if (m_bulletList[wChairID].size() == 0)
	{
		Warnning("撒网与发射数不匹配 wChairID:"<<(wChairID));
		if (m_GameLogic.MaxError(wChairID))
		{
			Warnning("错误次数过多! wChairID:"<<wChairID);
			m_GameLogic.ClearError(wChairID);
			return false;
		}
		m_GameLogic.AddError(wChairID);
		return true;
	}
	int bulletCost = m_bulletList[wChairID].front();
	m_bulletList[wChairID].pop_front();

	int nFishInNetCount = pCastNet->cbCount;

	for (int i=0; i<nFishInNetCount; i++)
	{
		wFishID = pCastNet->FishNetIDs[i].wID;
		wFishType = pCastNet->FishNetIDs[i].cbType;
		if (IsFishInValid(wFishID, wFishType))
		{
			Print("无效鱼 id:"<<(wFishID)<<" type:"<<(wFishType)<<
				" m_FishObjects[wFishID].wID:"<<(GetFishID(wFishID))<<" m_FishObjects[wFishID].wType:"<<(GetFishType(wFishID)));
			
			m_GameLogic.AddLostScore(wChairID, bulletCost);
			return true;
		}
	}

	int ratioSum = 0;
	for (int i=0; i<nFishInNetCount; i++)
	{
		ratioSum += m_GameLogic.GetRatioByTpye(pCastNet->FishNetIDs[i].cbType);
	}
	nSucessCount = nFishInNetCount;
	if (nSucessCount >= MAX_FISH_IN_NET)     	
	{
		Warnning("客户端发送数据异常 nSucessCount:"<<nSucessCount);
		return true;
	}

	bool killFish = m_GameLogic.FishGoldByStyle(wChairID, bulletCost, ratioSum, true);

	if(!killFish)
	{
		int willgetscore = ratioSum*bulletCost;
		if (willgetscore <= (int)m_GameLogic.GetLostScore(wChairID))
		{
			m_GameLogic.AddLostScore(wChairID, -willgetscore);
			m_GameLogic.AddScore(wChairID,willgetscore);
			killFish = true;
		}
		else
		{
			return true;
		}
	}
	ASSERT(m_GameLogic.GetNowScore(wChairID)!=lastScore);

	CMD_S_Cast_Net_Success_dte castNetSuccess;
	castNetSuccess.cbChairID = (BYTE)wChairID;

	for (int i=0; i<nFishInNetCount; i++)
	{
		wFishID = pCastNet->FishNetIDs[i].wID;
		wFishType = pCastNet->FishNetIDs[i].cbType;

		castNetSuccess.FishNetIDs[i].wID = wFishID;
		castNetSuccess.FishNetIDs[i].cbType = (BYTE)wFishType;

		switch(wFishType)
		{	
		//case 25://水晶
		//	//m_GameLogic.AddEnergy(wChairID);
		//	break;
		case 23:
			//Print("定屏炸弹爆炸 wFishType:"<<wFishType);
			m_normalFishTime += LOCK_SCREEN_TIME_DELAY;
			m_specialFishTime += LOCK_SCREEN_TIME_DELAY;
			break;
		//case 24:
		//	//Print("范围炸弹爆炸 wFishType:"<<wFishType);
		//	break;
		}

		if (wFishType <= 20)
		{
			if (m_GameLogic.GetCannonType(wChairID)==0)
			{
				if (rand()%1000 <= 5)
				{
					m_GameLogic.SetCannonTypeByServer(wChairID, 1, true);
					CMD_S_Get_Energy_Gun getGun;
					getGun.cbChairID = (BYTE)wChairID;
					getGun.target.cbType = (BYTE)wFishType;
					getGun.target.wID = wFishID;
					SendTableData(INVALID_CHAIR, SUB_S_SEND_GET_ENERGY_GUN, &getGun, sizeof(CMD_S_Get_Energy_Gun));
				}
			}
		}

		if (m_tableState == eGroup)//收集阵型中被打死的鱼
		{
			if(m_groupToSendWhenUserIn.wCount < MAX_IN_GROUP-1)
			{
				m_groupToSendWhenUserIn.FishNetIDs[m_groupToSendWhenUserIn.wCount].wID = wFishID;
				m_groupToSendWhenUserIn.FishNetIDs[m_groupToSendWhenUserIn.wCount].cbType = (BYTE)wFishType;
				m_groupToSendWhenUserIn.wCount++;
			}
		}

		KillFish(wFishID);
	}
	if (nSucessCount == 0 )return true;

	castNetSuccess.dwPlayerScore = m_GameLogic.GetNowScore(wChairID);
	DWORD addScore = castNetSuccess.dwPlayerScore-lastScore;
	castNetSuccess.dwPlayerAddScore = addScore;
	castNetSuccess.cbCount = nSucessCount;

	CheckAndSendRewordNotice(wChairID, pIServerUserItem, addScore);
	return SendTableData(INVALID_CHAIR, SUB_S_CAST_NET_SUCCESS, &castNetSuccess, (WORD)(sizeof(CMD_S_Cast_Net_Success_dte)-(MAX_FISH_IN_CUCESS-nSucessCount)*sizeof(CMD_C_Fish_Net_Object)));
}

//爆炸成功使用撒网成功结果
bool CTableFrameSink::OnHitScreenBomb(IServerUserItem * pIServerUserItem, CMD_C_Hit_Screen_Bomb *pBomb)
{
	WORD wFishID;
	WORD wFishType;
	int nSucessCount = 0;
	WORD wChairID = pIServerUserItem->GetChairID();
	int lastScore = m_GameLogic.GetNowScore(wChairID);

	//Print("wChairID:"<<wChairID<<" m_maxFishID:"<<m_maxFishID);

	if (m_bulletList[wChairID].size() == 0)
	{
		Warnning("爆炸与发射数不匹配 wChairID:"<<(wChairID));
		return true;
	}

	int bulletCost = m_bulletList[wChairID].front();
	m_bulletList[wChairID].pop_front();

	wFishID = pBomb->hitFish.wID;
	wFishType = pBomb->hitFish.cbType;
	if(wFishType!=SCREEN_BOMB_ID)return true;
	if (IsFishInValid(wFishID, wFishType))
	{
		m_GameLogic.AddLostScore(wChairID, bulletCost);
		return true;
	}

	//判断炸弹是否死亡
	bool killFish = m_GameLogic.FishGoldByStyle(wChairID, bulletCost, pBomb->wRatio, true);

	if(!killFish)return true;

	CMD_S_Cast_Net_Success_dte castNetSuccess;
	castNetSuccess.cbChairID = (BYTE)wChairID;
	//先添加炸弹到成功列表
	castNetSuccess.FishNetIDs[nSucessCount].wID = GetFishID(wFishID);
	castNetSuccess.FishNetIDs[nSucessCount].cbType = (BYTE)GetFishType(wFishID);
	nSucessCount++;
	KillFish(wFishID);

	for (int i=0; i<=m_maxFishID; i++)
	{
		if (nSucessCount >= MAX_FISH_IN_CUCESS)     	
		{
			Warnning("超过最大撒网数 nSucessCount:"<<nSucessCount);
			break;
		}

		if (IsFishNotKillByBomb(i) == false && IsFishAlive(i))//可以炸死
		{
			castNetSuccess.FishNetIDs[nSucessCount].wID = GetFishID(i);
			castNetSuccess.FishNetIDs[nSucessCount].cbType = (BYTE)GetFishType(i);
			nSucessCount++;
			KillFish(i);
		}
	}

	castNetSuccess.dwPlayerScore = m_GameLogic.GetNowScore(wChairID);
	DWORD addScore = castNetSuccess.dwPlayerScore-lastScore;
	castNetSuccess.dwPlayerAddScore = addScore;
	castNetSuccess.cbCount = nSucessCount;
	m_normalFishTime = getTime();
	CheckAndSendRewordNotice(wChairID, pIServerUserItem, addScore);
	//Print("castNetSuccess.cbCount:"<<castNetSuccess.cbCount);
	return SendTableData(INVALID_CHAIR, SUB_S_CAST_NET_SUCCESS, &castNetSuccess, (WORD)(sizeof(CMD_S_Cast_Net_Success_dte)-(MAX_FISH_IN_CUCESS-nSucessCount)*sizeof(CMD_C_Fish_Net_Object)));
}

bool CTableFrameSink::OnHitBoat( IServerUserItem * pIServerUserItem, CMD_C_HitBoat* pBoat )
{
	WORD wFishID;
	WORD wFishType;
	int nSucessCount = 0;
	WORD wChairID = pIServerUserItem->GetChairID();
	int lastScore = m_GameLogic.GetNowScore(wChairID);

	//Print("wChairID:"<<wChairID<<" m_maxFishID:"<<m_maxFishID);

	if (m_bulletList[wChairID].size() == 0)
	{
		Warnning("爆炸与发射数不匹配 wChairID:"<<(wChairID));
		return true;
	}

	int bulletCost = m_bulletList[wChairID].front();
	m_bulletList[wChairID].pop_front();

	wFishID = pBoat->hitFish.wID;
	wFishType = pBoat->hitFish.cbType;
	if(wFishType!=BOAT_ID)return true;
	if (IsFishInValid(wFishID, wFishType))
	{
		Print("无效鱼 id:"<<(wFishID)<<" type:"<<(wFishType)<<
			" m_FishObjects[wFishID].wID:"<<(GetFishID(wFishID))<<" m_FishObjects[wFishID].wType:"<<(GetFishType(wFishID)));
		
		m_GameLogic.AddLostScore(wChairID, bulletCost);
		return true;
	}

	//判断炸弹是否死亡
	bool killFish = m_GameLogic.FishGoldByStyle(wChairID, bulletCost, pBoat->wRatio, true);
	if(!killFish)return true;

	CMD_S_Cast_Net_Success_dte castNetSuccess;
	castNetSuccess.cbChairID = (BYTE)wChairID;
	//先添加炸弹到成功列表
	castNetSuccess.FishNetIDs[nSucessCount].wID = GetFishID(wFishID);
	castNetSuccess.FishNetIDs[nSucessCount].cbType = (BYTE)GetFishType(wFishID);
	nSucessCount++;
	KillFish(wFishID);

	castNetSuccess.dwPlayerScore = m_GameLogic.GetNowScore(wChairID);
	DWORD addScore = castNetSuccess.dwPlayerScore-lastScore;
	castNetSuccess.dwPlayerAddScore = addScore;
	castNetSuccess.cbCount = nSucessCount;
	CheckAndSendRewordNotice(wChairID, pIServerUserItem, addScore);
	return SendTableData(INVALID_CHAIR, SUB_S_CAST_NET_SUCCESS, &castNetSuccess, (WORD)(sizeof(CMD_S_Cast_Net_Success_dte)-(MAX_FISH_IN_CUCESS-nSucessCount)*sizeof(CMD_C_Fish_Net_Object)));
}

bool CTableFrameSink::OnHitTypeBomb( IServerUserItem * pIServerUserItem, CMD_C_TpyeBomb_dte *pBomb )
{
	WORD wFishID;
	WORD wFishType;
	WORD wChairID = pIServerUserItem->GetChairID();
	int lastScore = m_GameLogic.GetNowScore(wChairID);
	//Print("wChairID:"<<wChairID);

	if (m_bulletList[wChairID].size()==0)
	{
		Warnning("发射数不匹配");
		return true;
	}

	int bulletCost = m_bulletList[wChairID].front();
	m_bulletList[wChairID].pop_front();
	
	int nFishWithSameTpyeCount = pBomb->cbCount;
	if (nFishWithSameTpyeCount >= MAX_FISH_BY_BOMB)
	{
		Warnning("nFishInNetCount >= MAX_FISH_BY_BOMB nFishInNetCount:"<<nFishWithSameTpyeCount);
		return false;
	}

	WORD wOneType = pBomb->hitFish.cbType;

	//检测是否同类鱼
	for (int i=0; i<nFishWithSameTpyeCount; i++)
	{
		wFishID = pBomb->FishNetIDs[i].wID;
		wFishType = pBomb->FishNetIDs[i].cbType;
		if (wOneType != wFishType)
		{
			//LogError("不是同类鱼 wFishType:"<<(wFishType));
			//m_logging.Log("不是同类鱼 wFishType:%d uid:%d",wFishType, pIServerUserItem->GetUserID());
			return false;
		}
		if (IsFishInValid(wFishID, wFishType))
		{
			Print("无效鱼 id:"<<(wFishID)<<" type:"<<(wFishType)<<
				" m_FishObjects[wFishID].wID:"<<(GetFishID(wFishID))<<" m_FishObjects[wFishID].wType:"<<(GetFishType(wFishID)));
			
			m_GameLogic.AddLostScore(wChairID, bulletCost);
			return true;
		}
	}

	wFishID = pBomb->hitFish.wID;
	wFishType = pBomb->hitFish.cbType;
	if (IsFishInValid(wFishID, wFishType))
	{
		Print("hitFish 无效鱼 id:"<<(wFishID)<<" type:"<<(wFishType)<<
			" m_FishObjects[wFishID].wID:"<<(GetFishID(wFishID))<<" m_FishObjects[wFishID].wType:"<<(GetFishType(wFishID)));
		
		m_GameLogic.AddLostScore(wChairID, bulletCost);
		return true;
	}

	int ratioSum = m_GameLogic.GetRatioByTpye(pBomb->hitFish.cbType);
	for (int i=0; i<nFishWithSameTpyeCount; i++)
	{
		ratioSum += m_GameLogic.GetRatioByTpye(pBomb->FishNetIDs[i].cbType);
	}

	bool killFish = m_GameLogic.FishGoldByStyle(wChairID, bulletCost, ratioSum, true);
	if(!killFish)return true;

	KillFish(pBomb->hitFish.wID);

	CMD_S_TypeBomb_Success_dte typeBombSuccess;
	typeBombSuccess.cbChairID = (BYTE)wChairID;
	typeBombSuccess.hitFish = pBomb->hitFish;

	for (int i=0; i<nFishWithSameTpyeCount; i++)
	{
		wFishID = pBomb->FishNetIDs[i].wID;
		wFishType = pBomb->FishNetIDs[i].cbType;

		typeBombSuccess.FishNetIDs[i].wID = wFishID;
		typeBombSuccess.FishNetIDs[i].cbType = (BYTE)wFishType;

		KillFish(wFishID);
	}

	typeBombSuccess.dwPlayerScore = m_GameLogic.GetNowScore(wChairID);
	DWORD addScore = typeBombSuccess.dwPlayerScore-lastScore;
	typeBombSuccess.dwPlayerAddScore = addScore;
	typeBombSuccess.cbCount = nFishWithSameTpyeCount;
	CheckAndSendRewordNotice(wChairID, pIServerUserItem, addScore);
	return SendTableData(INVALID_CHAIR, SUB_S_TPYE_BOBM_SUCCESS, &typeBombSuccess, (WORD)(sizeof(CMD_S_TypeBomb_Success_dte)-(MAX_FISH_BY_BOMB-nFishWithSameTpyeCount)*sizeof(CMD_C_Fish_Net_Object)));
}


bool CTableFrameSink::OnHitTeam( IServerUserItem * pIServerUserItem, CMD_C_HitTeam_dte* pTeam )
{
	WORD wFishID;
	WORD wFishType;
	WORD wChairID = pIServerUserItem->GetChairID();
	int lastScore = m_GameLogic.GetNowScore(wChairID);
	//Print("wChairID:"<<(int)wChairID);

	if (m_bulletList[wChairID].size()==0)
	{
		//LogError("发射数不匹配 uid:"<<m_GameLogic.GetUID(wChairID));
		//m_logging.Log("发射数不匹配 uid:%d", pIServerUserItem->GetUserID());
		return true;
	}
	
	int bulletCost = m_bulletList[wChairID].front();
	m_bulletList[wChairID].pop_front();

	int nFishInTeamCount = pTeam->cbCount;

	bool checkOk = (nFishInTeamCount<=6);
	if(!checkOk)
	{
		//LogError("小队数量有误! nFishInTeamCount:"<<(nFishInTeamCount)<<" uid:"<<m_GameLogic.GetUID(wChairID));
		//m_logging.Log("小队数量有误! nFishInTeamCount:%d uid:%d", nFishInTeamCount, pIServerUserItem->GetUserID());
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//check
	wFishID = pTeam->hitFish.wID;
	wFishType = pTeam->hitFish.cbType;
	if (IsFishInValid(wFishID, wFishType))
	{
		Print("hitFish 无效鱼 id:"<<(wFishID)<<" type:"<<(wFishType)<<
			" m_FishObjects[wFishID].wID:"<<(GetFishID(wFishID))<<" m_FishObjects[wFishID].wType:"<<(GetFishType(wFishID)));
		
		m_GameLogic.AddLostScore(wChairID, bulletCost);
		return true;
	}
	//check
	//////////////////////////////////////////////////////////////////////////

	int ratioSum = m_GameLogic.GetRatioByTpye(pTeam->hitFish.cbType);
	for (int i=0; i<nFishInTeamCount; i++)
	{
		ratioSum += m_GameLogic.GetRatioByTpye(pTeam->FishNetIDs[i].cbType);
	}

	bool killFish = m_GameLogic.FishGoldByStyle(wChairID, bulletCost, ratioSum, true);
	if(!killFish)return true;
	
	KillFish(pTeam->hitFish.wID);
		
	CMD_S_HitTeam_Success_dte teamSuccess;
	teamSuccess.cbChairID = (BYTE)wChairID;
	teamSuccess.hitFish = pTeam->hitFish;
	teamSuccess.cbCount = nFishInTeamCount;

	for (int i=0; i<nFishInTeamCount; i++)
	{
		wFishID = pTeam->FishNetIDs[i].wID;
		wFishType = pTeam->FishNetIDs[i].cbType;

		teamSuccess.FishNetIDs[i].wID = pTeam->FishNetIDs[i].wID;
		teamSuccess.FishNetIDs[i].cbType = pTeam->FishNetIDs[i].cbType;

		KillFish(wFishID);
	}

	teamSuccess.dwPlayerScore = m_GameLogic.GetNowScore(wChairID);
	DWORD addScore = teamSuccess.dwPlayerScore-lastScore;
	teamSuccess.dwPlayerAddScore = addScore;
	CheckAndSendRewordNotice(wChairID, pIServerUserItem, addScore);
	return SendTableData(INVALID_CHAIR, SUB_S_HIT_TEAM_SUCCESS, &teamSuccess, (WORD)(sizeof(CMD_S_HitTeam_Success_dte)-(MAX_FISH_IN_TEAM-teamSuccess.cbCount)*sizeof(CMD_C_Fish_Net_Object)));
}

bool CTableFrameSink::OnChangeCannon(IServerUserItem * pIServerUserItem, CMD_C_Change_Cannon *pChangeCannon)
{
	WORD wChairID = pIServerUserItem->GetChairID();

	ASSERT(pChangeCannon->cbStyle>=0 && pChangeCannon->cbStyle<MAX_CANNON_STYLE);
	if (pChangeCannon->cbStyle<0 || pChangeCannon->cbStyle>=MAX_CANNON_STYLE)
	{
		//LogError ("非法改变炮筒 超过炮筒类型上限 uid:"<<m_GameLogic.GetUID(wChairID));
		//m_logging.Log("非法改变炮筒 超过炮筒类型上限 uid:%d",pIServerUserItem->GetUserID());
		return false;
	}

	bool b = m_GameLogic.SetBulletCost(wChairID, pChangeCannon->wCost);
	if(!b)return false;
	m_GameLogic.SetCannonTpye(wChairID, pChangeCannon->cbStyle);

	CMD_S_Change_Cannon changeCannon;

	changeCannon.cbChairID = (BYTE)pIServerUserItem->GetChairID();
	changeCannon.cbStyle = m_GameLogic.GetCannonType(wChairID);
	changeCannon.wCost = pChangeCannon->wCost;

	return SendTableData(INVALID_CHAIR, SUB_S_CHANGE_CANNON, &changeCannon, sizeof(CMD_S_Change_Cannon));

}

bool CTableFrameSink::OnAccount(IServerUserItem * pIServerUserItem, CMD_C_Account *pAccount)
{
	
	WORD wChairID = pIServerUserItem->GetChairID();
	int nowScore = m_GameLogic.GetNowScore(wChairID);

	Print("wChairID:"<<wChairID);

	if (nowScore == 0)return true;
	if(nowScore<=0)
	{
		//LogError("玩家分数异常 nowScore:"<<nowScore<<" uid:"<<m_GameLogic.GetUID(wChairID));
		//m_logging.Log("玩家分数异常 nowScore:%d uid:%d",nowScore,pIServerUserItem->GetUserID());
		return false;
	}

	m_GameLogic.AddlScore(wChairID, nowScore*m_pGameServiceOption->lCellScore);
	m_pITableFrame->WriteUserScore(pIServerUserItem,
		m_GameLogic.GetAccountScore(wChairID),
		0,enScoreKind_Draw);

	bool ret = m_GameLogic.AccountChair(pIServerUserItem->GetUserID(), wChairID, sql);
	if (!ret)
	{
		//LogError("玩家数据写入SQLite失败 uid:"<<pIServerUserItem->GetUserID());
		//m_logging.Log("玩家数据写入SQLite失败 uid:%d",pIServerUserItem->GetUserID());
	}

	m_GameLogic.SetlScore(wChairID, pIServerUserItem->GetUserScore()->lScore);

	CMD_S_Account account;
	account.cbChairID = (BYTE)wChairID;
	account.dwPlayerScore = (DWORD)nowScore;
	account.lCellScore = m_pGameServiceOption->lCellScore;

	return SendTableData(INVALID_CHAIR, SUB_S_ACCOUNT, &account, sizeof(CMD_S_Account));
}

bool CTableFrameSink::SendNormalFish(const TypeList& list)
{
	//return true;
	int number = (int)list.size();
	if (number >= MAX_FISH_SEND)
	{
		LogError ("m_typeList.size() >= MAX_FISH_SEND");
		number = MAX_FISH_SEND-1;
	}
	CMD_S_Fish_Path sendGroupPath;
	sendGroupPath.cbSeed = rand();
	sendGroupPath.cbCount = number;
	sendGroupPath.cbType = FISH_PATH_LINE;

	int nFreeFishID;
	
	float time = getTime();

	for (int i=0; i<sendGroupPath.cbCount; i++)
	{
		int fishType = list[i];
		nFreeFishID = GetFreeFishID();
		if (nFreeFishID>=MAX_FISH_OBJECT)
		{
			LogError ("GetFreeFishID");
			return false;
		}

		sendGroupPath.FishNetObject[i].wID = nFreeFishID;
		sendGroupPath.FishNetObject[i].cbType = fishType;

		AliveFish(nFreeFishID , fishType , time);

		if (fishType >= 22)
		{
			SetFishNotKillByBomb(nFreeFishID, true);
		}
		else
		{
			SetFishNotKillByBomb(nFreeFishID, false);
		}
		//Print("nFreeFishID:"<<nFreeFishID<<" fishType:"<<fishType);

	}
	//Print("SendEnd");
	return SendTableData(INVALID_CHAIR, SUB_S_SEND_FISH_PATH, &sendGroupPath, sizeof(sendGroupPath)-(MAX_FISH_SEND-sendGroupPath.cbCount)*sizeof(CMD_C_Fish_Net_Object));

}

bool CTableFrameSink::SendSpecialFish(int type, const TypeList& list)
{
	//if(type != FISH_PATH_SP_HAS_ID)
	//return true;
	ASSERT(list.size() < MAX_FISH_SEND);
	if (list.size() >= MAX_FISH_SEND)
	{
		LogError ("list.size() >= MAX_FISH_SEND");
		return false;
	}
	CMD_S_Fish_Path sendGroupPath;
	sendGroupPath.cbSeed = rand();
	sendGroupPath.cbCount = (BYTE)list.size();
	sendGroupPath.cbType = type;
	

	int nFreeFishID;

	float time = getTime();

	for (int i=0; i<sendGroupPath.cbCount; i++)
	{
		int fishType = list[i];
		ASSERT(fishType!=0);
		nFreeFishID = GetFreeFishID();
		if (nFreeFishID>=MAX_FISH_OBJECT)
		{
			LogError ("GetFreeFishID");
			return false;
		}

		sendGroupPath.FishNetObject[i].wID = nFreeFishID;
		sendGroupPath.FishNetObject[i].cbType = fishType;

		AliveFish(nFreeFishID , fishType , time);
		
		switch(type)
		{
		case FISH_PATH_SP_TYPE_BOMB:
		case FISH_PATH_SP_SPTTEAM:
		case FISH_PATH_SP_HAS_ID:
			SetFishNotKillByBomb(nFreeFishID , true);
			break;
		default:
			SetFishNotKillByBomb(nFreeFishID , false);
			break;
		}
		//Print("nFreeFishID:"<<nFreeFishID<<" nFishType:"<<nFishTypeID<<" m_FishObjects[nFreeFishID].wID:"<<m_FishObjects[nFreeFishID].wID);

	}
	//Print("Send End");
	return SendTableData(INVALID_CHAIR, SUB_S_SEND_FISH_PATH, &sendGroupPath, sizeof(sendGroupPath)-(MAX_FISH_SEND-sendGroupPath.cbCount)*sizeof(CMD_C_Fish_Net_Object));

}

int CTableFrameSink::GetFreeFishID()
{
	for (int i=0; i<MAX_FISH_OBJECT; i++)
	{
		if (IsFishAlive(i)==false)
		{
			m_maxFishID = max(m_maxFishID, i);
			return i;
		}
	}
	LogError ("GetFreeFishID error");
	return MAX_FISH_OBJECT;
}

void CTableFrameSink::CheckFishDestroy()
{
	float time = getTime();

	for (int i=0; i<MAX_FISH_OBJECT; i++)
	{
		if (IsFishAlive(i))
		{
			if ((time-GetFishTime(i)) >= FISH_DESTROY_TIME)
			{
				KillFish(i);
			}
		}
	}
	ASSERT(m_nFishCount>=0);
}

void CTableFrameSink::DesroyAllFish()
{
	for (int i=0; i<MAX_FISH_OBJECT; i++)
	{
		if (IsFishAlive(i))
		{
			KillFish(i);
		}
	}
	m_maxFishID = 0;
	ASSERT(m_nFishCount>=0);
	if (m_nFishCount != 0)
	{
		Warnning("m_nFishCount有误! m_nFishCount:"<<m_nFishCount);
	}
#ifndef _DEBUG
	m_nFishCount = max(0, m_nFishCount);
#endif
}

void CTableFrameSink::OnTimerTick()
{
	//if(WaitForSingleObject(m_TableEvent, 100)==WAIT_TIMEOUT)
	{
		//LogError("WaitForSingleObject(m_TableEvent, 100)==WAIT_TIMEOUT");
	}
	//CAutoLock lock(&m_TimerEvent);

	m_GameLogic.Tick();
if(m_tableState!=eGroup)//阵型不杀鱼
	CheckFishDestroy();

	float time = getTime();

	switch(m_tableState)
	{
	case eNormal:
		if (time - m_sceneStartTime > CHANGE_SCENE_TIME_DELAY)
		{
			OnChangeScene();//切换场景并发送消息
			m_tableState = eWaitChangeScene;
			m_waitChangSceneTime = time;
		}
		if (time - m_normalFishTime > NORMAL_FISH_TIME_DELAY)
		{
			OnNormalFish(time - m_normalFishTime);
			m_normalFishTime = time;
		}
		if (time - m_specialFishTime > SPECAIL_FISH_TIME_DELAY)
		{
			CreateSpecialFish(time - m_specialFishTime);
			m_specialFishTime = time;
		}
		break;
	case eWaitChangeScene:
		if (time - m_waitChangSceneTime > CHANGE_SCENE_TIME_WAIT)
		{//客户端场景切换结束
			OnGroupStart();//发送阵型
			m_tableState = eGroup;
			m_groupTime = time;
			Print("阵型开始");
		}
		break;
	case eGroup:
		if (time - m_groupTime > GROUP_FISH_TIME_DELAY)
		{
			m_tableState = eNormal;
			m_normalFishTime = time;
			m_specialFishTime = time;
			m_sceneStartTime = time-NORMAL_FISH_TIME_DELAY-2;
			Print("阵型结束");
			memset(&m_groupToSendWhenUserIn, 0, sizeof(m_groupToSendWhenUserIn));
		}
		else
		{
			m_groupToSendWhenUserIn.fPassTime = time - m_groupTime;
		}
		break;
	}
}

void CTableFrameSink::OnChangeScene()
{
	m_cbScene ++;
	m_cbScene %= MAX_SCENE;
	Print("切换场景，ID:"<<(int)m_cbScene);

	CMD_S_Change_Scene ChangeScene;

	ChangeScene.cbScene = m_cbScene;
	SendTableData(INVALID_CHAIR, SUB_S_CHANGE_SCENE, &ChangeScene, sizeof(CMD_S_Change_Scene));
}

void CTableFrameSink::OnNormalFish(float delta)
{
	CreateNormalFish(delta);
	bool ret = SendNormalFish(m_typeList);
	if (!ret)
	{
		LogError("SendNormalFish");
	}
}

void CTableFrameSink::CreateSpecialFish(float delta)
{
	//delta *= 2;

	m_spHasIDList.clear();
	m_teamTypeList.clear();
	m_spTeamTypeList.clear();
	m_typeBombList.clear();
	m_waterPlantList.clear();

	for (int i=0;i<MAX_SP_FISH_TYPE;i++)
	{
		Fish_Time& fishTime = m_spFishTimeTable[i];
		fishTime.leftTime += delta;
		int fishNumber = 0;
		while(fishTime.leftTime >= fishTime.deltaTime )
		{
			fishTime.leftTime -= fishTime.deltaTime;
			fishNumber++;
		}
		//Print("fishTime.typeId:"<<fishTime.typeId<<" fishNumber:"<<fishNumber);
		int type = 1;
		int teamNumber = 3;
		switch(fishTime.typeId)
		{
		//case 21://水草
		case TEAM_TYPE_FISH:
		case SP_TEAM_TYPE_FISH:
		case TYPE_BOMB_FISH:
			type = rand()%9+1;
			break;
		}
		for (int j=0;j<fishNumber;j++)
		{
			switch(fishTime.typeId)
			{
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
				//if(fishTime.typeId==25)
				m_spHasIDList.push_back(fishTime.typeId);
				break;
			case TEAM_TYPE_FISH:
				m_teamTypeList.assign(m_GameLogic.RandomTeamNumber(), type);
				break;
			case SP_TEAM_TYPE_FISH:
				m_spTeamTypeList.assign(m_GameLogic.RandomTeamNumber(), type);
				break;
			case TYPE_BOMB_FISH:
				m_typeBombList.push_back(type);
				break;
			case WANTER_PLANT://水草
				m_waterPlantList.push_back(MTRandomT(1,12));
				break;
			default:
				LogError("Error Type!");
				break;
			}
		}
	}

	if (m_spHasIDList.size()>0)
	{
		bool ret =SendSpecialFish(FISH_PATH_SP_HAS_ID, m_spHasIDList);
		if (!ret)
		{
			LogError("SendSpecialFish FISH_PATH_WATER_PLANTS");
		}
	}
	if (m_teamTypeList.size()>0)
	{
		bool ret =SendSpecialFish(FISH_PATH_SP_TEAM, m_teamTypeList);
		if (!ret)
		{
			LogError("SendSpecialFish FISH_PATH_SP_TEAM");
		}
	}
	if (m_spTeamTypeList.size()>0)
	{
		bool ret =SendSpecialFish(FISH_PATH_SP_SPTTEAM, m_spTeamTypeList);
		if (!ret)
		{
			LogError("SendSpecialFish FISH_PATH_SP_SPTTEAM");
		}
	}
	if (m_typeBombList.size()>0)
	{
		bool ret =SendSpecialFish(FISH_PATH_SP_TYPE_BOMB, m_typeBombList);
		if (!ret)
		{
			LogError("SendSpecialFish FISH_PATH_SP_TYPE_BOMB");
		}
	}
	if (m_waterPlantList.size()>0)
	{
		bool ret =SendSpecialFish(FISH_PATH_WATER_PLANT, m_waterPlantList);
		if (!ret)
		{
			LogError("SendSpecialFish FISH_PATH_WATER_PLANT");
		}
	}
	
}

void CTableFrameSink::CreateNormalFish(float delta)
{
	//return;
	m_typeList.clear();
	for (int i=0;i<MAX_NORMAL_FISH_TYPE;i++)
	{
		Fish_Time& fishTime = m_normalFishTimeTable[i];
		fishTime.leftTime += delta;
		while(fishTime.leftTime>=fishTime.deltaTime)
		{
			fishTime.leftTime -= fishTime.deltaTime;
			if(fishTime.number<=0)continue;
			if(m_typeList.size()+fishTime.number<MAX_FISH_SEND)
				m_typeList.insert(m_typeList.end(), fishTime.number, fishTime.typeId);
		}
	}
}

bool CTableFrameSink::SendGroup()
{
	BYTE groupID = MTRandomT(1,6);
	Print("发送阵型,ID:"<<(int)groupID);

	switch(groupID)
	{
	case 1:
		InitGroupFish1();
		break;
	case 2:
		InitGroupFish2();
		break;
	case 3:
		InitGroupFish3();
		break;
	case 4:
		InitGroupFish4();
		break;
	case 5:
		InitGroupFish5();
		break;
	case 6:
		InitGroupFish6();
		break;
	case 7:
		InitGroupFish7();
		break;
	case 8:
		InitGroupFish8();
		break;
	case 9:
		InitGroupFish9();
		break;
	case 10:
	case 11:
		InitGroupFish10_11();
		break;
	default:
		LogError("无效阵型");
		return false;
	}

	CMD_S_Fish_Group group;
	group.cbSeed = (BYTE)rand();
	group.cbType = groupID;
	group.fPassTime = 0;
	group.wCount = 0;

	m_groupToSendWhenUserIn = group;

	DWORD sendSize = sizeof(CMD_S_Fish_Group)-(MAX_IN_GROUP-group.wCount)*sizeof(CMD_C_Fish_Net_Object);
	return SendTableData(INVALID_CHAIR, SUB_S_SEND_FISH_PATH_GROUP, &group, (WORD)sendSize);
}

void CTableFrameSink::OnGroupStart()
{
	//清空鱼池
	DesroyAllFish();
	bool ret = SendGroup();//发送阵型消息
	if (!ret)
	{
		LogError("SendGroup error");
	}
}

//////////////////////////////////////////////////////////////////////////

void CTableFrameSink::InitGroupFish1()
{
	ActiveGroupFish(280);
	SetGroupFish(0,		99,		1);
	SetGroupFish(100,	199,	2);
	SetGroupFish(200,	219,	14);
	SetGroupFish(220,	239,	15);
	SetGroupFish(240,	259,	16);
	SetGroupFish(260,	279,	17);
}

void CTableFrameSink::InitGroupFish2()
{
	ActiveGroupFish(40);
	SetGroupFish(0,		9,		17);
	SetGroupFish(10,	19,		18);
	SetGroupFish(20,	29,		19);
	SetGroupFish(30,	39,		20);
}

void CTableFrameSink::InitGroupFish3()
{
	ActiveGroupFish(280);
	SetGroupFish(0,		119,	1);
	SetGroupFish(120,	239,	2);
	SetGroupFish(240,	249,	17);
	SetGroupFish(250,	259,	18);
	SetGroupFish(260,	269,	19);
	SetGroupFish(270,	279,	20);
}

void CTableFrameSink::InitGroupFish4()
{
	ActiveGroupFish(910);
	SetGroupFish(0,		299,	1);
	SetGroupFish(300,	599,	2);
	SetGroupFish(600,	899,	3);
	SetGroupFish(900,	909,	4);
}

void CTableFrameSink::InitGroupFish5()
{
	ActiveGroupFish(202);
	SetGroupFish(0,		39,		1);
	SetGroupFish(40,	79,		2);
	SetGroupFish(80,	109,	3);
	SetGroupFish(110,	129,	4);
	SetGroupFish(130,	159,	5);
	SetGroupFish(160,	189,	6);
	SetGroupFish(190,	199,	7);
	SetGroupFish(200,	200,	19);
	SetGroupFish(201,	201,	20);
}

void CTableFrameSink::InitGroupFish6()
{
	ActiveGroupFish(270);
	SetGroupFish(0,		119,	1);
	SetGroupFish(120,	149,	2);
	SetGroupFish(150,	179,	3);
	SetGroupFish(180,	209,	4);
	SetGroupFish(210,	239,	5);
	SetGroupFish(240,	269,	6);
}

void CTableFrameSink::InitGroupFish7()
{
	ActiveGroupFish(285);
	SetGroupFish(0,		119,	1);
	SetGroupFish(120,	219,	2);
	SetGroupFish(220,	279,	6);
	SetGroupFish(280,	284,	20);
}

void CTableFrameSink::InitGroupFish8()
{
	ActiveGroupFish(180);
	SetGroupFish(0,		119,	1);
	SetGroupFish(120,	159,	2);
	SetGroupFish(160,	219,	6);
	SetGroupFish(220,	229,	20);
}

void CTableFrameSink::InitGroupFish9()
{
	ActiveGroupFish(315);
	SetGroupFish(0,		149,	1);
	SetGroupFish(150,	229,	2);
	SetGroupFish(230,	309,	6);
	SetGroupFish(310,	314,	19);
}

void CTableFrameSink::InitGroupFish10_11()
{
	ActiveGroupFish(150);
	SetGroupFish(0,		59,		1);
	SetGroupFish(60,	119,	2);
	SetGroupFish(120,	129,	3);
	SetGroupFish(130,	134,	6);
	SetGroupFish(135,	139,	8);
	SetGroupFish(140,	144,	19);
	SetGroupFish(145,	149,	20);
}


//////////////////////////////////////////////////////////////////////////

void CTableFrameSink::OneUserLeave(int chairId)
{
	if (m_GameLogic.HasNoneAIUser()==false)
	{
		m_tickTimerStop = true;
		m_sceneStartTime = 0;
		DesroyAllFish();
		//Print("桌子已空");
	}
	

	if (m_GameLogic.GetTotalUserCount()==0)
	{
		KillTimer();
		m_pITableFrame->ConcludeGame();
		m_pITableFrame->SetGameStatus(GS_FREE);
	}
}

bool CTableFrameSink::SendGroupWhenUserIn( WORD wChairID )
{
	if (m_tableState == eGroup)
	{
		Print("wChairID:"<<wChairID);
		ASSERT(m_groupToSendWhenUserIn.wCount>=0);
		ASSERT(m_groupToSendWhenUserIn.wCount<MAX_IN_GROUP);

		m_groupToSendWhenUserIn.wCount = min(m_groupToSendWhenUserIn.wCount, MAX_IN_GROUP-1);

		m_groupToSendWhenUserIn.fPassTime = getTime()-m_groupTime;

		return SendTableData(wChairID, SUB_S_SEND_FISH_PATH_GROUP, &m_groupToSendWhenUserIn, 
			sizeof(m_groupToSendWhenUserIn)-(MAX_IN_GROUP-m_groupToSendWhenUserIn.wCount)*sizeof(CMD_C_Fish_Net_Object));
	}
	return true;
}

bool CTableFrameSink::SendRewordNotice( WORD wChairID, int type, DWORD value )
{
	CMD_S_Reword_Notice notice;
	notice.cbChairID = (BYTE)wChairID;
	notice.cbType = (BYTE)type;
	notice.dwValue = (DWORD)value;
	return SendTableData(INVALID_CHAIR, SUB_S_REWORD_NOTIC, &notice, sizeof(CMD_S_Reword_Notice));
}

void CTableFrameSink::WriteUserScore( WORD wChairID, IServerUserItem * pIServerUserItem )
{
	int bulletScore = 0;
	CostList& bulletCostList = m_bulletList[wChairID];
	CostList::iterator it = bulletCostList.begin();
	for (;it!=bulletCostList.end();it++)
	{
		bulletScore += *it;
	}
	bulletCostList.clear();
	bulletScore = max(0,bulletScore);
	m_GameLogic.AddScore(wChairID, bulletScore);
	

	// 剩余子弹
	int nScore = 0;
	if (pIServerUserItem->IsAndroidUser())
	{
		nScore = rand()%1000*110 - 1000;
	}
	else
	{
		nScore = m_GameLogic.GetNowScore(wChairID)*m_pGameServiceOption->lCellScore;
	}


	m_GameLogic.AddlScore(wChairID, nScore*m_pGameServiceOption->lCellScore);
	m_pITableFrame->WriteUserScore(pIServerUserItem,m_GameLogic.GetAccountScore(wChairID),0,enScoreKind_Draw);

	bool ret = m_GameLogic.ClearChair(pIServerUserItem->GetUserID(), wChairID, sql);
	if (!ret)
	{
		//LogError("玩家数据写入失败 uid:"<<pIServerUserItem->GetUserID());
		//m_logging.Log("玩家数据写入SQLite失败 uid:%d",pIServerUserItem->GetUserID());
	}
}

bool CTableFrameSink::SendChangeCannon( WORD wChairID, WORD cost, BYTE cbType )
{
	CMD_S_Change_Cannon changeCannon;
	changeCannon.cbChairID = (BYTE)wChairID;
	changeCannon.cbStyle = cbType;
	changeCannon.wCost = cost;
	return SendTableData(INVALID_CHAIR, SUB_S_CHANGE_CANNON, &changeCannon, sizeof(CMD_S_Change_Cannon));
}

bool CTableFrameSink::SendChangeEnergy( WORD wChairID, BYTE cbEnergy )
{
	CMD_S_Change_Energy energy;
	energy.cbChairID = (BYTE)wChairID;
	energy.cbValue = cbEnergy;
	return SendTableData(INVALID_CHAIR, SUB_S_ENERGY_CHANGE, &energy, sizeof(CMD_S_Change_Energy));
}

void CTableFrameSink::CheckAndSendRewordNotice( WORD wChairID, IServerUserItem * pIServerUserItem, DWORD addScore )
{
	if( script_CheckAndSendRewordNotice)
	{
		//pTableFrameSink,pIServerUserItem,nMaxCost,addScore,szGameName,szRoomName
		 //east.su
		//bool b = CallLuaFunction(m_pLua, script_CheckAndSendRewordNotice,pIServerUserItem,m_GameLogic.GetRoomMaxCost(),addScore,GAME_NAME,m_pGameServiceOption->szGameRoomName);
		
	}

	////if (addScore >= (DWORD)((100*m_GameLogic.GetRoomMaxCost())))
	//DWORD lowScore = (DWORD)((80*m_GameLogic.GetRoomMaxCost()));
	//DWORD midScore = (DWORD)((500*m_GameLogic.GetRoomMaxCost()));
	//DWORD highScore = (DWORD)((990*m_GameLogic.GetRoomMaxCost()));


	//if (addScore >= lowScore) //大于50倍在全房间显示
	//{
	//	SendRewordNotice(wChairID, eBigPrize, addScore);

	//	char szMessage[300] = {0};
	//	_snprintf(szMessage,sizeof(szMessage),"恭喜玩家[ %s ]获得大奖，奖金:%d金币.",pIServerUserItem->GetAccounts(),addScore);
	//	
	//	char szGlobalMessage[200] = {0};
	//	_snprintf(szGlobalMessage,sizeof(szGlobalMessage),"恭喜玩家[ %s ]在[ %s ]的[ %s ]获得大奖，奖金:%d金币.",pIServerUserItem->GetAccounts(),GAME_NAME,m_pGameServiceOption->szGameRoomName,addScore);

	//	/*
	//	std::string s1 = "恭喜玩家 ";
	//	std::string s2 = pIServerUserItem->GetAccounts();
	//	std::string s3 = " 获得大奖，奖金:";
	//	std::string s4 = num2str(addScore);
	//	SendRoomGolobalMessage((s1+s2+s3+s4).c_str());
	//	*/

	//	
	//	if (addScore >= highScore)
	//	{
	//		SendRoomGolobalMessage(szGlobalMessage,3);
	//		SendRoomGolobalMessage(szMessage,1);
	//	}
	//	else if(addScore >= midScore)
	//	{
	//		SendRoomGolobalMessage(szGlobalMessage,2);
	//		SendRoomGolobalMessage(szMessage,1);
	//	}else
	//	{
	//		SendRoomGolobalMessage(szMessage,1);
	//	}
	//	
	//}
}

bool CTableFrameSink::SendTableData(WORD chairId, WORD subId, void* pData, WORD size)
{
	bool b = m_pITableFrame->SendTableData(chairId, subId, pData, size);
	m_pITableFrame->SendLookonData(chairId, subId, pData, size);
	return b;
}

bool CTableFrameSink::SetUserFree( IServerUserItem * pIServerUserItem )
{
	if(pIServerUserItem==NULL)return false;
	if(pIServerUserItem->GetUserStatus()==US_FREE)return true;
	return pIServerUserItem->SetUserStatus(US_FREE, m_pITableFrame->GetTableID(), pIServerUserItem->GetChairID());
}

bool CTableFrameSink::SendRoomGolobalMessage( const std::string& msg,int type )
{
	bool b = false;

	IServerUserItem * pIServerUserItem = NULL;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem)
		{
			if(type == 1)b = m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_ROOM);
			if(type == 2)b = m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_KIND);
			if(type == 3)b = m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_ALL);
		}
	}
	return b;
}

bool CTableFrameSink::SendRoomGolobalMessage( IServerUserItem * pIServerUserItem,const std::string& msg,int type )
{
	if(NULL != pIServerUserItem)
	{
			if(type == 1) m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_ROOM);
			if(type == 2) m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_KIND);
			if(type == 3) m_pITableFrame->SendRoomMessage(pIServerUserItem, msg.c_str(), SMT_CH_GAME_TO_ALL);
	}
	return true;
}

void CTableFrameSink::KillTimer()
{
	//m_pITableFrame->KillGameTimer(IDI_TIMER_TICK);
	//m_pITableFrame->KillGameTimer(IDI_TIMER_TICK2);
}

void CTableFrameSink::thread_func()
{
	if(m_tickTimerStop)return;
	CAutoLock lock(&m_csLock);
	OnTimerTick();
}	

DWORD WINAPI threadfunc( LPVOID param )
{
	CTableFrameSink* table = (CTableFrameSink*)param;
	while(1)
	{
		if(bExit)break;
		Sleep(1000);
		table->thread_func();
	}
	return 1;
}
