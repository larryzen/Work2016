#include "Stdafx.h"
#include "AndroidUserItemSink.h"
//#include "mtdll.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间
#define TIME_LESS_CHANGE_BULLET		60									//交换子弹的最少时间

//游戏时间
#define TIME_BUY_BULLET				3									//买子弹时间
#define TIME_FIRE					3									//发射子弹时间
#define TIME_CHANGE_BULLET			120									//交换子弹时间
#define TIME_LOCKTAGET			150

////游戏时间
#define IDI_BUY_BULLET				1001//(IDI_ANDROID_ITEM_SINK+0)			//买子弹定时器
#define IDI_FIRE					1002//(IDI_ANDROID_ITEM_SINK+1)			//发射子弹定时器
#define IDI_CHANGE_BULLET			1003//(IDI_ANDROID_ITEM_SINK+2)			//交换子弹定时器
#define IDI_CHANGE_BULLET_ONCE		1004//(IDI_ANDROID_ITEM_SINK+3)			//交换子弹定时器
#define IDI_LOCKTARGET                    1000


#define MAX_CANNON_COST        1000		//修改版：10分一个炮，10分一次累加，到100分一个炮，100分累加一次，最高难1000分
#define MAX_CANNON_TYPEVALUE	19			//19种分值不同的炮

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//接口变量
	m_pIAndroidUserItem=NULL;

	srand((UINT)time(NULL));

	//m_bType = rand()%3;

	m_nFishGold = 0;
	m_cbScene = 0;
	m_lCellScore = 0;
	m_CannonType = 0;
	m_fLastFireRote = 0.0f;
	
    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObjects[i].wChairID = -1;
        m_RoleObjects[i].dwFishGold = 0;
        m_RoleObjects[i].wCannonType = 0;
    }

	m_nWeaponCost = 10;//默认是10

	m_nToChangeRote = 0;
	m_nLastRoteFireCount = 0;
	m_fNowFireRote = M_PI_2;
	m_onceUpScore = 0;
	m_nNumberToFire = 0;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	//m_pIAndroidUserItem=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	srand((UINT)time(NULL));

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_nFishGold = 0;
	m_fLastFireRote = 0.0f;

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	return true;
	switch (nTimerID)
	{
	case IDI_BUY_BULLET:			//买子弹
		{
			BuyBullet();

			return true;
		}
	case IDI_CHANGE_BULLET:			//切换子弹
		{
			ChangeCannon();
			return true;
		}
	case IDI_CHANGE_BULLET_ONCE:	//切换子弹(一次)
		{
			ChangeCannon();
			return true;
		}
	case IDI_FIRE:					//发射子弹
		{
			Fire();

			return true;
		}
	case IDI_LOCKTARGET:
		{
			cout<<"IDI_TEST"<<endl;
			LockTarget();
			return true;
		}
	}

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
	switch(wSubCmdID)
	{
	case SUB_S_BUY_BULLET_SUCCESS:					//子弹购买成功消息
		{
			ASSERT(wDataSize == sizeof(CMD_S_Buy_Bullet_Success));
			if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Success)) return false;

			CMD_S_Buy_Bullet_Success * pBuyBulletSuccess = (CMD_S_Buy_Bullet_Success *)pData;
			
			if (pBuyBulletSuccess->cbChairID == m_pIAndroidUserItem->GetChairID())
			{
				m_nFishGold += pBuyBulletSuccess->dwCount;

				/// 切换子弹
				if (rand()%100 < 50)
					m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BULLET, rand()%TIME_CHANGE_BULLET+TIME_LESS_CHANGE_BULLET);
				else
					ChangeCannon();

				m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, rand()%TIME_FIRE+TIME_LESS);
			}
			
			return true;
		}
	case SUB_S_BUY_BULLET_FAILED:					//子弹购买失败消息
		{
			return true;
		}
	case SUB_S_CHANGE_CANNON:						//子弹切换消息
		{
			//ASSERT(wDataSize==sizeof(CMD_S_Change_Cannon));
			//if (wDataSize!=sizeof(CMD_S_Change_Cannon)) return false;

			//CMD_S_Change_Cannon *pChangeCannon = (CMD_S_Change_Cannon *)pData;

			//ASSERT(pChangeCannon->cbStyle<MAX_CANNON_STYLE);

			//if (pChangeCannon->cbChairID==m_pIAndroidUserItem->GetChairID())
			//{
			//	m_CannonType = pChangeCannon->cbStyle;
			//}

			//m_pIAndroidUserItem->SetGameTimer(IDI_LOCKTARGET, rand()%TIME_LOCKTAGET);
			return true;
		}
	case SUB_S_FIRE_LOCK_SUCCESS:		//这条消息没有调用到，没有打印调试信息
		{
			ASSERT(wDataSize==sizeof(CMD_S_Fire_Success_Lock));
			if (wDataSize!=sizeof(CMD_S_Fire_Success_Lock)) return false;

			CMD_S_Fire_Success_Lock *pLockTarget = (CMD_S_Fire_Success_Lock *)pData;

			//ASSERT(pLockTarget->cbCount<0);

			cout<<"CMD_S_Fire_Success_Lock   AndroidUserItemSink.cpp"<<endl;
			return true;
		}
	case SUB_S_FIRE_SUCCESS:						//子弹发射成功
		{			
			ASSERT(wDataSize==sizeof(CMD_S_Fire_Success));
			if (wDataSize!=sizeof(CMD_S_Fire_Success)) return false;

			//CMD_S_Fire_Success *pFireSuccess = (CMD_S_Fire_Success *)pBuffer;

			return true;
		}
	case SUB_S_CAST_NET_SUCCESS:
		{
			const CMD_S_Cast_Net_Success_dte *pCastNetSuccess = (CMD_S_Cast_Net_Success_dte *)pData;

			if (wDataSize!=sizeof(CMD_S_Cast_Net_Success_dte)-(MAX_FISH_IN_CUCESS-pCastNetSuccess->cbCount)*sizeof(CMD_C_Fish_Net_Object)) 
			{
				printf("校验失败!\n");
				return false;
			}
			m_nFishGold = pCastNetSuccess->dwPlayerScore;
			return true;
		}
	case SUB_S_HIT_TEAM_SUCCESS:
		{
			const CMD_S_HitTeam_Success_dte* team_ = (CMD_S_HitTeam_Success_dte*)pData;
			if (wDataSize != sizeof(CMD_S_HitTeam_Success_dte)-(MAX_FISH_IN_TEAM-team_->cbCount)*sizeof(CMD_C_Fish_Net_Object))
			{
				printf("校验失败!\n");
				return false;
			}	
			m_nFishGold = team_->dwPlayerScore;
			return true;
		}
	case SUB_S_TPYE_BOBM_SUCCESS:
		{
			const CMD_S_TypeBomb_Success_dte* bomb_ = (CMD_S_TypeBomb_Success_dte*)pData;
			if (wDataSize != sizeof(CMD_S_TypeBomb_Success_dte)-(MAX_FISH_BY_BOMB-bomb_->cbCount)*sizeof(CMD_C_Fish_Net_Object))
			{
				printf("校验失败!\n");
				return false;
			}	
			m_nFishGold = bomb_->dwPlayerScore;
			return true;
		}
	}

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:				//空闲状态
		{
			//ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			//if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			//CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

			//m_cbScene = pStatusFree->cbScene;
			//m_lCellScore = pStatusFree->lCellScore;
			//m_maxWeaponCost = pStatusFree->wMaxFire;
			//m_onceUpScore = pStatusFree->wOnceUpScore;
			//
   //         CopyMemory(m_RoleObjects, pStatusFree->RoleObjects, GAME_PLAYER*sizeof(Role_Net_Object));

			//IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//if (pIServerUserItem == NULL) return true;

			////玩家设置
			//if (pIServerUserItem->GetUserStatus() == US_SIT)
			//{
			//	UINT nElapse=rand()%TIME_BUY_BULLET+TIME_LESS*3;
			//	m_pIAndroidUserItem->SetGameTimer(IDI_BUY_BULLET,nElapse);
			//}
			
			return true;
		}
	case GS_PLAYING:			//游戏状态
		{
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{

}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

/// 购买子弹
void CAndroidUserItemSink::BuyBullet()
{
	//printf("BuyBullet()\n");
	//CMD_C_Buy_Bullet buyBullet;

	//buyBullet.wCount = m_onceUpScore;

	////发送数据
	//m_pIAndroidUserItem->SendSocketData(SUB_C_BUY_BULLET,&buyBullet,sizeof(buyBullet));
}

void CAndroidUserItemSink::LockTarget()
{
	////cout<<"ClearBullet()"<<endl;
	//CMD_C_Fire_Lock lockTarget;
	//CMD_C_Fish_Net_Object fishtpye;
	//fishtpye.cbType = 0;
	//fishtpye.wID = 0;
	//lockTarget.target = fishtpye;

	//m_pIAndroidUserItem->SendSocketData(SUB_C_FIRE_LOCK, &lockTarget, sizeof(CMD_C_Fire_Lock));
}
/// 切换子弹
void CAndroidUserItemSink::ChangeCannon()
{
	//if ( m_nWeaponCost < 100 )
	//{
	//	m_nWeaponCost += 10;
	//}
	//else if ( m_nWeaponCost >= 100 && m_nWeaponCost < 1000 )
	//{
	//	m_nWeaponCost += 100;
	//}
	//else if ( m_nWeaponCost >= 1000  )
	//{
	//	m_nWeaponCost += 1000;
	//}

	////切换子弹
	//CMD_C_Change_Cannon changeCannon;
	//changeCannon.cbStyle = m_CannonType;
	//changeCannon.wCost = m_nWeaponCost;

	////发送数据
	//m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_CANNON,&changeCannon,sizeof(changeCannon));
}

/// 发射子弹
void CAndroidUserItemSink::Fire()
{
	//if (m_nFishGold > m_nWeaponCost)
	//{		
	//	m_nLastRoteFireCount++;
	//	CMD_C_Fire fire;

	//	WORD wChairID = m_pIAndroidUserItem->GetChairID();

	//	//if (wChairID == 0 || wChairID == 1)
	//	//	Fire.fRote += 180;
	//	m_fLastFireRote = m_fNowFireRote;
	//	if (m_nLastRoteFireCount >= m_nToChangeRote)
	//	{
	//		m_nLastRoteFireCount = 0;
	//		m_nToChangeRote = MTRandom(2,20);
	//		m_fNowFireRote = (float)MTRandom(0,(int)(M_PI*100000))/100000.f;
	//		//printf("m_fNowFireRote:%lf\n",m_fNowFireRote);
	//		m_fNowFireRote-=M_PI_2;
	//	}
	//	
	//	fire.fRote = m_fNowFireRote;

	//	if (m_nNumberToFire == 0)
	//	{
	//		m_nNumberToFire = MTRandom(1,10);
	//		m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, rand()%TIME_FIRE+TIME_LESS);
	//		//printf("m_nNumberToFire == 0\n");
	//	}
	//	else
	//	{
	//		m_pIAndroidUserItem->SendSocketData(SUB_C_FIRE, &fire, sizeof(CMD_C_Fire));
	//		m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, 220);
	//		//printf("m_nNumberToFire != 0\n");
	//	}

	//	m_nNumberToFire--;
	//	
	//}
	//else if (m_nFishGold != 0 && (rand()%10)>3)
	//{
	//	ChangeCannon();
	//}
	//else
	//{
	//	/// 购买子弹
	//	BuyBullet();
	//}
}
//////////////////////////////////////////////////////////////////////////