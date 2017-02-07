// GoClient.cpp : 实现文件

#include "stdafx.h"
#include "MEFServerDlg.h"
#include "NetGoAction.h"
#include "GoAction.h"
#include "GoClient.h"

// GoClient

GoClient::GoClient()
{
}

GoClient::~GoClient()
{
}


// GoClient 成员函数

//设置本客户端围棋对象
void GoClient::initAction(GoAction * dig)
{
	goaction = dig;
}


//解析数据，应该接收的格式x:y[:*]//max:250(char)
void GoClient::OnReceive(int nErrorCode)
{
	// 收到信息，解析
	char szTemp[250];
	ZeroMemory(szTemp, 250);
	int n = Receive(szTemp, 250);
	szTemp[n] = '\0';

	CString sTemp;
	sTemp.Format(_T("%s"), szTemp);
	//AfxMessageBox(sTemp);

	CString spx;
	CString spy;
	//CString ext;//may 拓展信息传递
	AfxExtractSubString(spx, (LPCTSTR)sTemp, 0, ':');
	AfxExtractSubString(spy, (LPCTSTR)sTemp, 1, ':');

	px = _wtoi(spx);
	py = _wtoi(spy);

	//处理棋局	
	play();

	CAsyncSocket::OnReceive(nErrorCode);
}

void GoClient::play() {

	if (px >= 0) {
		//落子状态传递
		//落子异常时可以检查连接合法性
		CDC* pDC = goaction->winDig->GetDC();
		goaction->guiGo->oppositeGo(goaction->winDig->GetDC(), px, py);
		goaction->isMe = true;//第一次任意方落子

	}
	else {
	
		switch (py)
		{
		case -1:
			//悔棋确认，可能导致BUG？，当一方请求a,另一方同时请求b，单一agree标识会有问题（2016/4/20）
			//
			if (!goaction->agree) {

				if (::MessageBox(goaction->winDig->m_hWnd, _T("对手请求悔棋,是否同意\n"), _T("悔棋"), MB_OKCANCEL) != IDOK)
				{
					//回信确认拒绝
					CString send = _T("-1:0");
					goaction->winDig->gogame.Send(send, send.GetLength() * 2);
					break;

				}
				//回信确认同意
				CString send = _T("-1:-1");

				goaction->winDig->gogame.Send(send, send.GetLength() * 2);
			}
			goaction->guiGo->upGo(goaction->winDig->GetDC());
			goaction->turnBW();
			goaction->agree = false;
			break;

		case -2:
			goaction->guiGo->passStip(goaction->winDig->GetDC());
			goaction->isMe = true;
			break;
		case -3:
			break;
		case -4:
			//新游戏确认//发起方无需确认
			if (!goaction->agree) {
				if (::MessageBox(goaction->winDig->m_hWnd, _T("对手请求新游戏,是否同意\n"), _T("新游戏"), MB_OKCANCEL) != IDOK)
				{
					//回信确认拒绝
					CString send = _T("-1:0");
					goaction->winDig->gogame.Send(send, send.GetLength() * 2);
					break;
				}
				else {
					//回信确认同意
					CString send = _T("-1:-4");
					goaction->winDig->gogame.Send(send, send.GetLength() * 2);//重发新游戏信号
					goaction->guiGo->initGo(goaction->winDig->GetDC());
				}
			}
			else {
				goaction->guiGo->initGo(goaction->winDig->GetDC());
			}
			goaction->agree = false;
			break;
		case -5:
			//检查连接
			if (!isOk) {
				CString send = _T("-1:-5");
				goaction->winDig->gogame.Send(send, send.GetLength() * 2);

			}
			isOk = true;
			goaction->OnNewGame();
			//开始游戏
			break;
		case 0:
			//处理拒绝。。
			goaction->agree = false;
			break;
		default:
			break;
		}

	}

}


//客户端连接服务端事件
void GoClient::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode) {
		isOk = false;
	}
	else {
		//连接成功，开始游戏
		goaction->winDig->hideIPInput();
		goaction->initGoData();
		goaction->winDig->gogame.initAction(goaction);
		goaction->winDig->game = true;
		isOk = true;
	}
	CAsyncSocket::OnConnect(nErrorCode);
}
