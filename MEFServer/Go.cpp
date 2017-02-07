#include "stdafx.h"
#include "Go.h"


Go::Go()
{
}


Go::~Go()
{
}

//开始棋局
void Go::startGo() {
	for (int i = 0; i <= SIZEOFBOARD; i++) {
		for (int j = 0; j <= SIZEOFBOARD; j++) {
			go[i][j].Init(i, j);
			path[i][j] = false;
		}
	}
	dropbackup.clear();//走步，影响，（颜色，为序号默认）
	die.clear();
	cpath.clear();
	count = 1;
};

//检查该点是否无气，并处理
void Go::isDie(int x, int y) {
	//是否在棋盘上（防止path数组越界。。。）
	if (path[x][y] || ans == false) {
		//已经走过（path数组标记），或已经发现气，取消检测
		return;
	}

	//为空应停止本次检查
	if (go[x][y].isEmpty()) {
		ans = false;
	}
	else {
		Cell ce = go[x][y];
		path[x][y] = true;//标记走过，带颜色的点
		cpath.push_back(ce);//记录每个标记(空不可标记，会影响后续检查)，检查完后清理标记
		if (go[x][y].isSameColor(checkcolor)) {//相同颜色，继续拓展
			die.push_back(ce);//这样应该是复制一个对象，非引用(后面的函数调用使用修改ce不影响die存放的信息) = =
			int check[4][2];
			prepareExt(x, y, check);
			for (int i = 0; i < 4; i++) {
				if (isOnBoard(check[i][0], check[i][1])) {
					isDie(check[i][0], check[i][1]);
				}
			}
		}
	}
}

//下棋后检查吃子
void Go::checkStarus(int x, int y) {

	int check[4][2];
	prepareExt(x, y, check);
	//此处4个检查应该互不影响
	for (int i = 0; i < 4; i++) {
		ans = true;//发现empty标记false；
		die.clear();//先删除上次状态
					//不在棋盘时，检查无意义
		if (isOnBoard(check[i][0], check[i][1])) {
			isDie(check[i][0], check[i][1]);
			chearPath();
			//检查玩之后，清理path待下次检查,返回die,ans标记是否有效
			if (ans) {
				for (Cell ce : die) {
					tp.push_back(ce);
				}
			}
		}

	}


}

//下棋，新落子
bool Go::Point(Cell tmp) {

	int x = tmp.x;
	int y = tmp.y;

	//是否已经有落子
	if (!go[x][y].isEmpty()) {
		return false;
	}

	tp.clear();
	tp.push_back(tmp);
	go[x][y] = tmp;

	Status recheck;
	if (count % 2) {
		recheck = s_black;
		checkcolor = s_write;
	}
	else {
		recheck = s_write;
		checkcolor = s_black;
	}

	checkStarus(x, y);//检查状态是否吃子
	if (tp.size() > 1) {
		//如果吃子：一步之内同形再现?
		vector<Cell> parent = dropbackup[count - 1];
		if (parent.size() == 2 && tp.size() == 2) {
			if (parent.at(0).equal(tp.at(1)) && parent.at(1).equal(tp.at(0))) {
				go[x][y].updateStatus(s_empty);
				return false;
			}
		}
		//确定生效~//清除死子
		for (unsigned int i = 1; i < tp.size(); i++) {
			CellUpdate(tp.at(i), s_empty);
		}

	}
	else {
		//没吃子时，检查自己是不是挂了
		ans = true;//发现empty标记false；
		die.clear();//先删除上次状态
		checkcolor = recheck;
		isDie(x, y);
		chearPath();
		if (ans) {//本次操作无效
				  //恢复go[x][y]...
			go[x][y].updateStatus(s_empty);
			return false;
		}
	}

	dropbackup.insert(pair<int, vector<Cell> >(count, tp));
	count++;
	return true;
	//清理死子//保存死子，保存记录，保存用于悔棋。
	//上一级需要处理死子GUI变化（PointCheck）
}

//返回当前手，及其吃的子，供GUI更新界面
vector<Cell> Go::PointCheck() {
	return dropbackup[count - 1];//最后一个为着点，不处理
}


//放弃本手，跳过
void Go::passStip() {
	tp.clear();
	dropbackup.insert(pair<int, vector<Cell> >(count, tp));
	count++;
}

//悔棋：返回走步，加走步引起的死子(更新棋盘)
//上层应做悔棋条件检查，是否为开始状态没有记录。。,
vector<Cell> Go::undo() {
	vector<Cell> re;
	if (count > 1) {
		count--;
		re = dropbackup[count];
		dropbackup.erase(count);

		if (re.size() == 0) {//处理弃权情况
			return re;
		}

		Status bb = re.at(0).status;
		CellUpdate(re.at(0), s_empty);

		if (re.size() > 1) {
			Status diecell;//吃的子，颜色肯定与其相反
			if (bb == s_black) {
				diecell = s_write;
			}
			else {
				diecell = s_black;
			}
			//re tp...复制粘贴导致的错误最可怕。。。
			for (unsigned int i = 1; i < re.size(); i++) {
				CellUpdate(re.at(i), diecell);
			}
		}
	}
	//上层需更新棋盘
	return re;
}

//准备向四个方向扩展
void Go::prepareExt(int x, int y, int check[4][2]) {

	check[0][0] = x - 1;
	check[0][1] = y;

	check[1][0] = x;
	check[1][1] = y - 1;

	check[2][0] = x + 1;
	check[2][1] = y;

	check[3][0] = x;
	check[3][1] = y + 1;

}

void Go::chearPath() {
	for (Cell ce : cpath) {
		path[ce.x][ce.y] = false;
	}
	cpath.clear();
}

void Go::CellUpdate(Cell ce, Status s) {
	go[ce.x][ce.y].updateStatus(s);
}



float Go::getBlackCell() {
	float sum = 0;
	for (int i = 0; i < SIZEOFBOARD; i++) {
		for (int j = 0; j < SIZEOFBOARD; j++) {
			flagEmpty[i][j] = false;//准备空格计数
			if (go[i][j].isSameColor(s_black)) {
				sum += 1;
			}
		}
	}
	return sum;
}
//着子完毕的棋局，采用数子法计算胜负。
//将双方死子清理出盘外后，对任意一方的活棋和活棋围住的点以子为单位进行计数。

void Go::isBlackEmpty(int x, int y) {

	if (flagEmpty[x][y]) {
		return;
	}

	if (go[x][y].isEmpty()) {

		flagEmpty[x][y] = true;
		tcountEmpty++;
		int check[4][2];
		prepareExt(x, y, check);

		for (int i = 0; i < 4; i++) {
			if (isOnBoard(check[i][0], check[i][1])) {
				isBlackEmpty(check[i][0], check[i][1]);
			}
		}

	}
	else {
		if (go[x][y].isSameColor(s_black)) {
			flagBlack = true;
		}
		else {
			flagWrite = true;
		}
	}

}

//
float Go::countCell() {
	float num = 0;
	num = getBlackCell();
	//计算空格

	for (int i = 0; i < SIZEOFBOARD; i++) {
		for (int j = 0; j < SIZEOFBOARD; j++) {
			if (go[i][j].isEmpty() && (!flagEmpty[i][j])) {
				flagBlack = flagWrite = false;
				tcountEmpty = 0;
				isBlackEmpty(i, j);

				if (flagBlack) {
					if (flagWrite) {
						//同时遇到黑白子，双方平分空格
						num = num + (tcountEmpty) / 2;
					}
					else {
						//只有黑子，则独占空格棋盘
						num = num + tcountEmpty;
					}
				}

			}
		}
	}
	return num;
}

//返回x,y棋子的颜色信息
Status Go::infGo(int x, int y) {
	return go[x][y].status;
}
