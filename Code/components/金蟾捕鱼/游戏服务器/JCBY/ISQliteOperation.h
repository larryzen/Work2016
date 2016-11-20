#pragma once
#include "YJAlgorithm.h"

//��������Ҫ��ȡ���������
struct SPlayerDBServerReadData
{
	int nowScore;//���Ŀǰ����
	int nowPutScore;//��ҵ����Ϸ���
	int putScore;//������Ϸ�ֵ
	int getScore;//������·�ֵ
	int shootingTime;//���������
	int maxScore;//�������ֵ
	int deadTime;//�����������
	int gunScore;//���ÿ�ڷ���
	int lostScore;//�����ʧ����
};



//���ݿ�����Ҫ����������Ϣ
struct SDataBasePlayerData
{
	int nowScore;//���Ŀǰ����
	int nowPutScore;//��ҵ����Ϸ���
	int putScore;//������Ϸ�ֵ
	int getScore;//������·�ֵ
	int shootingTime;//���������
	int maxScore;//�������ֵ
	int deadTime;//�����������
	int gunScore;//���ÿ�ڷ���
	int lostScore;//�����ʧ����
	int playerRegulation;//�ֶ��ٿ�Ӱ��ֵ
	char name[32];//�������
	unsigned long IP;//���IP
};

inline void SDataBasePlayerData_To_SPlayerDBServerReadData(const SDataBasePlayerData* dbpd_in, SPlayerDBServerReadData* pdbs_out)
{
	if(dbpd_in==NULL || pdbs_out==NULL)return;
	memcpy(pdbs_out, dbpd_in, sizeof(SPlayerDBServerReadData));
}

typedef void (*ErrorCallBack)(const char* msg);//������Ϣ�ص����������ݿ����ԭ��

/************************************************************************/
/* ���ݿ�����ӿڣ�������ֻ��ͨ���˽ӿڽ������ݿ����					*/
/* ���ݿ��߼���Ϊֻ�������жϣ������������κ��߼��ж�					*/
/* ���統�����������ʱ��������ݲ�������ֱ�ӷ���false					*/
/************************************************************************/
struct ISQLiteOperation
{
	//�ͷŽӿ�
	virtual void Release() = 0;

	//��ʼ���ӿڣ��������ʼ��ֱ�ӷ���true
	virtual bool InitDB() = 0;

	//��ȡ������Ϣ
	virtual bool ReadRoomData(const char* roomName, SRoomData* data_out) = 0;

	//���ô���ص�����
	virtual void SetErrorCallBack(ErrorCallBack callback) = 0;

	//��ȡ������ݿ���Ϣ(��ʵ�֣�����ʵ��)
	bool ReadPlayerData(DWORD uid, SPlayerDBServerReadData* data_out){
		SDataBasePlayerData dbpd;
		bool b=ReadDataBasePlayerData(uid, &dbpd);
		if(!b || data_out==NULL)return false;
		else SDataBasePlayerData_To_SPlayerDBServerReadData(&dbpd, data_out);
		return true;
	}

	//��ȡ�ֶ��ٿ�Ӱ��ֵ(��ʵ�֣�����ʵ��)
	bool ReadPlayerRegulation(DWORD uid, int* data_out){
		if(data_out==NULL)return false;
		SDataBasePlayerData dbpd;
		bool b=ReadDataBasePlayerData(uid, &dbpd);
		if(!b)return false;
		else *data_out = dbpd.playerRegulation;
		return true;
	}

	//���������Ϣ�����ݿ�(��ʵ�֣�����ʵ��)
	bool UpdatePlayerData(DWORD uid, const SPlayerDBServerReadData* data_in){
		if(data_in == NULL)return false;
		int playerRegulation=0;
		if(!ReadPlayerRegulation(uid, &playerRegulation))return false;
		SDataBasePlayerData dbpd;
		memcpy(&dbpd, data_in, sizeof(SPlayerDBServerReadData));
		dbpd.playerRegulation= playerRegulation;
		return UpdateDataBasePlayerData(uid, &dbpd);
	}

	//��������Ϣ�����ݿ�(��ʵ�֣�����ʵ��)
	bool AddPlayerData(DWORD uid, const SPlayerDBServerReadData* data_in){
		if(data_in == NULL)return false;
		SDataBasePlayerData dbpd;
		memcpy(&dbpd, data_in, sizeof(SPlayerDBServerReadData));
		dbpd.playerRegulation = 0;
		return AddDataBasePlayerData(uid, &dbpd);
	}


protected:
	//������ݿ������Ϣ
	virtual bool AddDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in) = 0;

	//��ȡ���ݿ������Ϣ
	virtual bool ReadDataBasePlayerData(DWORD uid, SDataBasePlayerData* data_out) = 0;

	//�������ݿ������Ϣ
	virtual bool UpdateDataBasePlayerData(DWORD uid, const SDataBasePlayerData* data_in) = 0;
};

extern "C" 
{
	ISQLiteOperation* CreateISQLiteOperation(void); //�������ݿ�ӿں���
	void SetSQLiteErrorCallBack(ErrorCallBack cb);
}