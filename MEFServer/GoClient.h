#pragma once
//#include "GoAction.h"

class CMEFServerDlg;
class GoAction;

// GoClient 命令目标
//客户端类，负责下棋通信交互

class GoClient : public CAsyncSocket
{
public:
	GoClient();
	virtual ~GoClient();

	virtual void OnReceive(int nErrorCode);

public:

	int px;
	int py;

	bool isOk = false;

	GoAction* goaction;
	//设置窗口指针，更新棋局GUI
	void initAction(GoAction* dig);

	void play();

	virtual void OnConnect(int nErrorCode);

	
};


