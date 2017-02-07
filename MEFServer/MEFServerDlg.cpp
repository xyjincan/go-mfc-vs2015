
// MEFServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MEFServer.h"
#include "MEFServerDlg.h"
#include "afxdialogex.h"


//class GoAction;
//class PCGoAction;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMEFServerDlg 对话框



CMEFServerDlg::CMEFServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEFSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMEFServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMEFServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_Server, &CMEFServerDlg::OnServer)
	ON_COMMAND(ID_Client, &CMEFServerDlg::OnClient)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_undo, &CMEFServerDlg::Onundo)
	ON_COMMAND(ID_count, &CMEFServerDlg::Oncount)
	ON_COMMAND(ID_newgame, &CMEFServerDlg::Onnewgame)
	ON_COMMAND(ID_pass, &CMEFServerDlg::Onpass)
	ON_COMMAND(ID_onePC, &CMEFServerDlg::Ononepc)
	ON_COMMAND(ID_resetWin, &CMEFServerDlg::Onresetwin)
END_MESSAGE_MAP()


// CMEFServerDlg 消息处理程序

BOOL CMEFServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//on init program
	GetDlgItem(ID_ServerIp)->SetWindowText(_T("127.0.0.1"));
	goaction = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMEFServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{

		if (nID == 61536) {//关闭窗口
			if (::MessageBox(m_hWnd, _T("确定要离开？\n"), _T("离开游戏"), MB_OKCANCEL) != IDOK)//可能点确定，点取消，还可能点X关掉窗口
			{
				return;
			}
			else {
				if (goaction != NULL) {//服务器未连接客户端时，此为null
					delete goaction; //释放空间
					goaction = NULL;
				}
			}
		
		}

		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。


//MFC程序窗口重绘函数

void CMEFServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//迅速拖动窗口，会导致重绘事件发生
		if (game) {
			//处理游戏界面重绘问题
			resetBackGround();
			CDC* pDC = GetDC();
			guiGo.updateInf(GetDC());
		}
		else {
			showIPInput();
		}

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMEFServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//调整窗口大小时，重绘棋盘
void CMEFServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	int min = cx;
	if (cy < min) {
		min = cy;
	}
	int GoBoardsize = min - guiGo.origin;
	int cell = GoBoardsize / SIZEOFBOARD;
	guiGo.setSize(cell);

	//没有开始游戏时不能画棋盘，，
	if (!game) {
		return;
	}
	
	resetBackGround();
	guiGo.updateInf(GetDC());

}



void CMEFServerDlg::OnServer()
{

	if (work && !agreeExitGame()) {
		// sure ?
		return;
	}
	goaction = NULL;
	CString ips;
	GetHostAddress(ips);
	GetDlgItem(ID_ServerIp)->SetWindowText(ips);
	golisten.Close();

	BOOL mport = golisten.Create(sPort, SOCK_STREAM);

	if (mport) {
		//监听端口成功
		golisten.Listen(1);
		golisten.initDig(this);
		work = true;
		showIPInput();
	}
	else {
		//监听端口失败

		golisten.Close();
		MessageBox(_T("对不起，打开服务端口失败，您可以尝试作为客户端。"));
	}

	// TODO: 在此添加命令处理程序代码
}


void CMEFServerDlg::OnClient()
{

	if (work && !agreeExitGame()) {
		// sure ?
		return;
	}
	CString sip;
	GetDlgItem(ID_ServerIp)->GetWindowText(sip);//获取服务端ip地址
	// TODO: 在此添加命令处理程序代码
	gogame.Close();
	gogame.Create();
	gogame.Connect(sip, sPort);

	goaction = new NetGoAction(this);
	gogame.initAction(goaction);
	work = true;
	//连接失败时清理状态
}




void CMEFServerDlg::resetBackGround()
{
	CRect   rect;
	CPaintDC   dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(241, 224, 157));   //设置为背景，类似木头颜色
}


void CMEFServerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 650;   //x宽度  
	lpMMI->ptMinTrackSize.y = 550;   //y高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


//鼠标点击下棋
void CMEFServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//下棋
	if (!game) {
		return;
	}
	goaction->OnPoint(point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMEFServerDlg::Onundo()
{
	if (!game) {
		return;
	}
	goaction->OnUndo();
	//是否有棋可悔
}


void CMEFServerDlg::Oncount()
{
	if (!game) {
		return;
	}
	//自己查看游戏黑白子数量
	goaction->OnCount();
}


void CMEFServerDlg::Onnewgame()
{
	if (!game) {
		return;
	}
	resetBackGround();
	goaction->OnNewGame();
	//获取对方确认，然后新游戏

}


void CMEFServerDlg::Onpass()
{
	if (!game) {
		return;
	}
	goaction->OnPass();
}

//准备开始游戏，
void CMEFServerDlg::hideIPInput()
{
	GetDlgItem(ID_ServerIp)->ShowWindow(FALSE); //隐藏该ip控件
	GetDlgItem(ID_FILE_OPEN)->ShowWindow(FALSE); //隐藏ip提示控件
	resetBackGround();
}

//开始新游戏
void CMEFServerDlg::newGame()
{
	resetBackGround();
	guiGo.initGo(GetDC());

}




//开始单机游戏PK
void CMEFServerDlg::Ononepc()
{
	if (game && !agreeExitGame()) {
		// sure ?
		return;
	}

	hideIPInput();
	game = true;
	work = true;
	goaction = new PCGoAction(this);

	goaction->initGoData();

}


bool CMEFServerDlg::agreeExitGame()
{
	if (::MessageBox(m_hWnd, _T("确定离开当前游戏？\n"), _T("离开"), MB_OKCANCEL) != IDOK)//可能点确定，点取消，还可能点X关掉窗口
	{
		return false;
	}
	//确定离开时：clear
	golisten.Close();
	gogame.Close();
	if (goaction != NULL) {
		delete goaction; //服务器未连接客户端时，此为null
		goaction = NULL;
	}

	game = false;
	work = false;

	return true;
}


//重新显示相关标签
void CMEFServerDlg::showIPInput()
{
	resetBackGround();
	GetDlgItem(ID_ServerIp)->ShowWindow(true); //显示该ip输入控件
	GetDlgItem(ID_FILE_OPEN)->ShowWindow(true); //隐藏ip提示控件
}


//点击显示IP 确定则结束游戏，显示IP输入框
void CMEFServerDlg::Onresetwin()
{	//没有开始游戏，则不处理
	if (work && !agreeExitGame()) {
		return;
	}
	showIPInput();
}




//关闭程序，离开事件触发？（非点X）
void CMEFServerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (::MessageBox(m_hWnd, _T("确定要离开？\n"), _T("离开游戏"), MB_OKCANCEL) != IDOK)//可能点确定，点取消，还可能点X关掉窗口
	{
		return;
	}
	else {
		if (goaction != NULL) {
			delete goaction; //服务器未连接客户端时，此为null
			goaction = NULL;
		}
	}

	CDialogEx::OnOK();
}


//获取主机ip地址
void CMEFServerDlg::GetHostAddress(CString &strIPAddr)
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))   //初始化
	{
		AfxMessageBox(_T("初始化网络环境失败!"));
		return;
	}
	char    HostName[100];
	gethostname(HostName, sizeof(HostName));// 获得本机主机名.
	hostent *p = gethostbyname(HostName); //从计算机名得到主机信息
	if (p == NULL)
	{
		AfxMessageBox(_T("得到本机网络信息失败!"));
		return;
	}
	strIPAddr = inet_ntoa(*(struct in_addr *)p->h_addr_list[0]);//把ip换成字符串形式
	WSACleanup();               //释放Winsock API
}
