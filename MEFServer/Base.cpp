#include "stdafx.h"

enum cellcolor { b, w };//棋子颜色信息变量
enum Status { s_empty, s_black, s_write };//棋盘三种状态

//棋盘大小
static int SIZEOFBOARD = 19;

//判断棋子是否在棋盘上
static bool isOnBoard(int x, int y) {
	if (x >= 0 && y >= 0) {
		if (x < SIZEOFBOARD && y < SIZEOFBOARD) {
			return true;
		}
	}
	return false;
}
