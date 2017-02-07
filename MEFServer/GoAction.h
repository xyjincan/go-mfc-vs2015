#pragma once
#include "stdafx.h"
//#include "GOListen.h"
//#include "GoClient.h"
#include "GUIGo.h"
//#include "MEFServerDlg.h"
class CMEFServerDlg;


//抽象下棋类，为支持，单机，联网下围棋
class GoAction
{
public:
	GoAction(CMEFServerDlg*);
	~GoAction();

public:

	CMEFServerDlg* winDig;//对话框指针

	int sPort = 6666;

	GUIGo* guiGo;//bug 该对象保存了棋子大小信息，转移时，不应丢失


	//标识是否游戏Ing
	bool game = false;
	bool agree = false;
	//开始游戏

	//下棋
	virtual void OnPoint(CPoint);
	//悔棋
	virtual void OnUndo();
	//查看状态
	virtual void  OnCount();
	//新游戏
	virtual void  OnNewGame();
	//跳过
	virtual void  OnPass();


	void initGoData();

public:
	//联机时用于检查是否该自己下
	bool isMe = true;
	void turnBW();

};

