#ifndef CMD_FISH_HEAD_FILE
#define CMD_FISH_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						404								//��Ϸ I D
#define GAME_PLAYER					6								//��Ϸ����
#define GAME_NAME					TEXT("��󸲶��")				//��Ϸ����

#define AI_BULLET_MAX_COUNT			5	 								// �����ӵ������Ŀ

//////////////////////////////////////////////////////////////////////////
//��������ͻ��˷��͵�����
enum ServerCMD{
	SUB_S_BEGIN = 101,
	SUB_S_FIRE_LOCK_SUCCESS       ,//��������ɹ�
	SUB_S_SEND_FISH_PATH		  ,//ɢ��
	SUB_S_SEND_FISH_PATH_GROUP    ,//����
	SUB_S_SEND_CLEAR_PLAYER		  ,//������
	SUB_S_SEND_HIGHT_SCORE        ,//ȫ�ַ���֪ͨ
	SUB_S_SEND_CANNON_ROTATE	  ,//��Ͳƫת
	SUB_S_SEND_GET_ENERGY_GUN     ,//���������
	SUB_S_CHANGE_SCENE            ,//�ı䳡��
	SUB_S_BUY_BULLET_SUCCESS      ,//�����ӵ��ɹ�
	SUB_S_BUY_BULLET_FAILED       ,//�����ӵ�ʧ��
	SUB_S_REWORD_NOTIC	          ,//��������
	SUB_S_ENERGY_CHANGE	          ,//�����ı�
	SUB_S_FIRE_SUCCESS            ,//�����ӵ��ɹ�
	SUB_S_FIRE_FAILED             ,//�����ӵ�ʧ��
	SUB_S_CAST_NET_SUCCESS        ,//�����ɹ�
	SUB_S_CAST_NET_FAILED         ,//����ʧ��
	SUB_S_CHANGE_CANNON           ,//�ı���Ͳ
	SUB_S_ACCOUNT                 ,//�˻����أ�Դ����ֻ�����رղ���
	SUB_S_HIT_TEAM_SUCCESS        ,//����С��
	SUB_S_TPYE_BOBM_SUCCESS       ,//ͬ��ը���ɹ�
	SUB_S_TASK                    ,//�����������������
	SUB_S_AI_ENTER                ,//�����ڷ���ɹ�
	SUB_S_BOMB_SUCCESS            ,//ը���ɹ�
	SUB_S_BOMB_SUCCESS_FAILED     ,//ը��ʧ�ܣ�δʹ��
	SUB_S_SEND_TIME,
	SUB_S_END
};

#define OFFSET 51

//�ͻ������������������
enum ClientCMD{
	SUB_C_BEGIN = SUB_S_END+OFFSET,
	SUB_C_FIRE                    ,//����
	SUB_C_BUY_BULLET              ,//�����ӵ�
	SUB_C_CAST_NET                ,//����
	SUB_C_CHANGE_CANNON           ,//�ı���Ͳ����
	SUB_C_ACCOUNT                 ,//�˻�
	SUB_C_HIT_BOAT                ,//���к�����
	SUB_C_HIT_TEAM                ,//����С��
	SUB_C_FIRE_LOCK			      ,//��������
	SUB_C_SPECAIL_FIRE            ,//������
	SUB_C_CANNON_ROTATE			  ,//��Ͳƫת
	SUB_C_TPYE_BOBM               ,//ͬ��ը��
	SUB_C_SCREEN_BOBM             ,//ȫ��ը��
	SUB_C_GAME_END,					//��Ϸ����
	SUB_C_END
};
//////////////////////////////////////////////////////////////////////////

#define MAX_FISH_IN_NET         5
#define MAX_FISH_IN_CUCESS      192
#define MAX_FISH_BY_BOMB		192
#define MAX_FISH_IN_TEAM		16
#define MAX_FISH_SEND           50
#define MAX_IN_GROUP			1024

#define MAX_FISH_PATH           14
#define MAX_FISH_STYLE          12

#define MAX_CANNON_STYLE        2 

#define MAX_SCENE              3

#define MAX_SMALL_POINT_PATH   208
#define MAX_BIG_POINT_PATH     130
#define MAX_HUGE_POINT_PATH    62

#define MAX_LEFT_LINE_SPATH     3
#define MAX_RIGHT_LINE_SPATH     3

#define MAX_REWORD_MSG			128
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#pragma pack(1)

//���
struct Role_Net_Object
{
    WORD wChairID;//����ID
    DWORD dwFishGold;//���
    WORD wCannonType;  //������
	WORD wCost;//����
	BYTE cbEnergy;//������
	bool isAI;
	BYTE cbSeed;//�������������
};

/////////////////////////////////////////////////////////////////////////
struct CMD_S_StatusFree
{
    BYTE cbScene;
	DWORD dwOnceUpScore;//ÿ���Ϸ���ֵ
	WORD wMaxFire;//�������
    LONG lCellScore;//��λ���֣����ڼ��㻻��
    Role_Net_Object RoleObjects[GAME_PLAYER];
};

struct CMD_S_Change_Scene 
{
    BYTE cbScene;
};

struct CMD_C_Buy_Bullet
{
    DWORD dwCount;
};

struct CMD_S_Buy_Bullet_Success
{
    BYTE cbChairID;
    DWORD dwCount;
};

struct CMD_S_Buy_Bullet_Failed
{
    BYTE cbChairID;
    char szReason[32];
};

struct CMD_C_Fire
{
    float fRote;
};

struct CMD_S_Fire_Success
{
    BYTE  cbChairID;
    float fRote;
};

struct CMD_S_Fire_Failed
{
    WORD wChairID;//�ɸĽ���ʹ��BYTE����
    char szReason[32];
};

struct CMD_S_Bomb_Failed
{
    WORD wChairID;//�ɸĽ���ʹ��BYTE����
    char szReason[32];
};

struct CMD_C_Change_Cannon
{
    BYTE cbStyle;
	WORD wCost;
};

struct CMD_S_Change_Cannon
{
    BYTE cbChairID;
    BYTE cbStyle;
	WORD wCost;
};

struct CMD_C_Account
{
    BYTE cbChairID;
};

struct CMD_S_Account
{
    BYTE cbChairID;
	DWORD dwPlayerScore;
	LONG lCellScore;
};

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* ����                                                                 */
/************************************************************************/
enum FishPath{
	FISH_PATH_BEGIN = 0,
	FISH_PATH_LINE , //ֱ��·��+����·��
	FISH_PATH_SPLINE , //����·����������
	FISH_PATH_SPECAIL , //С��
	FISH_PATH_SP_HAS_ID , //�й̶�ID��Ⱥ��
	FISH_PATH_SP_TEAM ,//С��
	FISH_PATH_SP_SPTTEAM ,//����С��
	FISH_PATH_SP_TYPE_BOMB ,//ͬ��ը��
	FISH_PATH_WATER_PLANT ,//ˮ��
	FISH_PATH_END
};


//�� �ͻ�����
struct CMD_C_Fish_Net_Object
{
	WORD wID;//ID
	BYTE cbType; //�������
};

struct CMD_S_Fish_Path
{
	BYTE cbType;//·������
	BYTE cbCount;//ʵ���������
	BYTE cbSeed;//���������
	CMD_C_Fish_Net_Object FishNetObject[MAX_FISH_SEND];//·���������Ϣ
};

struct CMD_S_Clear_Player
{
	BYTE cbChairID;
};

struct CMD_S_High_Score
{
	BYTE cbChairID;
};

struct CMD_C_Special_Fire
{
	BYTE cbType;//����δ��
	float fRote;
};

struct CMD_C_Cannon_Rotate
{
	float fRote;
};

struct CMD_S_Cannon_Rotate
{
	BYTE cbChairID;
	float fRote;
};

struct CMD_S_Laser_Bean_Success_dte
{
	BYTE cbCount;
	BYTE cbChairID;
	float fRote;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_IN_NET];
};

struct CMD_S_Cast_Net_Success_dte
{
	BYTE cbCount;
	BYTE cbChairID;
	DWORD dwPlayerScore;//��ҵ�ǰ����
	DWORD dwPlayerAddScore;//������ӵķ���
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_IN_CUCESS];
};

struct CMD_C_Cast_Net_dte
{
	BYTE cbCount;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_IN_NET];
};

struct CMD_C_HitTeam_dte
{
	BYTE cbCount;
	CMD_C_Fish_Net_Object hitFish;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_IN_TEAM];
};

struct CMD_S_HitTeam_Success_dte
{
	BYTE cbCount;
	BYTE cbChairID;
	DWORD dwPlayerScore;
	DWORD dwPlayerAddScore;//������ӵķ���
	CMD_C_Fish_Net_Object hitFish;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_IN_TEAM];
};

struct CMD_C_TpyeBomb_dte
{
	BYTE cbCount;
	CMD_C_Fish_Net_Object hitFish;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_BY_BOMB];
};

struct CMD_S_TypeBomb_Success_dte
{
	BYTE cbCount;
	BYTE cbChairID;
	DWORD dwPlayerScore;
	DWORD dwPlayerAddScore;//������ӵķ���
	CMD_C_Fish_Net_Object hitFish;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_BY_BOMB];
};

struct CMD_S_Fish_Group
{
	BYTE cbType;//Ⱥ������
	BYTE cbSeed;//�������
	float fPassTime;//���ͳ����ۼ�ʱ�䣬��λ��
	WORD wCount;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_IN_GROUP];//���ͳ��ֺ�ɱ������
};

enum RewordType
{
	eBigPrize = 0,
};

struct CMD_S_Reword_Notice
{
	BYTE cbChairID;
	BYTE cbType;
	DWORD dwValue;
	//WORD wMsgLen;
	//char szMsg[MAX_REWORD_MSG];
};

struct CMD_C_Hit_Screen_Bomb
{
	WORD wRatio;
	CMD_C_Fish_Net_Object hitFish;
};

struct CMD_S_Change_Energy
{
	BYTE cbChairID;
	BYTE cbValue;
};

struct CMD_C_HitBoat
{
	BYTE cbCount;
	WORD wRatio;//����
	CMD_C_Fish_Net_Object hitFish;
};

struct CMD_C_Fire_Lock
{
	CMD_C_Fish_Net_Object target;
};

struct CMD_S_Fire_Success_Lock
{
	BYTE cbChairID;
	CMD_C_Fish_Net_Object target;
};

struct CMD_S_AI_Enter
{
	BYTE cbChairID;
	BYTE cbSeed;
};

struct CMD_S_Get_Energy_Gun
{
	BYTE cbChairID;
	CMD_C_Fish_Net_Object target;
};

struct CMD_S_Time
{
	time_t time;
};

#pragma pack()

#endif