#pragma once

#define MAX_FISH_TYPE		20
#define AL_PRECISION	100

struct SPlayer
{
	int playerID;//���ID
	int playerNowScore;//���Ŀǰ����
	int playerNowPutScore;//��ҵ����Ϸ���
	int playerPutScore;//������Ϸ�ֵ
	int playerGetScore;//������·�ֵ
	int playerShootingTime;//���������
	int playerMaxScore;//�������ֵ
	int playerDeadTime;//�����������
	int playerGunScore;//���ÿ�ڷ���
	int playerRegulation;//�ֶ��ٿ�Ӱ��ֵ
};




struct SMonster
{
	int MonID;//����ID
	int MonMul;//���ﱶ��
	int RT;//ÿ�����ָ�����������
	int FirstNT;//�������ָ��������ʳ�������
	int SecondNT;//�������ָ��������ʳ�������
	int ThirdNT;//ĩ�����ָ��������ʳ�������
	double Influence;//����ֵ
	double MLevel;//����ȼ���1��5��
	//����������
	double level1;
	double level2;
	double level3;
	double level4;
	double level5;
};

//��������
struct SRoomData
{
	int		roomMaxCost;
	int		onceUpScore;
	float	factor;//�����Ѷ�ϵ��
	int		level;//�����Ѷȱ�׼����
};

//һ�����ӵ�����
struct TableData
{
	int totalUpScore;//���Ϸ�
	int totalDownScore;//���·�
	int totalNowScore;//��ӯ��
	int totalPlayUseScore;//���������
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
				int MaxGunScore);//�������level


//������
int RMSystem(int bulletCost,
			 int MaxGunScore,//��ǰ������������ÿ�ڷ���
			 const SMonster* monster,
			 const SPlayer* player,
			 LpPlayerLevel playerLevel
			 );

void PlayerRegulation(SPlayer* player, 
					  int value);//�ֶ�Ӱ��ֵ


//////////////////////////////////////////////////////////////////////////



