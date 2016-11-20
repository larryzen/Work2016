#ifndef CMD_FISH_HEAD_FILE
#define CMD_FISH_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						404								//游戏 I D
#define GAME_PLAYER					6								//游戏人数
#define GAME_NAME					TEXT("金蟾捕鱼")				//游戏名字

#define AI_BULLET_MAX_COUNT			5	 								// 购买子弹最大数目

//////////////////////////////////////////////////////////////////////////
//服务器向客户端发送的数据
enum ServerCMD{
	SUB_S_BEGIN = 101,
	SUB_S_FIRE_LOCK_SUCCESS       ,//锁定发射成功
	SUB_S_SEND_FISH_PATH		  ,//散鱼
	SUB_S_SEND_FISH_PATH_GROUP    ,//阵形
	SUB_S_SEND_CLEAR_PLAYER		  ,//清空玩家
	SUB_S_SEND_HIGHT_SCORE        ,//全局分数通知
	SUB_S_SEND_CANNON_ROTATE	  ,//炮筒偏转
	SUB_S_SEND_GET_ENERGY_GUN     ,//获得能量炮
	SUB_S_CHANGE_SCENE            ,//改变场景
	SUB_S_BUY_BULLET_SUCCESS      ,//购买子弹成功
	SUB_S_BUY_BULLET_FAILED       ,//购买子弹失败
	SUB_S_REWORD_NOTIC	          ,//奖励公告
	SUB_S_ENERGY_CHANGE	          ,//能量改变
	SUB_S_FIRE_SUCCESS            ,//发射子弹成功
	SUB_S_FIRE_FAILED             ,//发射子弹失败
	SUB_S_CAST_NET_SUCCESS        ,//撒网成功
	SUB_S_CAST_NET_FAILED         ,//撒网失败
	SUB_S_CHANGE_CANNON           ,//改变炮筒
	SUB_S_ACCOUNT                 ,//账户开关，源码中只看到关闭操作
	SUB_S_HIT_TEAM_SUCCESS        ,//击中小组
	SUB_S_TPYE_BOBM_SUCCESS       ,//同类炸弹成功
	SUB_S_TASK                    ,//可能是任务或者锁定
	SUB_S_AI_ENTER                ,//特殊炮发射成功
	SUB_S_BOMB_SUCCESS            ,//炸弹成功
	SUB_S_BOMB_SUCCESS_FAILED     ,//炸弹失败，未使用
	SUB_S_SEND_TIME,
	SUB_S_END
};

#define OFFSET 51

//客户端向服务器发送数据
enum ClientCMD{
	SUB_C_BEGIN = SUB_S_END+OFFSET,
	SUB_C_FIRE                    ,//开火
	SUB_C_BUY_BULLET              ,//购买子弹
	SUB_C_CAST_NET                ,//撒网
	SUB_C_CHANGE_CANNON           ,//改变炮筒类型
	SUB_C_ACCOUNT                 ,//账户
	SUB_C_HIT_BOAT                ,//打中海盗船
	SUB_C_HIT_TEAM                ,//击中小组
	SUB_C_FIRE_LOCK			      ,//锁定开火
	SUB_C_SPECAIL_FIRE            ,//特殊炮
	SUB_C_CANNON_ROTATE			  ,//炮筒偏转
	SUB_C_TPYE_BOBM               ,//同类炸弹
	SUB_C_SCREEN_BOBM             ,//全屏炸弹
	SUB_C_GAME_END,					//游戏结束
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

//玩家
struct Role_Net_Object
{
    WORD wChairID;//椅子ID
    DWORD dwFishGold;//金币
    WORD wCannonType;  //炮类型
	WORD wCost;//消耗
	BYTE cbEnergy;//能量槽
	bool isAI;
	BYTE cbSeed;//机器人随机种子
};

/////////////////////////////////////////////////////////////////////////
struct CMD_S_StatusFree
{
    BYTE cbScene;
	DWORD dwOnceUpScore;//每次上分数值
	WORD wMaxFire;//最大炮数
    LONG lCellScore;//单位积分，用于计算换算
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
    WORD wChairID;//可改进，使用BYTE即可
    char szReason[32];
};

struct CMD_S_Bomb_Failed
{
    WORD wChairID;//可改进，使用BYTE即可
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
/* 新增                                                                 */
/************************************************************************/
enum FishPath{
	FISH_PATH_BEGIN = 0,
	FISH_PATH_LINE , //直线路径+曲线路径
	FISH_PATH_SPLINE , //曲线路径（废弃）
	FISH_PATH_SPECAIL , //小队
	FISH_PATH_SP_HAS_ID , //有固定ID的群组
	FISH_PATH_SP_TEAM ,//小队
	FISH_PATH_SP_SPTTEAM ,//特殊小队
	FISH_PATH_SP_TYPE_BOMB ,//同类炸弹
	FISH_PATH_WATER_PLANT ,//水草
	FISH_PATH_END
};


//鱼 客户端用
struct CMD_C_Fish_Net_Object
{
	WORD wID;//ID
	BYTE cbType; //鱼的类型
};

struct CMD_S_Fish_Path
{
	BYTE cbType;//路径类型
	BYTE cbCount;//实际鱼的数量
	BYTE cbSeed;//随机和种子
	CMD_C_Fish_Net_Object FishNetObject[MAX_FISH_SEND];//路径中鱼的信息
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
	BYTE cbType;//类型未定
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
	DWORD dwPlayerScore;//玩家当前分数
	DWORD dwPlayerAddScore;//玩家增加的分数
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
	DWORD dwPlayerAddScore;//玩家增加的分数
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
	DWORD dwPlayerAddScore;//玩家增加的分数
	CMD_C_Fish_Net_Object hitFish;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_FISH_BY_BOMB];
};

struct CMD_S_Fish_Group
{
	BYTE cbType;//群组类型
	BYTE cbSeed;//随机种子
	float fPassTime;//阵型出现累计时间，单位秒
	WORD wCount;
	CMD_C_Fish_Net_Object FishNetIDs[MAX_IN_GROUP];//阵型出现后被杀死的鱼
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
	WORD wRatio;//倍率
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