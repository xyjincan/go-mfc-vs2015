#pragma once
#include <map>
#include <vector>
#include "Cell.h"
using namespace std;

class Go
{
public:
	Go();
	~Go();

	void startGo();//清理旧信息


	Cell go[20][20];
	map<int, vector<Cell> > dropbackup;//用于记录所有的历史纪录，（悔棋信息会丢弃）

	int count = 1;//棋局计数,与颜色相关


	Status checkcolor;//要递归检查气的颜色
	vector<Cell> tp;//检查时记录所有没气的棋子
	vector<Cell> die;//临时记录（落子时独立检查四方）
	bool ans;  //标记是否继续检查,以及确定die是否有效
	bool path[20][20]; //临时标记走过的路，防止重复
	vector<Cell> cpath;//临时记录所有走过路径



	void isDie(int x, int y);//判断x，y点区域棋子是否无气，死掉 更新 vector<Cell> tp
	
	void checkStarus(int x, int y);//x y 点落子后，检查

	bool Point(Cell tmp);//落子，tmp  含xy坐标

	//返回当前手，及其吃的子，供GUI更新界面
	vector<Cell> PointCheck();

	//放弃本手，
	void passStip();
	//上层应做悔棋条件检查，是否为开始状态没有记录。。,返回走步，加走步引起的死子(更新棋盘)
	vector<Cell> undo();

	//准备向四个方向扩展
	void prepareExt(int x, int y, int check[4][2]);

	void chearPath();

	void CellUpdate(Cell ce, Status s);//更新显示提示信息，next 颜色

	
public:
	//计算分数，及公共临时变量
	bool flagBlack;
	bool flagWrite;
	bool flagEmpty[20][20]; //临时标记数过的空格，防止重复计数
	float tcountEmpty;

	float getBlackCell();
	//着子完毕的棋局，采用数子法计算胜负。
	//将双方死子清理出盘外后，对任意一方的活棋和活棋围住的点以子为单位进行计数。
	void isBlackEmpty(int x, int y);

	float countCell();//获取黑子数目


	//返回x,y棋子的颜色信息
	Status infGo(int x, int y);

};

