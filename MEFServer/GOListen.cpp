// GOListen.cpp : 实现文件
//

#include "stdafx.h"
#include "MEFServer.h"

#include "GOListen.h"
#include "MEFServerDlg.h"
#include "NetGoAction.h"


// GOListen
// GOListen 成员函数

//接受客户端连接后，开始游戏
void GOListen::OnAccept(int nErrorCode)
{
	//	//不再接收中间者进入游戏
	if (Gdig->game != true) {

		Gdig->hideIPInput();
		Accept(Gdig->gogame);
		Gdig->goaction = new NetGoAction(Gdig);
		Gdig->gogame.initAction(Gdig->goaction);
		Gdig->goaction->initGoData();
		Gdig->game = true;

	}
	CAsyncSocket::OnAccept(nErrorCode);
}

//设置窗口指针
void GOListen::initDig(CMEFServerDlg* dig) {
	Gdig = dig;
}
