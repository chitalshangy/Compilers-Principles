#include "Source.h"

//创建First集合
void Source::make_First(char E) {
	int i = symbol.at(E);
	if (First[i].is)return;
	First[i].len = 0;
	//遍历所有产生式
	for (int j = 0; j < count; ++j) {
		//查找该非终结符所在产生式
		if (Gra_Set[j].P[0] == E) {
			//指向产生式右边字符的指针
			int x = 3;

			//终结符直接放入first集合
			if (!is_non_Ter(Gra_Set[j].P[x])) {
				Set n;
				n.is = true;
				n.len = 1;
				n.set[0] = Gra_Set[j].P[x];
				First[i] = add_Null(First[i],n);
			}

			//非终结符
			if (is_non_Ter(Gra_Set[j].P[x])) {
				//和产生式左边的符号相等，则不处理
				if (Gra_Set[j].P[x] == E) {
					continue;
				}
				//1、第一种情况
				//判断其first集合是否已经求出，已求出则直接加入并入左边符号的first集合
				if (is_first(Gra_Set[j].P[x])) {
					First[i] = add(First[i], First[symbol.at(Gra_Set[j].P[x])]);
				}
				//否则求其first集合，并入
				else {
					make_First(Gra_Set[j].P[x]);
					First[i] = add(First[i], First[symbol.at(Gra_Set[j].P[x])]);
				}

				//2、第二种情况，如果可以推出~
				while (is_Null(Gra_Set[j].P[x])) {
					x++;
					//3、第三种情况，如果可以推出~并且第二种情况的遍历完结，将~并入
					if (x >= Gra_Set[j].len) {
						Set n;
						n.is = true;
						n.len = 1;
						n.set[0] = '~';
						First[i] = add_Null(First[i],n);
						break;
					}
					//是终结符，直接并入
					if (!is_non_Ter(Gra_Set[j].P[x])){
						Set n;
						n.is=true;
						n.len=1;
						n.set[0]=Gra_Set[j].P[x];
						First[i]=add_Null(First[i],n);
						break;
					}
					//不是终结符，需要排除符号first集中的~
					if (is_first(Gra_Set[j].P[x])) {
						First[i] = add(First[i], First[symbol.at(Gra_Set[j].P[x])]);
					}
					else {
						make_First(Gra_Set[j].P[x]);
						First[i] = add(First[i], First[symbol.at(Gra_Set[j].P[x])]);
					}
				}
			}
		}
	}
	First[i].is = true;
}

//创建Follow集合
void Source::make_Follow() {
	bool flag = true;

	//首先初始化为空
	for (int i = 0; i < n_Ter.size(); ++i) {
		Follow[i].len = 0;
	}

	while (flag) {
		flag = false;

		//遍历每一个非终结符
		for (int i = 0; i < n_Ter.size(); ++i) {
			char E = n_Ter[i];

			//如果当前符号为输入文法的开始符号，将#加入其follow集合
			if (E == Gra_Set[0].P[0]) {
				Set n;
				n.is = true;
				n.len = 1;
				n.set[0] = '#';
				Follow[i] = add_Null(Follow[i], n);
			}

			for (int j = 0; j < count; ++j) { 

				//找出右边含有E的产生式
				if (is_Have(Gra_Set[j], E) != 0) {
					int x = is_Have(Gra_Set[j], E);

					//如果E在产生式的末尾：A->...E；
					if (x >= Gra_Set[j].len - 1) { 
						//如果产生式j左边符号的follow已经求出，则并入E的follow：A->...E，follow(E)=follow(E)+follow(E)；
						if (is_follow(Gra_Set[j].P[0])) { 
							//before用来判断E的follow集合是否已经全部求出
							int before = Follow[i].len;
							Follow[i] = add(Follow[i], Follow[symbol.at(Gra_Set[j].P[0])]);
							if (before != Follow[i].len) {
								flag = true;
							}
						}
						else {
							flag = true;
						}
					}
					//如果E不在产生式末尾：A->...E...；
					else {
						Set str;//存储E后面的first集合
						str.len = 0;
						while (x < Gra_Set[j].len - 1) {
							x++;
							//E后面的如果是非终结符，follow(E)=follow(E)+first(...)
							if (is_non_Ter(Gra_Set[j].P[x])) {
								if (is_first(Gra_Set[j].P[x])) { 
									str = add(str, First[symbol.at(Gra_Set[j].P[x])]);
								}
								else {
									make_First(Gra_Set[j].P[x]);
									str = add(str, First[symbol.at(Gra_Set[j].P[x])]);
								}
								//E后面的非终结符可以推出~，follow(E)=follow(E)+~
								if (is_Null(Gra_Set[j].P[x])) {
									if (x == Gra_Set[j].len - 1) {
										Set n;
										n.is = true;
										n.len = 1;
										n.set[0] = '~';
										str = add_Null(str, n);
									}
									continue;
								}
								else {	
									break;
								}
							}
							//如果是终结符，直接并入
							else { 
								Set n;
								n.is = true;
								n.len = 1;
								n.set[0] = Gra_Set[j].P[x];
								str = add_Null(str, n);
								break;
							}
						}
						//合并入E的follow集中
						int before = Follow[i].len;
						Follow[i] = add(Follow[i], str);
						for (int z = 0; z < str.len; ++z) {
							if (str.set[z] == '~') {
								Follow[i] = add(Follow[i], Follow[symbol.at(Gra_Set[j].P[0])]);
							}
						}
						//同样也是判断E是否需要继续执行follow生成算法
						if (before != Follow[i].len) {
							flag = true;
						}
					}
				}
			}
			Follow[i].is = true;
		}
	}
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
bool Source::is_Exit(char E) {
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

//判断E是否能推出~
bool Source::is_Null(char E) {
	if (!First[symbol.at(E)].is) {
		make_First(E);
	}
	for (int i = 0; i < First[symbol.at(E)].len; ++i) {
		if (First[symbol.at(E)].set[i] == '~') {
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
			if (s2.set[i] == '~') {
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
			if (j == s1.len - 1 && s2.set[i] != '~') {
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
	for (int i = 0; i < count; ++i) { //遍历所有的产生式
		int x = 3;
		//产生式右边串的first集合
		Set tmp;
		tmp.len = 0;
		while (x < Gra_Set[i].len) {
			//该字符是非终结符
			if (is_non_Ter(Gra_Set[i].P[x])) { 
				//该终结符的非空字符并入first
				if (is_first(Gra_Set[i].P[x])) {
					tmp = add(tmp, First[symbol.at(Gra_Set[i].P[x])]);//其first集合的非~元素交集并入
				}
				else {
					cout << "error!!!" << endl;
				}
				if (is_Null(Gra_Set[i].P[x])) {//该非终结符能推出~
					if (x == Gra_Set[i].len - 1) {//所有字符都能推出~
						Set n;
						n.is = true;
						n.len = 1;
						n.set[0] = '~';
						tmp = add_Null(tmp, n);//将~加入first集
					}
					x++;
					continue;//继续循环
				}
				else {	//该非终结符不能推出~
					break;//停止循环
				}
			}
			else { //字符是终结符
				Set n;
				n.is = true;
				n.len = 1;
				n.set[0] = Gra_Set[i].P[x];
				tmp = add_Null(tmp, n);//将该终结符加入first集
				break;
			}
			x++;
		}
		
		//开始建表
		for (int j = 0; j < tmp.len; ++j) {
			if (tmp.set[j] == '~') {
				for (int z = 0; z < Follow[symbol.at(Gra_Set[i].P[0])].len; ++z) {
					int x = symbol.at(Gra_Set[i].P[0]);
					int y = symbol.at(Follow[symbol.at(Gra_Set[i].P[0])].set[z]);
					A_Table[x][y] = Gra_Set[i].P;
				}
			}
			else {
				int x = symbol.at(Gra_Set[i].P[0]);
				int y = symbol.at(tmp.set[j]);
				A_Table[x][y] = Gra_Set[i].P;
			}
		}

	}
}

//构造函数
Source::Source() {
	n_Ter.clear();
	Ter.clear();
	Symbol.clear();
	symbol.clear();
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			A_Table[i][j] = "null";
		}
	}
}

//析构函数
Source::~Source() {}