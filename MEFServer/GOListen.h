

#pragma once
// GOListen 命令目标
class CMEFServerDlg;

//服务器等待客户端监听类
class GOListen : public CAsyncSocket
{
public:

	virtual void OnAccept(int nErrorCode);

public:
	CMEFServerDlg* Gdig;
	void initDig(CMEFServerDlg* dig);
};



