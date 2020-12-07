#pragma once
//定义first,follow集合的语法结构
class Set {
public:
	char set[100];
	int len;
	bool is;
	Set() {
		memset(set, 0, sizeof set);
		is = false;
	}
	~Set() {}
};