#include "stdafx.h"
#include <vector>
#include <string.h>
#include <assert.h>
#include "pbc.h"
#pragma comment(lib, "pbc.lib")

using namespace std;

struct pbc_env * env = pbc_new();

static void read_file (const char *filename , struct pbc_slice *slice) {
	FILE* f;
	fopen_s(&f, filename, "rb");
	if (f == NULL) {
		slice->buffer = NULL;
		slice->len = 0;
		return;
	}
	fseek(f,0,SEEK_END);
	slice->len = ftell(f);
	fseek(f,0,SEEK_SET);
	slice->buffer = malloc(slice->len);
	fread(slice->buffer, 1 , slice->len , f);
	fclose(f);
}

static void test(struct pbc_env *env) {
	struct pbc_wmessage* w_msg = pbc_wmessage_new(env, "S2c_login_info");

	//账号
	pbc_wmessage_string(w_msg, "list_id", "12345", 0);
	//角色数据1
	struct pbc_wmessage * user = pbc_wmessage_message(w_msg , "LoginUserList");
	pbc_wmessage_string(user , "uid", "111" , -1);
	pbc_wmessage_string(user , "name", "aaa" , -1);
	pbc_wmessage_integer(user , "photo", 12345 , 0);
	//角色数据2
	user = pbc_wmessage_message(w_msg , "list");
	pbc_wmessage_string(user , "uid", "222" , -1);
	pbc_wmessage_string(user , "name", "bbb" , -1);
	pbc_wmessage_integer(user , "photo", 54321 , 0);

	struct pbc_slice sl;
	char buffer[1024];
	sl.buffer = buffer, sl.len = 1024;
	pbc_wmessage_buffer(w_msg, &sl);

	struct pbc_rmessage* r_msg = NULL;
	r_msg = pbc_rmessage_new(env, "S2c_login_info", &sl);
	//读取账号ID
	string list_id = pbc_rmessage_string(r_msg, "list_id", 0 , NULL);
	printf("list_id:%s \n", &list_id);
	//读取角色数据
	int user_n = pbc_rmessage_size(r_msg, "list");
	for (int i=0;i<user_n;i++) {
		struct pbc_rmessage * p = pbc_rmessage_message(r_msg , "list", i);
		string uid = pbc_rmessage_string(p , "uid", i ,NULL);
		string name = pbc_rmessage_string(p, "name", i, NULL);
		int photo = pbc_rmessage_integer(p, "photo", i, NULL);
		printf("uid:%s, name:%s, photo:%d \n", &uid, &name, photo);
	}

	pbc_rmessage_delete(r_msg);
	pbc_wmessage_delete(w_msg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	struct pbc_slice slice;
	read_file("login.pb", &slice);
	int ret = pbc_register(env, &slice);
	assert(ret == 0);
	test(env);
	getchar();
	return 0;
}