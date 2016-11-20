#pragma once
#include "sqlite3.h"

#include "ISQliteOperation.h"
#include <Windows.h>
#include <iostream>
using namespace std;

#define ROOMTABLE 1
#define USERINFOTABLE 0


struct DatabaseContent
{
	int row;
	int column;
	char ** azResult;
};


struct IErrorCallBack
{
	virtual void LogError(const char* msg) = 0;
};

//UTF-8到GB2312的转换
inline void U2G(const char* utf8,char *outstr)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	memcpy(outstr, str, strlen(str));
	if(str) delete[] str;
}

//GB2312到UTF-8的转换
inline void G2U(const char* gb2312,char *outstr)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	memcpy(outstr, str, strlen(str));
	if(str) delete[] str;
}

class SQLiteOperate 
{
public:
	SQLiteOperate();
	~SQLiteOperate();


	bool ReadRoomData(const char* roomUID, SRoomData &roomData,int roomPort_);

	//指定ID号查找玩家信息
	bool SearchPlayerUID(DWORD uid);

	//指定ID号查找房间信息
	bool SearchRoomUID(const char* roomUID);

	//添加数据库玩家信息
	bool AddDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in);

	//读取数据库玩家信息
	bool ReadDataBasePlayerData(DWORD uid, SDataBasePlayerData* data_out);

	//更新数据库玩家信息
	bool UpdateDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in);

	void SetErrorCallBack(IErrorCallBack* cb){callback = cb;}


private:

	sqlite3 *db;	
	char *roomNameUID;
	char **azResult;
	int m_playerProfit;
	int UserGdui;
	int tableFalg;
	int m_row;
	int m_reslut;
	IErrorCallBack* callback;
	char m_outstr[256];

public:



};