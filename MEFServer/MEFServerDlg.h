
// MEFServerDlg.h : 头文件
//


#pragma once
#include "GoAction.h"
#include "PCGoAction.h"
#include "NetGoAction.h"
#include "GOListen.h"
#include "GoClient.h"
#include "GUIGo.h"


// CMEFServerDlg 对话框
class CMEFServerDlg : public CDialogEx
{
// 构造
public:
	CMEFServerDlg(CWnd* pParent = NULL);	// 标准构造函数



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEFSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void GetHostAddress(CString &strIPAddr);

	GoAction* goaction;//单机，或联机游戏，游戏封装

	GOListen golisten;//服务端监听
	GoClient goclient;//客户端，进行游戏

	int sPort = 6666;
	GoClient gogame;//负责两端游戏交流

	GUIGo guiGo;

	//资源标识，
	bool work = false;

	//标识是否游戏Ing
	bool game = false;

	
	bool isOnePC = false;
	bool agree = false;


	//开始游戏
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//下棋
	afx_msg void Onundo();//悔棋
	afx_msg void Oncount();//点子
	afx_msg void Onnewgame();//新游戏
	afx_msg void Onpass();//跳过
	

	afx_msg void Onresetwin();//显示IP输入控件，（离开当前游戏）
	afx_msg void OnServer();//开启服务器
	afx_msg void OnClient();//打开客户端
	afx_msg void Ononepc();//单机人人对战


	void newGame();
	
	void hideIPInput();
	void resetBackGround();
	
	bool agreeExitGame(); //同意离开游戏时，处理游戏数据，关闭打开的资源
	void showIPInput();
	
	virtual void OnOK();
};
