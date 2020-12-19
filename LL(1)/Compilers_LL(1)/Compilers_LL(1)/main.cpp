#include<iostream>
#include<string>
#include<iomanip>
#include"windows.h"
#include"Source.h"
using namespace std;

/*测试数据（输入需为标准LL(1)文法）
E->TA
A->+TA
A->~
T->FB
B->*FB
B->~
F->(E)
F->a

此项测试结果在书本P130

或

E->TK
K->+TK
K->~
T->FM
M->*FM
M->~
F->i
F->(E)
*/

int main() {
	Source source;
	cout << "请输入产生式：（‘~’表示空）：" << endl;

	//存储输入产生式，有格式要求，需要标准的LL(1)文法且已经化简
	string tem ="";
	int i = 0;

	//数据输入
	while (getline(cin,tem)) {
		int j;
		for (j = 0; j < tem.length(); ++j) {
			//输入产生式
			source.Gra_Set[i].P[j] = tem[j];

			//初始化产生式右侧的符号集合
			if (j >= 3 && !source.is_Exit(source.Gra_Set[i].P[j])) {
				source.Symbol.push_back(source.Gra_Set[i].P[j]);
			}
		}

		//长度初始化
		source.Gra_Set[i].len = tem.length();

		//如果当前为非终结符，压入非终结符向量中
		if (!source.is_non_Ter(source.Gra_Set[i].P[0])) {
			source.n_Ter.push_back(source.Gra_Set[i].P[0]);
			source.symbol.insert(pair<char, int>(source.Gra_Set[i].P[0], source.n_Ter.size() - 1));
		}

		//记录产生式数目
		source.count++;

		i++;
	}

	for (int j = 0; j < source.Symbol.size(); ++j) {
		if (!source.is_non_Ter(source.Symbol[j])) {
			source.Ter.push_back(source.Symbol[j]);
			source.symbol.insert(pair<char, int>(source.Symbol[j], 99 - (source.Ter.size() - 1)));
		}
	}
	source.Ter.push_back('#');

	source.symbol.insert(pair<char, int>('#', 99 - (source.Ter.size() - 1)));

	cout << "产生式的个数：" << source.count << endl<<endl;

	cout << "非终结符：" << endl;
	for (int j = 0; j < source.n_Ter.size(); ++j) {
		cout << source.n_Ter[j] << " ";
	}
	cout << endl<<endl;

	cout << "终结符：" << endl;
	for (int j = 0; j < source.Ter.size(); ++j) {
		cout << source.Ter[j] << " ";
	}
	cout << endl<<endl;

	//求first集合
	cout << "first集：";
	for (i = 0; i < source.n_Ter.size(); ++i) { //遍历所有的非终结符
		source.make_First(source.n_Ter[i]);
		int j = 0;
		cout << endl << source.n_Ter[i] << ":";
		while (j < source.First[i].len) {
			cout << source.First[i].set[j]<<" ";
			++j;
		}
	}
	cout << endl<<endl;

	//求follow集
	cout << "follow集：";
	source.make_Follow();
	for (i = 0; i < source.n_Ter.size(); ++i) {
		int j = 0;
		cout << endl << source.n_Ter[i] << ":";
		while (j < source.Follow[i].len) {
			cout << source.Follow[i].set[j]<<" ";
			++j;
		}
	}
	cout << endl << endl;

	source.make_A_Table();
	cout << "预测分析表：" << endl << endl;
	for (int j = 0; j < source.Ter.size(); ++j) {
		cout << "       " << source.Ter[j];
	}
	cout << endl;
	for (i = 0; i < source.n_Ter.size(); ++i) {
		cout << source.n_Ter[i] << ":   ";
		cout << setprecision(3) << fixed;
		for (int j = 0; j < source.Ter.size(); ++j) {
			string tmp = source.A_Table[source.symbol.at(source.n_Ter[i])][source.symbol.at(source.Ter[j])];
			if (tmp == "null") {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << std::left << setw(8) << tmp;	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN| FOREGROUND_RED| FOREGROUND_BLUE);
			}
			else {
				cout << std::left<<setw(8)<< tmp;	
			}
			
		}
		cout << endl;
	}

	return 0;
}