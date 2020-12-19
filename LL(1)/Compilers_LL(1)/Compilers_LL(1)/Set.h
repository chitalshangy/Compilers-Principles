#pragma once
#include<iostream>
#include<cmath>

//定义first,follow集合的语法结构
class Set {
public:
	char set[100];//E的first/follow集的内容
	int len;//E的first/follow集的长度
	bool is;//E的first/follow是否已求出

	//构造与析构
	Set() {
		memset(set, 0, sizeof set);
		is = false;
	}
	~Set() {}
};