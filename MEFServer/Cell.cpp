#include "stdafx.h"
#include "Cell.h"


Cell::Cell()
{
}


Cell::~Cell()
{
}

//初始化棋子信息
void Cell::Init(int x, int y) {
	this->x = x;
	this->y = y;
	status = Status::s_empty;
}

void Cell::updateStatus(Status status) {
	this->status = status;
}

//比较棋子位置状态是否相同，两步同形检查
bool Cell::equal(Cell cell) {
	if (x == cell.x && y == cell.y && status == cell.status) {
		return true;
	}
	return false;
}

//获取棋子状态
Status Cell::getStatus() {
	return status;
}

//是否属于空格点
bool Cell::isEmpty() {
	return status == Status::s_empty;
}

//比较颜色，与指定状态比较是否相同
bool Cell::isSameColor(Status cp) {
	return status == cp;
}

