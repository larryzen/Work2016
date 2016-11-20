#include "Stdafx.h"
#include "Logging.h"
#include <time.h>
#include <stdarg.h>

static char szTime[128] = {0};
static char szTemp[10240] = {0};

static int FindTable(void* db_, int nCount, char** pValue,char** pName )
{
	sqlite3 *db = (sqlite3 *)db_;
	if (nCount==0)return SQLITE_ERROR;
	if (strcmp("count(*)", pName[0])!=0)return SQLITE_ERROR;

	char* cnum = pValue[0];
	int num = atoi(cnum);

	if (num==0)
	{
		const char* op = "create table Logging(Time varchar(32), Content varchar(64))";
		char* error = NULL;
		int rc = sqlite3_exec(db, op, 0, 0, &error);
		if (error)
		{
			sqlite3_free(error);
			return SQLITE_ERROR;
		}
	}
	return SQLITE_OK;
}

const char* GetFormatTime()
{
	time_t now;
	time(&now);
	struct tm time;
	localtime_s(&time, &now);
	strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &time);
	return szTime;
}

CSqlite3Logging::CSqlite3Logging( )
{
	db = NULL;
	stat = NULL;
	memset(szTime,0,sizeof(szTime));
}

CSqlite3Logging::~CSqlite3Logging()
{
	_freeStat();
	if(db)sqlite3_close(db);
}

void CSqlite3Logging::SetLogFile( const char* logfilename )
{
	return;
	int rc = sqlite3_open(logfilename, &db); //打开数据库
	if (rc != SQLITE_OK || db == NULL)
	{
		throw std::exception("can not open log file");
	}
	else
	{
		char* error = NULL;
		const char* op="select count(*) from sqlite_master where type = 'table' and name = 'Logging'";
		sqlite3_exec(db, op, FindTable, db, &error);
		if (error)
		{
			sqlite3_free(error);
			throw std::exception(error);
		}
	}
}

void CSqlite3Logging::Log( const std::string& content )
{
	return;
	const static char* op="insert into Logging values(?,?)";
	sqlite3_prepare(db,op,-1,&stat,0);
	sqlite3_bind_text(stat, 1, GetFormatTime(), -1, 0);
	sqlite3_bind_text(stat, 2, content.c_str(), (int)content.size(), 0);
	int rc = sqlite3_step(stat);
	
	_freeStat();

#ifdef _DEBUG
	printf("%s\n",content.c_str());
#endif // _DEBUG
}	



void CSqlite3Logging::Log( const char* fmt, ... )
{
	return;
	va_list args;
	va_start(args, fmt);
	vsprintf_s(szTemp, sizeof(szTemp), fmt, args);
	va_end(args);
	Log(std::string(szTemp));
}
