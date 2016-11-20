#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <exception>
#include <string>
#include <sstream>
#include "safestring.h"

typedef std::stringstream SSstr;

class CSqlite3Logging
{
private:
	sqlite3 *db;
	sqlite3_stmt *stat;
	SSstr str;
public:
	CSqlite3Logging();
	~CSqlite3Logging();

	void SetLogFile(const char* logfilename);

	void Log(const std::string& content);
	void Log(const char* fmt, ...);

private:
	void _freeStat()
	{
		if (stat)
		{
			sqlite3_finalize(stat);
			stat = NULL;
		}
	}
};

const char* GetFormatTime();