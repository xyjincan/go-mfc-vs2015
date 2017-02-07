#pragma once
#include "stdafx.h"
#include "Go.h"


//维护棋盘界面，供界面事件调用，内部包含围棋内部数据。

class GUIGo
{
public:

	Go go;//围棋内部逻辑数据
	int count = 1;//计数器

	int alen = 19; //围棋，19*19
	int size = 24;//单格尺寸
	int cell_h = 12;//棋子半径
	int origin = 30;//原点坐标（x=y）
	
	int extsize = 4;//棋盘上点半径
	int extlen = 6;////19*19围棋点距离6
	int ext;//特别点之间距离

	int sizeofbroad;//棋盘长度
	int broadrange;//棋盘坐标范围
	int base;//棋盘中心点界面坐标


	//几个资源画刷
	CBrush bblack;
	CBrush bwrite;
	CBrush background;

	//画笔，处理黑线残留
	CPen penblack;
	CPen penwrite;
	CPen clearcell;


	GUIGo();
	~GUIGo();

	void setSize(int newSize);//程序窗口大小改变时，重设棋子大小信息
	
	//初始化 or 新开局
	void initGo(CDC* pDC);

	//当游戏时，整个界面需要重绘
	void updateInf(CDC* pDC);

	//悔棋
	void upGo(CDC* pDC);

	//本步，弃权
	void passStip(CDC* pDC);

	//新下棋，检查是否有效//有效传给对方
	int px=-1;
	int py=-1;

	bool clickGo(CDC* pDC, CPoint point);

	bool oppositeGo(CDC* pDC,int ax ,int ay);

	void addCell(CDC* pDC, Cell cell, cellcolor cc);

	void removeCell(CDC* pDC, Cell cell);
	//清除某个点的棋子
	//更新棋盘时，检查是否为关键点（）
	void addBroad(CDC* pDC, int ax, int ay);

	//初始化棋盘
	void initBoard(CDC* pDC);

	//显示提示信息
	void updateTips(CDC* pDC);

	//显示当前局，统计结果
	void showCountBlack(CDC* pDC);

	//显示计数
	void showLog(CDC* pDC);


};

