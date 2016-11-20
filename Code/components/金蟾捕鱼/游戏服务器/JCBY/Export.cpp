#include "GameLogic.h"
#include "TableFrameSink.h"
#include <iostream>
#include <fstream>
using namespace std;

#define  EXPORT_CFUNCTION extern"C" __declspec(dllexport)

extern const char* GetFormatTime();

EXPORT_CFUNCTION SPlayer* GetData(void* g, int chairID)
{
	return ((CGameLogic*)g)->GetAlData(chairID);
}

EXPORT_CFUNCTION SGamePlayer* GetGameData(void* g, int chairID)
{
	return ((CGameLogic*)g)->GetData(chairID);
}

EXPORT_CFUNCTION void SendData(void* t, int chairId, int subId, void* pData, int size)
{
	((CTableFrameSink*)t)->SendTableData(chairId, subId, pData, size);
}

EXPORT_CFUNCTION void luaLog(const char* str)
{
	std::ofstream ofs(LOG_FILE_NAME, std::ios::app);
	ofs<<" "<<GetFormatTime()<<":"<<str << std::endl;
}