#include "stdafx.h"
#include "lua.hpp"
#include "TableFrameSink.h"

#define  EXPORT_LUA_FFI extern"C" __declspec(dllexport)

EXPORT_LUA_FFI void SendRoomMessages(CTableFrameSink *pTableFrameSink,IServerUserItem *pIServerUserItem,const char* pszMessages, int nType)  
{  
    pTableFrameSink->SendRoomGolobalMessage(pIServerUserItem,pszMessages,nType);
}

// ��ȡ�����
 EXPORT_LUA_FFI int MaxCost(CGameLogic* pGameLogic)
 {
	if(NULL != pGameLogic)
		return pGameLogic->GetRoomMaxCost();
	else
		return 9900;
 }

// ��ȡ�������
EXPORT_LUA_FFI void GetNickName(IServerUserItem *pIServerUserItem, char* pszName,size_t nSize)
{
	if(NULL != pIServerUserItem)
	{
		_snprintf(pszName,nSize,"%s",pIServerUserItem->GetAccounts());
	}
}

// �����н���Ϣ����Ϸ����
EXPORT_LUA_FFI void SendRewordNotice(CTableFrameSink *pTableFrameSink,int charId,int nType,long lAddScore)
{
	if(NULL != pTableFrameSink)
	{
		pTableFrameSink->SendRewordNotice(charId,nType,lAddScore);
	}
}

// ��ȡ����ID
EXPORT_LUA_FFI int GetChairID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetChairID();
	else
		return 0;
}


//��ȡ����ID
EXPORT_LUA_FFI int GetTableID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetTableID();
	else
		return 0;
}

// ��ȡUserID
EXPORT_LUA_FFI int GetUserID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetUserID();
	else
		return 0;
}

