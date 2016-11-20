#pragma once

//加密和解密，其实这两个函数完全相同的，简单起见嘛，另外我居然允许密码为空，也是简单起见
void JiangEncode(unsigned char *pBuff, int iBuffLen, char *pKey=NULL, int iKeyLen=0);
void JiangDecode(unsigned char *pBuff, int iBuffLen, char *pKey=NULL, int iKeyLen=0);