#include "stdafx.h"
#include "MEFServerDlg.h"
#include "GoAction.h"
#include "NetGoAction.h"

//联机游戏类


NetGoAction::NetGoAction(CMEFServerDlg * winDig) :GoAction(winDig)
{
	this->winDig = winDig;
}

NetGoAction::~NetGoAction()
{
}

void NetGoAction::OnPoint(CPoint point)
{

	if (!isMe) {
		return;
	}

	turnBW();
	//下棋
	if (guiGo->clickGo(winDig->GetDC(), point)) {

		int px = guiGo->px;
		int py = guiGo->py;
		CString stx, sty;
		stx.Format(_T("%d"), px);
		sty.Format(_T("%d"), py);
		CString send = stx + _T(":") + sty;
		winDig->gogame.Send(send, send.GetAllocLength()*2);
	}else {
		//走棋非法时，保留下棋权利 ps：bug（2016/5/6）
		turnBW();
	}
}

void NetGoAction::OnUndo()
{
	if (guiGo->count == 1) {
		return;
	}
	agree = true;
	CString send = _T("-1:-1");
	winDig->gogame.Send(send, send.GetAllocLength() * 2);
}

void NetGoAction::OnNewGame()
{
	agree = true;
	isMe = true;
	CString send = _T("-1:-4");
	winDig->gogame.Send(send, send.GetAllocLength() * 2);
}

void NetGoAction::OnPass()
{
	if (!isMe) {
		return;
	}
	turnBW();
	guiGo->passStip(winDig->GetDC());
	CString send = _T("-1:-2");
	winDig->gogame.Send(send, send.GetAllocLength() * 2);
}


