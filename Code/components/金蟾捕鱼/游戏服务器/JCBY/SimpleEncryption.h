#pragma once

//���ܺͽ��ܣ���ʵ������������ȫ��ͬ�ģ������������Ҿ�Ȼ��������Ϊ�գ�Ҳ�Ǽ����
void JiangEncode(unsigned char *pBuff, int iBuffLen, char *pKey=NULL, int iKeyLen=0);
void JiangDecode(unsigned char *pBuff, int iBuffLen, char *pKey=NULL, int iKeyLen=0);