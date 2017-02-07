#include "stdafx.h"
#include "MEFServerDlg.h"
#include "GoAction.h"


//父类 用于
//单机围棋
//联机围棋
//继承接口

GoAction::GoAction(CMEFServerDlg* winDig)
{
	this->winDig = winDig;
	this->guiGo = &winDig->guiGo;
}


GoAction::~GoAction()
{
}


void GoAction::OnPoint(CPoint point)
{
}

void GoAction::OnUndo()
{
}

void GoAction::OnNewGame()
{

}

void GoAction::OnPass()
{
}

//更新辅助信息
void GoAction::OnCount()
{
	guiGo->showCountBlack(winDig->GetDC());
}
//设置新围棋
void GoAction::initGoData()
{	
	guiGo->initGo(winDig->GetDC());
}

//调整围棋 黑落子或白子落 状态
void GoAction::turnBW()
{
	if (isMe) {
		isMe = false;
	}
	else {
		isMe = true;
	}
}
