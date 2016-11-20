#include "stdafx.h"
#include "lua.hpp"
#include "TableFrameSink.h"

#define  EXPORT_LUA_FFI extern"C" __declspec(dllexport)

EXPORT_LUA_FFI void SendRoomMessages(CTableFrameSink *pTableFrameSink,IServerUserItem *pIServerUserItem,const char* pszMessages, int nType)  
{  
    pTableFrameSink->SendRoomGolobalMessage(pIServerUserItem,pszMessages,nType);
}

// 获取最大炮
 EXPORT_LUA_FFI int MaxCost(CGameLogic* pGameLogic)
 {
	if(NULL != pGameLogic)
		return pGameLogic->GetRoomMaxCost();
	else
		return 9900;
 }

// 获取玩家名称
EXPORT_LUA_FFI void GetNickName(IServerUserItem *pIServerUserItem, char* pszName,size_t nSize)
{
	if(NULL != pIServerUserItem)
	{
		_snprintf(pszName,nSize,"%s",pIServerUserItem->GetAccounts());
	}
}

// 发送中奖消息到游戏桌面
EXPORT_LUA_FFI void SendRewordNotice(CTableFrameSink *pTableFrameSink,int charId,int nType,long lAddScore)
{
	if(NULL != pTableFrameSink)
	{
		pTableFrameSink->SendRewordNotice(charId,nType,lAddScore);
	}
}

// 获取椅子ID
EXPORT_LUA_FFI int GetChairID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetChairID();
	else
		return 0;
}


//获取桌子ID
EXPORT_LUA_FFI int GetTableID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetTableID();
	else
		return 0;
}

// 获取UserID
EXPORT_LUA_FFI int GetUserID(IServerUserItem *pIServerUserItem)
{
	if(NULL != pIServerUserItem)
		return pIServerUserItem->GetUserID();
	else
		return 0;
}

