#pragma once

#define MAX_NORMAL_FISH_TYPE		20
#define MAX_SP_FISH_TYPE			9

#define TEAM_TYPE_FISH				-1//С��
#define SP_TEAM_TYPE_FISH			-2//����С��
#define TYPE_BOMB_FISH				-3//ͬ��ը��
#define WANTER_PLANT				-4//ˮ��

#define DELTA_OFFSET				4
#define DELTA_OFFSET_SP				0

#define MAX_SP_FISH					20//�������������

#define FISH_DESTROY_TIME			30//ɱ�����ʱ��

#define MAX_FISH_OBJECT				2048

#define SCREEN_BOMB_ID				22
#define BOAT_ID						21

struct Fish_Time
{
	int typeId;
	int deltaTime;//�������
	float leftTime;//ʣ�����ʱ��
	int number;
};

const static Fish_Time sNormalFishTable[MAX_NORMAL_FISH_TYPE] = 
{
	{1,		1,	0, 5},
	{2,		2,	0, 4},
	{3,		2,	0, 4},
	{4,		2,	0, 3},
	{5,		2,	0, 3},
	{6,		2,	0, 2},
	{7,		5,	0, 2},
	{8,		3,	0, 2},
	{9,		4,	0, 2},
	{10,	4,	0, 2},
	{11,	10,	0, 2},
	{12,	11,	0, 2},
	{13,	12,	0, 2},
	{14,	13,	0, 2},
	{15,	14,	0, 2},
	{16,	15,	0, 2},
	{17,	16,	0, 2},
	{18,	20,	0, 2},
	{19,	22,	0, 1},
	{20,	30,	0, 1},
};

const static Fish_Time sSpFishTable[MAX_SP_FISH_TYPE]=
{
	{21,	20,	0, 1},
	{22,	15,	0, 1},
	{23,	10,	0, 1},
	{24,	15,	0, 1},
	{25,	10,	0, 1},
	{TEAM_TYPE_FISH,		1,	0, 1},
	{SP_TEAM_TYPE_FISH,		10,	0, 1},
	{TYPE_BOMB_FISH,		20,	0, 1},
	{WANTER_PLANT,			20,	0, 1},
};
