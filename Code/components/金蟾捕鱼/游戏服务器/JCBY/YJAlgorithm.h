#pragma once

#define MAX_FISH_TYPE		20
#define AL_PRECISION	100

struct SPlayer
{
	int playerID;//玩家ID
	int playerNowScore;//玩家目前分数
	int playerNowPutScore;//玩家当次上分数
	int playerPutScore;//玩家总上分值
	int playerGetScore;//玩家总下分值
	int playerShootingTime;//总射击次数
	int playerMaxScore;//分数最大值
	int playerDeadTime;//分数归零次数
	int playerGunScore;//玩家每炮分数
	int playerRegulation;//手动操控影响值
};




struct SMonster
{
	int MonID;//怪物ID
	int MonMul;//怪物倍率
	int RT;//每行数字个数（总数）
	int FirstNT;//首行数字个数（倍率出现数）
	int SecondNT;//次行数字个数（倍率出现数）
	int ThirdNT;//末行数字个数（倍率出现数）
	double Influence;//期望值
	double MLevel;//怪物等级（1到5）
	//五项阶数跨度
	double level1;
	double level2;
	double level3;
	double level4;
	double level5;
};

//房间数据
struct SRoomData
{
	int		roomMaxCost;
	int		onceUpScore;
	float	factor;//总体难度系数
	int		level;//总体难度标准阶数
};

//一个桌子的数据
struct TableData
{
	int totalUpScore;//总上分
	int totalDownScore;//总下分
	int totalNowScore;//总盈利
	int totalPlayUseScore;//总玩家消耗
};

typedef int (*LpPlayerLevel)(const SPlayer* player,
							 const SMonster* monster,
							 int MaxGunScore);

SMonster* GetMonster( SMonster* monsterteam, 
					 int type);

void SPMonster(SMonster* monster_out, 
			   int ratio, 
			   const SRoomData* difficult);

int PlayerLevel(const SPlayer* player, 
				const SMonster* monster, 
				int MaxGunScore);//计算玩家level


//计算结果
int RMSystem(int bulletCost,
			 int MaxGunScore,//当前房间的最大允许每炮分数
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevel
			 );

void PlayerRegulation(SPlayer* player, 
					  int value);//手动影响值


//////////////////////////////////////////////////////////////////////////



