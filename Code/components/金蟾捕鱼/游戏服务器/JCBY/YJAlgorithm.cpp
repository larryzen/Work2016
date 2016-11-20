#include "Stdafx.h"
#include "YJAlgorithm.h"
//#include "mtdll.h"
#include "math.h"
#include <afx.h>
#include <algorithm>
using namespace std;

//#define Random(x) (rand() % x) //通过取余取得指定范围的随机数

extern "C" int  MTRandomY(int min, int max)
{
	return min+Random(max - min);
}


double Approximate(double a);

void InitMonster(  
				 SMonster *monster_out,
				 int _MonID, 
				 int _MonMul, 
				 int _RT, 
				 int _FirstNT,
				 int _SecondNT, 
				 int _ThirdNt, 
				 double _Influence,
				 double _MLevel,
				 double _level1, 
				 double _level2, 
				 double _level3, 
				 double _level4, 
				 double _level5 )
{
	monster_out->MonID = _MonID;
	monster_out->MonMul = _MonMul;
	monster_out->RT = _RT;
	monster_out->FirstNT = _FirstNT;
	monster_out->SecondNT = _SecondNT;
	monster_out->ThirdNT = _ThirdNt;
	monster_out->Influence = _Influence;
	monster_out->MLevel = _MLevel;
	monster_out->level1 = _level1;
	monster_out->level2 = _level2;
	monster_out->level3 = _level3;
	monster_out->level4 = _level4;
	monster_out->level5 = _level5;
}

SMonster* GetMonster( SMonster* monsterteam, int type)
{
	ASSERT((type-1)>=0);
	ASSERT((type-1)<MAX_FISH_TYPE);
	return &monsterteam[type-1];
}

void SPMonster(SMonster* monster, int MonMul_, const SRoomData* difficult)
{
	InitMonster(monster, 0, 0, 0, 0, 0, 0, difficult->factor, difficult->level, 0, 0.5, 1.0, 1.5, 2.0);
	monster->MonMul = MonMul_;
	double ratio = monster->Influence / (monster->MLevel * monster->MonMul);
	double level = Approximate(ratio);
	monster->RT = (int)level;
	double _RMproduct = pow(level, 3) * ratio;

	int RMproduct = (int)_RMproduct;
	int time = RMproduct;
	int Array[] ={1,1,1};
     
	if (Array[0] * Array[1] * Array[2] == RMproduct)
	{
		 monster->FirstNT = Array[0];
		 monster->SecondNT = Array[1];
         monster->ThirdNT = Array[2];
	}
	for (int i = 0; i < time - 1; i++)
	{
		Array[i % 3] += 1;
		if (Array[0] * Array[1] * Array[2] == RMproduct)
		{ 
			monster->FirstNT = Array[0];
			monster->SecondNT = Array[1];
			monster->ThirdNT = Array[2];
		}
		if (Array[0] * Array[1] * Array[2] > RMproduct)
        {
			int a = Array[0] * Array[1] * Array[2] - RMproduct;

			monster->FirstNT = Array[0];
			monster->SecondNT = Array[1];
			monster->ThirdNT = Array[2];

            Array[(i - 1) % 3] -= 1;
            int b = abs((Array[0] * Array[1] * Array[2] - RMproduct));
            if (b < a)
            {
                 monster->FirstNT = Array[0];
				 monster->SecondNT = Array[1];
				 monster->ThirdNT = Array[2];
                 break;
			}
		} 
	}
}

double Approximate(double a)
{
    for (double i = 3; i < AL_PRECISION; i++)
    {
        double b = i * i * i * a;
        if ((double)((int)b) == b)
        {
			return i;
		}
    }
    return AL_PRECISION;
 }
//计算玩家level
int PlayerLevel(const SPlayer* player,
				const SMonster* monster,
				int MaxGunScore)
{
	double result1 = ((double)(player->playerPutScore - (player->playerNowScore + player->playerGetScore)))/((double)(5000*MaxGunScore));
	double result2 = ((double)MTRandomY(0,2000)) / 1000.0;
	double result = result1 + result2;
	if(result < monster->level1)
		return 0;
	else if(result >= monster->level1 && result < monster->level2)
		return 1;
	else if(result >= monster->level2 && result < monster->level3)
		return 2;
	else if(result >= monster->level3 && result < monster->level4)
		return 3;
	else if(result >= monster->level4 && result < monster->level5)
		return 4;
	else if(result >= monster->level5)
		return 5;
	return 0;
}
//手动影响值
void PlayerRegulation(SPlayer* player, int value)
{
	 player->playerRegulation  = value;
}

#define ARRAY_MAX	2048
static int arrayA [ARRAY_MAX]={0};
static int arrayB [ARRAY_MAX]={0};
static int arrayC [ARRAY_MAX]={0};

//计算结果
int RMSystem(int bulletCost,
			 int MaxGunScore,//当前房间的最大允许每炮分数
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevelFunc
			 )
{
	int playerRegulation = player->playerRegulation;

	if(playerRegulation == -4)return 0;
	if (playerRegulation <= -3 && monster->MonMul >= 150)
	{
		return 0;
	}

	int ArrayMax = monster->RT;
	if(ArrayMax < 0 || ArrayMax >= ARRAY_MAX)return 0;

	for (int i = 1; i < ArrayMax; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayA[i]=0;
		arrayB[i]=0;
		arrayC[i]=0;
	}

	int arrayAT = monster->FirstNT;
	int arrayBT = monster->SecondNT;
	int arrayCT = monster->ThirdNT;

	while(playerRegulation < 0)
	{
		arrayAT --;
		if (arrayAT == 0)
			return 0;
		playerRegulation++;
		if(playerRegulation == 0)
			break;
		arrayCT --;
		if (arrayCT == 0)
			return 0;
		playerRegulation++;
		if(playerRegulation == 0)
			break;
		arrayBT --;
		if(arrayBT == 0)
			return 0;
		playerRegulation++;
	}

	while (playerRegulation > 0)
	{
		arrayAT ++;
		playerRegulation--;
		if (playerRegulation == 0)
			break;
		arrayCT++;
		playerRegulation--;
		if (playerRegulation == 0)
			break;
		arrayBT++;
		playerRegulation--;
	}

	if(arrayAT < 0 || arrayAT >= ARRAY_MAX)return 0;
	if(arrayBT < 0 || arrayBT >= ARRAY_MAX)return 0;
	if(arrayCT < 0 || arrayCT >= ARRAY_MAX)return 0;

	for (int i = 0; i < arrayAT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayA[i] = 1;
	}

	for (int i = 0; i < arrayBT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayB[i] = 1;
	}

	for (int i = 0; i < arrayCT; i++)
	{
		ASSERT(i>=0&&i<ARRAY_MAX);
		arrayC[i] = 1;
	}

	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandomY(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayA[i], arrayA[a]);
	}
	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandomY(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayB[i], arrayB[a]);
	}
	for (int i = 1; i < ArrayMax; i++)
	{
		int a = MTRandomY(0, i);
		ASSERT(i>=0&&i<ARRAY_MAX);
		swap(arrayC[a], arrayC[i]);
	}

	int newarrayA [3]={0};
	int aa = MTRandomY(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(aa>=0&&aa<ARRAY_MAX);
		newarrayA[i] = arrayA[aa];
		aa++;
		if (aa >= ArrayMax)
		{
			aa = 0;
		}
	}
	int newarrayB [3]={0};
	int ab = MTRandomY(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(ab>=0&&ab<ARRAY_MAX);
		newarrayB[i] = arrayB[ab];
		ab++;
		if (ab >= ArrayMax)
		{
			ab = 0;
		}
	}
	int newarrayC [3]={0};
	int ac = MTRandomY(0, ArrayMax - 1);
	for (int i = 0; i < 3; i++)
	{
		ASSERT(ac>=0&&ac<ARRAY_MAX);
		newarrayC[i] = arrayC[ac];
		ac++;
		if (ac >= ArrayMax)
		{
			ac = 0;
		}
	}


	//判断是否成功击杀
	bool killfish = false;
	int level = playerLevelFunc(player, monster, MaxGunScore);
	//printf("level:%d\n",level);
	switch (level)
	{
	case 0:
		break;
	case 1:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		break;
	case 2:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		break;
	case 3:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		break;
	case 4:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[1] && newarrayB[1] == newarrayC[2] && newarrayA[0] != 0)
			killfish = true;
		break;
	case 5:
		if (newarrayA[1] == newarrayB[1] && newarrayB[1] == newarrayC[1] && newarrayA[1] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[0] && newarrayB[0] == newarrayC[0] && newarrayA[0] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[2] && newarrayB[2] == newarrayC[2] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[2] == newarrayB[1] && newarrayB[1] == newarrayC[0] && newarrayA[2] != 0)
			killfish = true;
		if (newarrayA[0] == newarrayB[1] && newarrayB[1] == newarrayC[2] && newarrayA[0] != 0)
			killfish = true;
		break;
	}

	//给分
	if (killfish == true)
		return bulletCost * monster->MonMul;
	else
		return 0;
}
