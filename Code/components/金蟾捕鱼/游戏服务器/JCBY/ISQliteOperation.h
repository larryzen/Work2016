#pragma once
#include "YJAlgorithm.h"

//服务器需要读取的玩家数据
struct SPlayerDBServerReadData
{
	int nowScore;//玩家目前分数
	int nowPutScore;//玩家当次上分数
	int putScore;//玩家总上分值
	int getScore;//玩家总下分值
	int shootingTime;//总射击次数
	int maxScore;//分数最大值
	int deadTime;//分数归零次数
	int gunScore;//玩家每炮分数
	int lostScore;//玩家损失分数
};



//数据库里需要保存的玩家信息
struct SDataBasePlayerData
{
	int nowScore;//玩家目前分数
	int nowPutScore;//玩家当次上分数
	int putScore;//玩家总上分值
	int getScore;//玩家总下分值
	int shootingTime;//总射击次数
	int maxScore;//分数最大值
	int deadTime;//分数归零次数
	int gunScore;//玩家每炮分数
	int lostScore;//玩家损失分数
	int playerRegulation;//手动操控影响值
	char name[32];//玩家名字
	unsigned long IP;//玩家IP
};

inline void SDataBasePlayerData_To_SPlayerDBServerReadData(const SDataBasePlayerData* dbpd_in, SPlayerDBServerReadData* pdbs_out)
{
	if(dbpd_in==NULL || pdbs_out==NULL)return;
	memcpy(pdbs_out, dbpd_in, sizeof(SPlayerDBServerReadData));
}

typedef void (*ErrorCallBack)(const char* msg);//错误信息回调，返回数据库错误原因

/************************************************************************/
/* 数据库操作接口，服务器只会通过此接口进行数据库操作					*/
/* 数据库逻辑改为只做错误判断，不再做其他任何逻辑判断					*/
/* 比如当更新玩家数据时，玩家数据不存在则直接返回false					*/
/************************************************************************/
struct ISQLiteOperation
{
	//释放接口
	virtual void Release() = 0;

	//初始化接口，如无需初始化直接返回true
	virtual bool InitDB() = 0;

	//读取房间信息
	virtual bool ReadRoomData(const char* roomName, SRoomData* data_out) = 0;

	//设置错误回调函数
	virtual void SetErrorCallBack(ErrorCallBack callback) = 0;

	//读取玩家数据库信息(已实现，无需实现)
	bool ReadPlayerData(DWORD uid, SPlayerDBServerReadData* data_out){
		SDataBasePlayerData dbpd;
		bool b=ReadDataBasePlayerData(uid, &dbpd);
		if(!b || data_out==NULL)return false;
		else SDataBasePlayerData_To_SPlayerDBServerReadData(&dbpd, data_out);
		return true;
	}

	//读取手动操控影响值(已实现，无需实现)
	bool ReadPlayerRegulation(DWORD uid, int* data_out){
		if(data_out==NULL)return false;
		SDataBasePlayerData dbpd;
		bool b=ReadDataBasePlayerData(uid, &dbpd);
		if(!b)return false;
		else *data_out = dbpd.playerRegulation;
		return true;
	}

	//更新玩家信息到数据库(已实现，无需实现)
	bool UpdatePlayerData(DWORD uid, const SPlayerDBServerReadData* data_in){
		if(data_in == NULL)return false;
		int playerRegulation=0;
		if(!ReadPlayerRegulation(uid, &playerRegulation))return false;
		SDataBasePlayerData dbpd;
		memcpy(&dbpd, data_in, sizeof(SPlayerDBServerReadData));
		dbpd.playerRegulation= playerRegulation;
		return UpdateDataBasePlayerData(uid, &dbpd);
	}

	//添加玩家信息到数据库(已实现，无需实现)
	bool AddPlayerData(DWORD uid, const SPlayerDBServerReadData* data_in){
		if(data_in == NULL)return false;
		SDataBasePlayerData dbpd;
		memcpy(&dbpd, data_in, sizeof(SPlayerDBServerReadData));
		dbpd.playerRegulation = 0;
		return AddDataBasePlayerData(uid, &dbpd);
	}


protected:
	//添加数据库玩家信息
	virtual bool AddDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in) = 0;

	//读取数据库玩家信息
	virtual bool ReadDataBasePlayerData(DWORD uid, SDataBasePlayerData* data_out) = 0;

	//更新数据库玩家信息
	virtual bool UpdateDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in) = 0;
};

extern "C" 
{
	ISQLiteOperation* CreateISQLiteOperation(void); //创建数据库接口函数
	void SetSQLiteErrorCallBack(ErrorCallBack cb);
}