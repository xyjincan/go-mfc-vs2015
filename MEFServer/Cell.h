#pragma once
#include "Base.cpp"

class Cell
{
public:
	int x;
	int y;
	Status status;

	Cell();

	void Init(int x, int y);//初始化棋子

	void updateStatus(Status status);//更新棋子状态
	
	bool equal(Cell cell);//比较棋子位置状态是否相同，两步同形检查

	Status getStatus();
	
	bool isEmpty();//是否属于空格点
	bool isSameColor(Status cp);//与指定状态比较是否相同

	
	~Cell();
};

