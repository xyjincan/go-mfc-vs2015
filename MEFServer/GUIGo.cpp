#include "stdafx.h"
#include "GUIGo.h"

//维护棋盘界面，供界面事件调用，内部包含围棋内部数据。

GUIGo::~GUIGo()
{
}

//初始化构造函数，准备几个不变的资源对象

GUIGo::GUIGo() {
	alen = SIZEOFBOARD;
	//
	penblack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	clearcell.CreatePen(PS_SOLID, 1, RGB(241, 224, 157));
	penwrite.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	bblack.CreateSolidBrush(RGB(0, 0, 0));
	bwrite.CreateSolidBrush(RGB(255, 255, 255));
	background.CreateSolidBrush(RGB(241, 224, 157));

}

//当程序窗口改变大小，重新设置棋子棋盘大小，

void GUIGo::setSize(int newSize) {

	extsize = newSize / 6;
	size = newSize;
	cell_h = size / 2;
	sizeofbroad = (alen - 1)*size;
	broadrange = origin + sizeofbroad;
	base = origin + (alen / 2)  * size;
	ext = extlen * size;//19*19围棋点距离6
	
}

//初始化 or 新开局
void GUIGo::initGo(CDC* pDC) {

	setSize(size);
	count = 1;
	py = px = -1;
	go.startGo();//内部数据清空
	initBoard(pDC);//reset background
	updateTips(pDC);

};

//当游戏时，整个界面需要重绘。。。
void GUIGo::updateInf(CDC* pDC) {

	initBoard(pDC);
	updateTips(pDC);

	for (int i = 0; i <= SIZEOFBOARD; i++) {
		for (int j = 0; j <= SIZEOFBOARD; j++) {
			Status cre = go.infGo(i, j);
			if (cre == s_empty) {
				continue;
			}
			//
			Cell cell = go.go[i][j];
			if (cre == s_write) {
				addCell(pDC, cell, w);
			}
			else {
				addCell(pDC, cell, b);
			}
		}
	}
}

//悔棋
void GUIGo::upGo(CDC* pDC) {

	if (count > 1) {
		count--;
		//返回走步，加走步引起的死子
		vector<Cell> re = go.undo();//小于0表示此步弃权
		if (re.size() > 0) {

			removeCell(pDC, re.at(0));//刷新GUI，移除走棋，
			//如有死棋，回填到棋盘,颜色与count-1一致
			cellcolor cf;
			cellcolor next;
			if ((count - 1) % 2 == 1) {
				cf = b;
				next = w;
			}
			else {
				cf = w;
				next = b;
			}
			for (unsigned int i = 1; i < re.size(); i++) {
				Cell back = re.at(i);
				addCell(pDC, back, cf);
			}
		}
		updateTips(pDC);
	}
}

//本步，弃权
void GUIGo::passStip(CDC* pDC) {

	py = px = -1;
	go.passStip();
	count++;
	updateTips(pDC);
}



//新下棋时，检查是否有效
bool GUIGo::oppositeGo(CDC* pDC, int ax, int ay) {
	//保持x y坐标
	px = ax;
	py = ay;

	Cell cg;
	if (!isOnBoard(ax, ay)) {
		return false;//是否在棋盘
	}

	cg.Init(ax, ay);
	cellcolor next;
	cellcolor nowcolor;
	if (count % 2 == 1) {
		cg.updateStatus(s_black);
		next = w;
		nowcolor = b;
	}
	else {
		cg.updateStatus(s_write);
		next = b;
		nowcolor = w;
	}

	//落子检查
	if (!go.Point(cg)) {
		return false;
	}
	addCell(pDC, cg, nowcolor);

	//落子反馈
	vector<Cell> ret = go.PointCheck();
	if (ret.size() > 1) {
		for (unsigned int i = 1; i < ret.size(); i++) {
			removeCell(pDC, ret.at(i));
		}
	}
	count++;
	updateTips(pDC);
	return true;

}


//下棋，检查是否有效
bool GUIGo::clickGo(CDC* pDC, CPoint point) {
	//计算坐标
	int ax = int((point.x - origin) / (float)size + 0.48);
	int ay = int((point.y - origin) / (float)size + 0.48);

	return oppositeGo(pDC, ax, ay);
}


//添加棋子到界面
void GUIGo::addCell(CDC* pDC, Cell cell, cellcolor cc) {

	int ax = origin + (cell.x)*size;
	int ay = origin + (cell.y)*size;
	CBrush *oldbrush;   //画刷
	CPen *oldpen;

	if (cc == b) {
		oldbrush = pDC->SelectObject(&bblack);
		oldpen = pDC->SelectObject(&penblack);
	}
	else {
		oldbrush = pDC->SelectObject(&bwrite);
		oldpen = pDC->SelectObject(&penwrite);
	}
	pDC->Ellipse(ax - cell_h, ay - cell_h, ax + cell_h, ay + cell_h);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
}

//移除界面某棋子
void GUIGo::removeCell(CDC* pDC, Cell cell) {

	//坐标系棋盘上标记为1，1开始实际为0，0（0-18，1-19）
	int ax = cell.x;
	int ay = cell.y;
	ax = origin + ax * size;
	ay = origin + ay * size;

	CBrush *oldbrush;   //画刷

	oldbrush = pDC->SelectObject(&background);
	//pDC->Ellipse(ax - cell_h, ay - cell_h, ax + cell_h, ay + cell_h);//清除棋子
	//以背景色画跟棋子大小相同的点，覆盖棋子
	pDC->SelectObject(clearcell);
	//pDC->MoveTo(ax, ay);
	//pDC->LineTo(ax, ay);//防止开始点产生间断
	pDC->Ellipse(ax - cell_h, ay - cell_h, ax + cell_h, ay + cell_h);
	pDC->SelectObject(&penblack);

	int xf, xe;
	int yf, ye;
	xf = ax - cell_h;
	xe = ax + cell_h;
	//检查回填线是否超出棋盘
	if (xf < origin) {
		xf = origin;
	}
	if (xe > broadrange) {
		xe = broadrange;
	}

	yf = ay - cell_h;
	ye = ay + cell_h;
	if (yf < origin) {
		yf = origin;
	}
	if (ye > broadrange) {
		ye = broadrange;
	}

	//补充棋盘线
	pDC->MoveTo(xf, ay);
	pDC->LineTo(xe, ay);//前开后闭画线。。。

	pDC->MoveTo(ax, yf);
	pDC->LineTo(ax, ye);

	pDC->SelectObject(oldbrush);
	//检查是否是关键点,补充坐标点
	addBroad(pDC, cell.x, cell.y);
}

//清除某个点的棋子时
//更新棋盘时，检查是否为关键点（）
void GUIGo::addBroad(CDC* pDC, int ax, int ay) {

	CBrush *oldbrush;   //画刷
	oldbrush = pDC->SelectObject(&bblack);
	if (ax == 3 || ax == 9 || ax == 15) {
		if (ay == 3 || ay == 9 || ay == 15) {
			ax = ax; ay = ay;
			ax = origin + ax*size; ay = origin + ay*size;
			pDC->Ellipse(ax + extsize, ay + extsize, ax - extsize, ay - extsize);
		}
	}
	pDC->SelectObject(oldbrush);
}


//初始化棋盘界面
void GUIGo::initBoard(CDC* pDC) {

	//保存旧画刷,笔
	CBrush *oldbrush = pDC->SelectObject(&background);
	CPen *oldpen = pDC->SelectObject(&clearcell);
	pDC->Rectangle(0 ,0 , origin + sizeofbroad + 300, origin + sizeofbroad + 300);// 清空棋盘

	pDC->SelectObject(&penblack);
	CPoint t;
	//棋盘画线，每次纵横画线
	for (int i = 0; i < alen; i++) {
		//横线
		t.x = origin;
		t.y = origin + i*size;
		pDC->MoveTo(t);
		t.x = broadrange;
		pDC->LineTo(t);
		//纵线
		t.x = t.y;
		t.y = origin;
		pDC->MoveTo(t);
		t.y = broadrange;
		pDC->LineTo(t);
	}

	//添加棋盘上的九个关键点

	pDC->SelectObject(&bblack);
	pDC->Ellipse(base + extsize, base + extsize, base - extsize, base - extsize);
	pDC->Ellipse(base + extsize + ext, base + extsize + ext, base - extsize + ext, base - extsize + ext);
	pDC->Ellipse(base + extsize - ext, base + extsize - ext, base - extsize - ext, base - extsize - ext);
	pDC->Ellipse(base + extsize, base + extsize - ext, base - extsize, base - extsize - ext);
	pDC->Ellipse(base + extsize, base + extsize + ext, base - extsize, base - extsize + ext);
	pDC->Ellipse(base + extsize - ext, base + extsize, base - extsize - ext, base - extsize);
	pDC->Ellipse(base + extsize + ext, base + extsize, base - extsize + ext, base - extsize);
	pDC->Ellipse(base + extsize - ext, base + extsize + ext, base - extsize - ext, base - extsize + ext);
	pDC->Ellipse(base + extsize + ext, base + extsize - ext, base - extsize + ext, base - extsize - ext);


	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);

}

//显示提示信息
void GUIGo::updateTips(CDC* pDC) {

	cellcolor next;//获取下一个子颜色，显示提示
	if (count % 2 == 0) {
		next = w;
	}
	else {
		next = b;
	}

	Cell markcolor;
	markcolor.x = alen + 2;
	markcolor.y = alen / 2;
	addCell(pDC, markcolor, next);//添加next棋子在信息栏
	showLog(pDC);
}


//显示当前局，统计点子结果
void GUIGo::showCountBlack(CDC* pDC) {

	//得到统计结果
	float sum = go.countCell();

	pDC->SetBkColor(RGB(255, 255, 255));//背景颜色
	pDC->SetBkMode(OPAQUE);//背景模式
	pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);//设置文本对齐方式

	CString stx;
	CString sty;
	CString aaa = _T("黑 :  ");
	CString asp = _T("    ");
	sty.Format(TEXT("%.2f"), sum);
	stx.Append(asp);
	stx.Append(aaa);
	stx.Append(sty);
	stx.Append(asp);
	pDC->TextOut(broadrange + 60, base - 100, stx);
	sum = alen*alen - sum;//先手优势？
	stx = "";
	aaa = _T("白 :  ");
	sty.Format(TEXT("%.2f"), sum);
	stx.Append(asp);
	stx.Append(aaa);
	stx.Append(sty);
	stx.Append(asp);
	pDC->TextOut(broadrange + 60, base - 140, stx);
}

//显示计数，最后一手位置信息
void GUIGo::showLog(CDC* pDC) {
	pDC->SetBkColor(RGB(255, 255, 255));//背景颜色
	pDC->SetBkMode(OPAQUE);//背景模式
	pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);//设置文本对齐方式
	CString stx;
	stx.Format(TEXT("%d"), count - 1);

	CString nowpoint;
	nowpoint.Format(TEXT(": %d * %d "),px+1,py+1);
	stx.Append(nowpoint);
	stx.Append(_T("    "));
	pDC->TextOut(broadrange + 60, base - 60, stx);


}

