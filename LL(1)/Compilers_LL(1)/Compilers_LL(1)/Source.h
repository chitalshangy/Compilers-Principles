#pragma once
#include"Grammer.h"
#include"Set.h"
#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<map>
using namespace std;

class Source
{
public:
	//定义原始文法的产生式集合
	Grammer Gra_Set[200];//grammarElement  gramOldSet[200];
	//定义first,follow集合的语法结构
	Set First[100], Follow[100];

	//定义预测分析表
	string A_Table[100][100];//string M[100][100];

	//非终结符
	vector<char>n_Ter;//vector <char> non_ter;

	//终结符
	vector<char>Ter;//vector <char> terSymbol;

	//产生式右侧所有符号
	vector<char>Symbol;
	map<char, int>symbol;

	//产生式个数
	int count;

	//创建First集合
	void make_First(char E);

	//创建Follow集合
	void make_Follow(char E);

	//判断是否为非终结符
	bool is_non_Ter(char E);

	//判断E是否已存在
	bool is_Symbol(char E);

	//判断E的First是否已求出
	bool is_first(char E);

	//判断E的Follow是否已求出
	bool is_follow(char E);

	//判断E是否能推出#
	bool is_Null(char E);

	//判断g的右侧是否存在非终结符
	int is_Have(Grammer g, char E);

	//将s1、s2中非终结符并入到s1中
	Set add(Set s1, Set s2);

	//合并s1、s2
	Set add_Null(Set s1, Set s2);

	//构造预测分析表
	void make_A_Table();

	//构造函数
	Source();

	//析构函数
	~Source();
};

