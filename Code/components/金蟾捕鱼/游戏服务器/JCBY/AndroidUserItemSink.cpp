#include "Stdafx.h"
#include "AndroidUserItemSink.h"
//#include "mtdll.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��
#define TIME_LESS_CHANGE_BULLET		60									//�����ӵ�������ʱ��

//��Ϸʱ��
#define TIME_BUY_BULLET				3									//���ӵ�ʱ��
#define TIME_FIRE					3									//�����ӵ�ʱ��
#define TIME_CHANGE_BULLET			120									//�����ӵ�ʱ��
#define TIME_LOCKTAGET			150

////��Ϸʱ��
#define IDI_BUY_BULLET				1001//(IDI_ANDROID_ITEM_SINK+0)			//���ӵ���ʱ��
#define IDI_FIRE					1002//(IDI_ANDROID_ITEM_SINK+1)			//�����ӵ���ʱ��
#define IDI_CHANGE_BULLET			1003//(IDI_ANDROID_ITEM_SINK+2)			//�����ӵ���ʱ��
#define IDI_CHANGE_BULLET_ONCE		1004//(IDI_ANDROID_ITEM_SINK+3)			//�����ӵ���ʱ��
#define IDI_LOCKTARGET                    1000


#define MAX_CANNON_COST        1000		//�޸İ棺10��һ���ڣ�10��һ���ۼӣ���100��һ���ڣ�100���ۼ�һ�Σ������1000��
#define MAX_CANNON_TYPEVALUE	19			//19�ַ�ֵ��ͬ����

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�ӿڱ���
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

	m_nWeaponCost = 10;//Ĭ����10

	m_nToChangeRote = 0;
	m_nLastRoteFireCount = 0;
	m_fNowFireRote = M_PI_2;
	m_onceUpScore = 0;
	m_nNumberToFire = 0;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	//m_pIAndroidUserItem=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	srand((UINT)time(NULL));

	return true;
}

//���ýӿ�
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_nFishGold = 0;
	m_fLastFireRote = 0.0f;

	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	return true;
	switch (nTimerID)
	{
	case IDI_BUY_BULLET:			//���ӵ�
		{
			BuyBullet();

			return true;
		}
	case IDI_CHANGE_BULLET:			//�л��ӵ�
		{
			ChangeCannon();
			return true;
		}
	case IDI_CHANGE_BULLET_ONCE:	//�л��ӵ�(һ��)
		{
			ChangeCannon();
			return true;
		}
	case IDI_FIRE:					//�����ӵ�
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

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
	switch(wSubCmdID)
	{
	case SUB_S_BUY_BULLET_SUCCESS:					//�ӵ�����ɹ���Ϣ
		{
			ASSERT(wDataSize == sizeof(CMD_S_Buy_Bullet_Success));
			if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Success)) return false;

			CMD_S_Buy_Bullet_Success * pBuyBulletSuccess = (CMD_S_Buy_Bullet_Success *)pData;
			
			if (pBuyBulletSuccess->cbChairID == m_pIAndroidUserItem->GetChairID())
			{
				m_nFishGold += pBuyBulletSuccess->dwCount;

				/// �л��ӵ�
				if (rand()%100 < 50)
					m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BULLET, rand()%TIME_CHANGE_BULLET+TIME_LESS_CHANGE_BULLET);
				else
					ChangeCannon();

				m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, rand()%TIME_FIRE+TIME_LESS);
			}
			
			return true;
		}
	case SUB_S_BUY_BULLET_FAILED:					//�ӵ�����ʧ����Ϣ
		{
			return true;
		}
	case SUB_S_CHANGE_CANNON:						//�ӵ��л���Ϣ
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
	case SUB_S_FIRE_LOCK_SUCCESS:		//������Ϣû�е��õ���û�д�ӡ������Ϣ
		{
			ASSERT(wDataSize==sizeof(CMD_S_Fire_Success_Lock));
			if (wDataSize!=sizeof(CMD_S_Fire_Success_Lock)) return false;

			CMD_S_Fire_Success_Lock *pLockTarget = (CMD_S_Fire_Success_Lock *)pData;

			//ASSERT(pLockTarget->cbCount<0);

			cout<<"CMD_S_Fire_Success_Lock   AndroidUserItemSink.cpp"<<endl;
			return true;
		}
	case SUB_S_FIRE_SUCCESS:						//�ӵ�����ɹ�
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
				printf("У��ʧ��!\n");
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
				printf("У��ʧ��!\n");
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
				printf("У��ʧ��!\n");
				return false;
			}	
			m_nFishGold = bomb_->dwPlayerScore;
			return true;
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:				//����״̬
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

			////�������
			//if (pIServerUserItem->GetUserStatus() == US_SIT)
			//{
			//	UINT nElapse=rand()%TIME_BUY_BULLET+TIME_LESS*3;
			//	m_pIAndroidUserItem->SetGameTimer(IDI_BUY_BULLET,nElapse);
			//}
			
			return true;
		}
	case GS_PLAYING:			//��Ϸ״̬
		{
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{

}

//�û��뿪
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//�û�״̬
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//�û���λ
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

/// �����ӵ�
void CAndroidUserItemSink::BuyBullet()
{
	//printf("BuyBullet()\n");
	//CMD_C_Buy_Bullet buyBullet;

	//buyBullet.wCount = m_onceUpScore;

	////��������
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
/// �л��ӵ�
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

	////�л��ӵ�
	//CMD_C_Change_Cannon changeCannon;
	//changeCannon.cbStyle = m_CannonType;
	//changeCannon.wCost = m_nWeaponCost;

	////��������
	//m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_CANNON,&changeCannon,sizeof(changeCannon));
}

/// �����ӵ�
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
	//	/// �����ӵ�
	//	BuyBullet();
	//}
}
//////////////////////////////////////////////////////////////////////////