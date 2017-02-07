#pragma once
#include "GoAction.h"


class PCGoAction :
	public GoAction
{
public:
	
	PCGoAction(CMEFServerDlg*);
	~PCGoAction();

	//下棋
	void OnPoint(CPoint point);
	//悔棋
	void OnUndo();
	
	//新游戏
	void  OnNewGame();
	//跳过
	void  OnPass();
};

