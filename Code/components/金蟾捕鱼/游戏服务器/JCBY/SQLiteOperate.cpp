#include "stdafx.h"
#include "SQLiteOperate.h"
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>


#define OPEN_DATABASE "sqlitejdbc.db"
#define CREAT_TABLEUSERINFO  "create table UserInfo(UserGUID int primary key ,[name] varchar(64),[IP] varchar(64),\
								[playerNowScore] varchar(64), [playerNowPutScore] varchar(64),\
								[playerPutScore] varchar(64),[playerGetScore] varchar(64),[playerShootingTime] varchar(64),\
								[playerMaxScore] varchar(64),[playerDeadTime] varchar(64),\
								[playerGunScore] varchar(64),[lostScore] varchar(64), [playerRegulation] varchar(64))"

#define DELETE_TABLEUSERDB "drop table UserInfo"
#define READ_TABLEUSERINFO "select * from UserInfo where UserGUID= ?"
#define INSERT_TABLEUSERINFO  "INSERT INTO UserInfo([UserGUID],[name],[IP],[playerNowScore], [playerNowPutScore], \
								[playerPutScore],[playerGetScore],[playerShootingTime],[playerMaxScore],[playerDeadTime],\
								[playerGunScore],[lostScore],[playerRegulation]) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?);"
#define REPLACE_TABLEUSERINFO "REPLACE INTO UserInfo([UserGUID],[name],[IP],[playerNowScore], [playerNowPutScore], \
								[playerPutScore],[playerGetScore],[playerShootingTime],[playerMaxScore],[playerDeadTime],\
								[playerGunScore],[lostScore],[playerRegulation]) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?);"

#define UPDATA_TABLEPARTUSERINF "UPDATE UserInfo SET name='%s', IP = '%d',playerNowScore = '%d' ,playerNowPutScore = '%d' ,\
								playerPutScore = '%d' , playerGetScore = '%d' ,playerShootingTime = '%d' , playerMaxScore = '%d' , \
								playerDeadTime = '%d' , playerGunScore = '%d' , lostScore = '%d' WHERE UserGUID = ?;"

#define UPDATA_DIFFIUSERINFO "UPDATE UserInfo SET playerRegulation = '%d' WHERE UserGUID = ?;"


#define UPDATA_TABLEUSERINFO "update UserInfo set playerRegulation='%d'"
#define SELECT_TABLEUSERINFO "select * from UserInfo where UserGUID= ?"
#define SELECT_TABLEUSERINFO_CONDITION "select * from UserInfo where UserGUID= ? or name=? "
#define DELETE_TABLEUSERINFO "delete from UserInfo where UserGUID = ?"
#define READALL_TABLEUSERINFO "select * from UserInfo;"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//游戏房间SQL命令语句
#define CREAT_TABLEROOM  "create table RoomInfo(RoomGUID varchar(64) primary key ,[RoomValue] varchar(64),[RoomPutScroe] varchar(64),[factor] varchar(64) ,[level] varchar(64))"
#define DELETE_TABLEROOMDB "drop table RoomInfo"
#define READ_TABLEROOM "select * from RoomInfo where RoomGUID= ?"
#define INSERT_TABLEROOM  "INSERT INTO RoomInfo([RoomGUID], [RoomValue],[RoomPutSCroe],[factor] ,[level]) VALUES(?,?,?,?,?);"
#define REPLACE_TABLEROOM "REPLACE INTO RoomInfo([RoomGUID], [RoomValue],[RoomPutSCroe],[factor] ,[level]) VALUES(?,?,?,?,?);"
#define SELECT_TABLEROOM "select * from RoomInfo where RoomGUID= ?"
#define DELETE_TABLEROOMINFO "delete from RoomInfo where RoomGUID = ?"
#define READALL_TABLEROOM "select * from RoomInfo;"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//登录用户SQL命令语句
#define READALL_TABLEADMIN "select * from Administrator;"

SQLiteOperate::SQLiteOperate()
{
	UserGdui = 0;
	tableFalg = 0;
	roomNameUID = "";
	m_row = 0;
	db = NULL;
	azResult=NULL;
	callback = NULL;
	m_reslut = 1;
	memset(m_outstr,0,sizeof(m_outstr));

	int rc = sqlite3_open(OPEN_DATABASE,&db); //打开数据库
	//int rc = sqlite3_open_v2(OPEN_DATABASE, &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_NOMUTEX, 0);
	if(db == NULL)
	{
		printf("打开sqlite失败");
	}
	else if(rc != SQLITE_OK) //数据库打开失败
	{
		printf("打开sqlite失败");
		sqlite3_close(db); //关闭数据库连接
	}
}

SQLiteOperate::~SQLiteOperate()
{
	if(db != NULL)
		sqlite3_close(db);
}

bool SQLiteOperate::ReadRoomData(const char* roomUID, SRoomData &roomData,int roomPort_)
{
	if(strlen(roomUID) <= 0)
		return false;

	char *sql = NULL;
	sql = READ_TABLEROOM;	
	sqlite3_stmt  *stmt = NULL;
	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);
	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));
		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}		
		return false;
	}
	memset(m_outstr,0,sizeof(m_outstr));
	G2U(roomUID,m_outstr);
	//sqlite3_bind_text(stmt, 1,m_outstr,(int)strlen(m_outstr),NULL);
	sqlite3_bind_int(stmt,1,roomPort_);
	int nColumn = sqlite3_column_count(stmt); //获取数据库表的列数

	rc = sqlite3_step(stmt);

	const char * p = NULL;
	p = (const char *)sqlite3_column_text16(stmt,0);
	if(p == NULL)
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	if(strcmp(p , roomUID) == 0)
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	memset(m_outstr,0,sizeof(m_outstr));
	U2G(roomUID,m_outstr);
	//memcpy(roomNameUID, m_outstr,strlen(m_outstr));
	roomData.roomMaxCost = sqlite3_column_int(stmt,1);
	roomData.onceUpScore = sqlite3_column_int(stmt,2);
	roomData.factor = (float)sqlite3_column_double(stmt,3);
	roomData.level = sqlite3_column_int(stmt,4);
#ifdef _DEBUG
	ASSERT(roomData.roomMaxCost >= 0 && roomData.onceUpScore >= 0 && (roomData.factor >= 0.1f && roomData.factor <= 2.0f) 
		&& (roomData.level >= 1 && roomData.level <= 5));
#else
	if( roomData.roomMaxCost < 0 || roomData.onceUpScore < 0 || (roomData.factor < 0.1f  || roomData.factor > 2.0f)
		|| (roomData.level < 1 || roomData.level > 5))
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
#endif

	rc = sqlite3_step(stmt); 

	if(rc == SQLITE_DONE)
	{
		//AfxMessageBox("已将记录存入数据库!");
	}
	else
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return true;
}

bool SQLiteOperate::SearchPlayerUID(DWORD uid)
{
	if(uid < 0 )
		return false;

	sqlite3_stmt  *stmt = NULL;
	char *sql = NULL;
	sql = SELECT_TABLEUSERINFO; 	
	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);
	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));

		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}
		return false;
	}
	sqlite3_bind_int(stmt,1,uid);	
	rc = sqlite3_step(stmt);

	if(sqlite3_column_int(stmt,0) != uid)
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	return true;
}

//指定ID号查找房间信息
bool SQLiteOperate::SearchRoomUID(const char* roomUID)
{
	if( strlen(roomUID) == 0 )
		return false;

	sqlite3_stmt  *stmt = NULL;
	char *sql = NULL;
	sql = SELECT_TABLEROOM; 	
	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);
	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));

		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}
		return false;
	}
	sqlite3_bind_text(stmt,1,roomUID,(int)strlen(roomUID),NULL);
	rc = sqlite3_step(stmt);

	char * p = NULL;
	p = (char *)sqlite3_column_text16(stmt,0);
	if(p == NULL)
	{
		sqlite3_reset(stmt);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	if(strcmp(p , roomUID) == 0)
	{
		sqlite3_finalize(stmt);
		return true;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	return false;
}

//添加数据库玩家信息
bool SQLiteOperate::AddDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in)
{
	if( data_in == NULL)
		return false;
	if( SearchPlayerUID(uid))
		return false;

	sqlite3_stmt *stmt = NULL; //准备语句对象
	char *sql = NULL;
	sql = INSERT_TABLEUSERINFO; 
	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);
	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));
		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}
		return false;
	}

#ifdef _DEBUG
	ASSERT( uid >= 0 &&  (int)strlen(data_in->name) >= 0  &&  data_in->IP >= 0 
		&& data_in->nowPutScore >= 0  && data_in->putScore >= 0  && 
		data_in->getScore >= 0  && data_in->shootingTime >= 0  && data_in->maxScore >= 0  && 
		data_in->deadTime >= 0  && data_in->gunScore >= 0  && data_in->lostScore >= 0   );
#else
	if( uid < 0 ||  (int)strlen(data_in->name) < 0  ||  data_in->IP < 0  
		|| data_in->nowPutScore < 0  || data_in->putScore < 0  || 
		data_in->getScore < 0  || data_in->shootingTime < 0  || data_in->maxScore < 0  || 
		data_in->deadTime < 0  || data_in->gunScore < 0  || data_in->lostScore < 0   )
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
#endif

	if(data_in->playerRegulation < -4 )
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}

	rc = sqlite3_bind_int(stmt,  1, uid);
	memset(m_outstr,0,sizeof(m_outstr));
	G2U(data_in->name,m_outstr);
	rc = sqlite3_bind_text(stmt, 2, m_outstr,(int)strlen(m_outstr),NULL );
	rc = sqlite3_bind_int(stmt, 3, data_in->IP );
	rc = sqlite3_bind_int(stmt,  4, data_in->nowScore );
	rc = sqlite3_bind_int(stmt,  5, data_in->nowPutScore);
	rc = sqlite3_bind_int(stmt,  6, data_in->putScore);
	rc = sqlite3_bind_int(stmt,  7, data_in->getScore);
	rc = sqlite3_bind_int(stmt,  8, data_in->shootingTime);
	rc = sqlite3_bind_int(stmt,  9, data_in->maxScore);
	rc = sqlite3_bind_int(stmt, 10, data_in->deadTime);
	rc = sqlite3_bind_int(stmt, 11, data_in->gunScore);
	rc = sqlite3_bind_int(stmt, 12, data_in->lostScore);
	rc = sqlite3_bind_int(stmt, 13, data_in->playerRegulation);		

	rc = sqlite3_step(stmt);
	if(rc!=SQLITE_DONE) 
	{ 
		if(callback)callback->LogError(sqlite3_errmsg(db));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false; 
	} 
	//cout<<"数据插入成功！"<<endl;
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return true;
}

//读取数据库玩家信息
bool SQLiteOperate::ReadDataBasePlayerData(DWORD uid, SDataBasePlayerData* data_out)
{
	char *sql = NULL;
	sql = READ_TABLEUSERINFO;	
	sqlite3_stmt  *stmt = NULL;
	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);
	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));
		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}
		return false;
	}
	sqlite3_bind_int(stmt,1,uid);
	int nColumn = sqlite3_column_count(stmt); //获取数据库表的列数

	rc = sqlite3_step(stmt);

	if(sqlite3_column_int(stmt,0) != uid)
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}


	UserGdui = sqlite3_column_int(stmt,0);

	memset(data_out->name,0,sizeof(data_out->name));	
	char* pstr = (char*)sqlite3_column_text(stmt,1);

	memset(m_outstr,0,sizeof(m_outstr));
	U2G(pstr,m_outstr);
	memcpy(data_out->name ,m_outstr,(int)strlen(m_outstr));

	data_out->IP = sqlite3_column_int(stmt,2);	

	data_out->nowScore  = sqlite3_column_int(stmt,3);
	data_out->nowPutScore = sqlite3_column_int(stmt,4);
	data_out->putScore = sqlite3_column_int(stmt,5);
	data_out->getScore = sqlite3_column_int(stmt,6);
	data_out->shootingTime = sqlite3_column_int(stmt,7);
	data_out->maxScore = sqlite3_column_int(stmt,8);
	data_out->deadTime = sqlite3_column_int(stmt,9);
	data_out->gunScore = sqlite3_column_int(stmt,10);
	data_out->lostScore = sqlite3_column_int(stmt,11);
	data_out->playerRegulation = sqlite3_column_int(stmt,12);

#ifdef _DEBUG
	ASSERT( UserGdui >= 0 &&  (int)strlen(data_out->name) >= 0  &&  data_out->IP >= 0 &&  
		data_out->nowScore >= 0 && data_out->nowPutScore >= 0 && data_out->putScore >= 0  && 
		data_out->getScore >= 0 && data_out->shootingTime >= 0 && data_out->maxScore >= 0  && 
		data_out->deadTime >= 0 && data_out->gunScore >= 0  && data_out->lostScore >= 0  );
#else
	if( UserGdui < 0 ||  (int)strlen(data_out->name) < 0  ||  data_out->IP < 0 ||  
		data_out->nowScore< 0 || data_out->nowPutScore< 0 || data_out->putScore< 0  || 
		data_out->getScore< 0 || data_out->shootingTime< 0 || data_out->maxScore< 0  || 
		data_out->deadTime< 0 || data_out->gunScore< 0  || data_out->lostScore< 0  )
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
#endif

	if(data_out->playerRegulation < -4 )
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}

	m_playerProfit =  (data_out->nowScore + data_out->getScore) - data_out->nowPutScore;

	rc = sqlite3_step(stmt); 

	if(rc == SQLITE_DONE)
	{
		//cout<<"select finish!"<<endl;
		//AfxMessageBox("已将记录存入数据库!");
	}
	else
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return true;
}

//更新数据库玩家信息
bool SQLiteOperate::UpdateDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in)
{
	if( data_in == NULL)
		return false;
	if( !SearchPlayerUID(uid))
		return false;

	sqlite3_stmt *stmt = NULL; //准备语句对象
	//char *sql = NULL;
	//sql = UPDATE_TABLEUSERINFO1;  

	char *sql;
	memset(m_outstr,0,sizeof(m_outstr));
	G2U(data_in->name,m_outstr);
	sql= sqlite3_mprintf(UPDATA_TABLEPARTUSERINF,m_outstr /*data_in->name */,data_in->IP,data_in->nowScore,
		data_in->nowPutScore,data_in->putScore,data_in->getScore,data_in->shootingTime,
		data_in->maxScore,data_in->deadTime,data_in->gunScore,data_in->lostScore);

	int rc = sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,NULL);

	if(rc != SQLITE_OK)
	{
		if(callback)callback->LogError(sqlite3_errmsg(db));
		if(stmt)
		{
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
		}
		return false;
	}

#ifdef _DEBUG
	ASSERT( uid >= 0 &&  (int)strlen(data_in->name) >= 0  &&  data_in->IP >= 0  
		&& data_in->nowPutScore >= 0  && data_in->putScore >= 0  && 
		data_in->getScore >= 0  && data_in->shootingTime >= 0  && data_in->maxScore >= 0  && 
		data_in->deadTime >= 0  && data_in->gunScore >= 0  && data_in->lostScore >= 0   );
#else
	if( uid < 0 ||  (int)strlen(data_in->name) < 0  ||  data_in->IP < 0  ||  data_in->nowScore < 0  
		|| data_in->nowPutScore < 0  || data_in->putScore < 0  || 
		data_in->getScore < 0  || data_in->shootingTime < 0  || data_in->maxScore < 0  || 
		data_in->deadTime < 0  || data_in->gunScore < 0  || data_in->lostScore < 0   )
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false;
	}
#endif

	rc = sqlite3_bind_int(stmt,  1, uid);

	rc = sqlite3_step(stmt);
	if(rc!=SQLITE_DONE) 
	{ 
		if(callback)callback->LogError(sqlite3_errmsg(db));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		return false; 
	} 
	//printf("修改成功!\n"); 

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	sqlite3_free(sql);

	return true;
}
