#include "Source.h"

//创建First集合
void Source::make_First(char E) {

}

//创建Follow集合
void Source::make_Follow(char E) {

}

//判断是否为非终结符
bool Source::is_non_Ter(char E) {
	for (int i = 0; i < n_Ter.size(); ++i) {
		if (E == n_Ter[i]) {
			return true;
		}
	}
	return false;
}

//判断E是否已存在
bool Source::is_Symbol(char E) {
	for (int i = 0; i < Symbol.size(); ++i) {
		if (Symbol[i] == E) {
			return true;
		}
	}
	return false;
}

//判断E的First是否已求出
bool Source::is_first(char E) {
	int i = symbol.at(E);
	if (First[i].is) {
		return true;
	}
	return false;
}

//判断E的Follow是否已求出
bool Source::is_follow(char E) {
	int i = symbol.at(E);
	if (Follow[i].is) {
		return true;
	}
	return false;
}

//判断E是否能推出#
bool Source::is_Null(char E) {
	if (!First[symbol.at(E)].is) {
		make_First(E);
	}
	for (int i = 0; i < First[symbol.at(E)].len; ++i) {
		if (First[symbol.at(E)].set[i] == '#') {
			return true;
		}
	}
	return false;
}

//判断g的右侧是否存在非终结符
int Source::is_Have(Grammer g, char E) {
	for (int i = 3; i < g.len; ++i) {
		if (g.P[i] == E) {
			return i;
		}
	}
	return 0;
}

//将s1、s2中非终结符并入到s1中
Set Source::add(Set s1, Set s2) {
	if (s1.len == 0) {
		for (int i = 0; i < s2.len; ++i) {
			if (s2.set[i] == '#') {
				s2.set[i] = s2.set[s2.len - 1];
				s2.len--;
			}
		}
		return s2;
	}

	for (int i = 0; i < s2.len; ++i) {
		for (int j = 0; j < s1.len; ++j) {
			if (s2.set[i] == s1.set[j]) {
				break;
			}
			if (j == s1.len - 1 && s2.set[i] != '#') {
				s1.set[s1.len] = s2.set[i];
				s1.len++;
			}
		}
	}
	return s1;
}

//合并s1、s2
Set Source::add_Null(Set s1, Set s2) {
	if (s1.len == 0) {
		return s2;
	}
	for (int i = 0; i < s2.len; ++i)
		for (int j = 0; j < s1.len; ++j) {
			if (s2.set[i] == s1.set[j]) {
				break;
			}
			if (j == s1.len - 1) {
				s1.set[s1.len] = s2.set[i];
				s1.len++;
			}
		}
	return s1;
}

//构造预测分析表
void Source::make_A_Table() {
	
}

//构造函数
Source::Source() {
	n_Ter.clear();
	Ter.clear();
	Symbol.clear();
	symbol.clear();
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			A_Table[i][j] = "error";
		}
	}
}

//析构函数
Source::~Source() {}