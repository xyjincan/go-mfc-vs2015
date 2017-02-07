#pragma once
#include "GoAction.h"
class GOListen;
class GoClient;

class NetGoAction :
	public GoAction
{
public:

	NetGoAction(CMEFServerDlg*);
	~NetGoAction();
	//下棋
	void OnPoint(CPoint point);
	//悔棋
	void OnUndo();

	//新游戏
	void  OnNewGame();
	//跳过
	void  OnPass();

public:
	
	GoClient* gogame;//负责两端游戏交流

	
};

