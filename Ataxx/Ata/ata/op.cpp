#include <string>
#include <cstdlib>
#include <ctime>
#include<graphics.h>
#include<fstream>
#include<thread>
#include<chrono>
using namespace std;

fstream lb("file.txt");
//后端部分初始化
int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[7][7] = { }; // 先x后y，记录棋盘状态
int blackPieceCount = 2, whitePieceCount = 2;
int startX, startY, resultX, resultY;
static int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };

const int X_POSITION[7] = { 12, 94, 172, 255, 339, 425, 508 };
const int Y_POSITION[7] = { 12, 105, 179, 264, 353, 435, 522 };
const int gap = 594;//检测鼠标位置，将图片化为两个区域
constexpr COLORREF BACKGROUND_COLOR = 929035UL;
bool toexit = 0, toreturn = 0, toload=0;
bool aai = 0, bai = 0;

//窗口大小
constexpr int WIDTH{ 885 };
constexpr int HEIGHT{ 607 };

void inital()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			gridInfo[i][j] = 0;
		}
	}
	blackPieceCount = whitePieceCount = 2;
}

IMAGE backgroundImage, blackImage, whiteImage, gameImage, aboutImage, selectFrameImage, pickmode, pickfirst, original, winpage, losepage, introduction, wipepage,blackwin,whitewin; // 图片对象，取为全局变量
//首页，黑棋，白棋，游戏中界面，介绍界面


IMAGE* defi(int x, int y);

void PutChessPiece(const int x, const int y);



void click();

// 判断是否在地图内
inline bool inMap(int x, int y);

// 向Direction方向改动坐标，并返回是否越界
inline bool MoveStep(int& x, int& y, int Direction);

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int color);

int needSearch(int x0, int y0, int type);

int getValue(int x0, int y0, int type);

inline bool danger(int x0, int y0, int type);

void loadit();



int main()
{
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

	bool ittakestwo = 0;
	// 初始化绘图窗口，宽度和高度
	initgraph(WIDTH, HEIGHT);
	cleardevice();
	loadimage(&backgroundImage, _T("png"), _T("background"));
	loadimage(&blackImage, _T("png"), _T("heiqi"));
	loadimage(&whiteImage, _T("png"), _T("baiqi"));
	loadimage(&gameImage, _T("png"), _T("game"));
	loadimage(&pickmode, _T("png"), _T("mode"));
	loadimage(&original, _T("png"), _T("origin"));
	loadimage(&winpage, _T("png"), _T("win"));
	loadimage(&pickfirst, _T("png"), _T("sequence"));
	loadimage(&introduction, _T("png"), _T("intr"));
	loadimage(&losepage, _T("png"), _T("loseit"));
	loadimage(&wipepage, _T("png"), _T("blank"));
	loadimage(&blackwin, _T("png"), _T("bw"));
	loadimage(&whitewin, _T("png"), _T("ww"));
	putimage(0, 0, &original);
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.x > 180 && msg.x < 529 && msg.y>369 && msg.y < 499)
		{
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				putimage(0, 0, &introduction);
				MOUSEMSG tem = GetMouseMsg();
				if (tem.uMsg == WM_RBUTTONDOWN)
					break;

			}
		}
		else {
			if (msg.uMsg == WM_LBUTTONDOWN)
				break;
		}
	}
stop2:
	ittakestwo = 0;
	toreturn = toload = toexit = 0;
	putimage(0, 0, &backgroundImage);
	while (1) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			if (msg.x > 658 && msg.x < 840 && msg.y>64 && msg.y < 151)
			{
				inital();
				gridInfo[0][0] = gridInfo[6][6] = 1;  //|黑|白|
				gridInfo[6][0] = gridInfo[0][6] = -1; //|白|黑|
				break;
			}
			else if (msg.x > 658 && msg.x < 840 && msg.y>231 && msg.y < 329)
			{
				loadit();
				if (!aai && bai)
				{
					ittakestwo = 0;
					currBotColor = -1;
					goto pp;
				}
				if (aai && !bai)
				{
					ittakestwo = 0;
					currBotColor = 1;
					goto pp;
				}
				if (!aai && !bai)
				{
					ittakestwo = 1;
					goto pp;
				}
				break;
			}
			else if (msg.x > 658 && msg.x < 840 && msg.y>412 && msg.y < 517)
			{
				return 0;
			}
		}
	}
	
	putimage(0, 0, &pickmode);
	while (1) {
		MOUSEMSG msg1 = GetMouseMsg();
		if (msg1.uMsg == WM_LBUTTONDOWN) {
			if (msg1.x > 93 && msg1.x < 354 && msg1.y>146 && msg1.y < 290)
			{
				break;
			}
			else if (msg1.x > 442 && msg1.x < 710 && msg1.y>146 && msg1.y < 294)
			{
				ittakestwo = 1;
				break;
			}
		}
	}
	if (!ittakestwo) {
	putimage(0, 0, &pickfirst);
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			if (msg.x > 61 && msg.x < 355 && msg.y>150 && msg.y < 356)
			{
				currBotColor = -1;
				break;
			}
			else if (msg.x > 457 && msg.x < 797 && msg.y>148 && msg.y < 328)
			{
				currBotColor = 1;
				break;
			}
		}
	}
	}
	pp:
	putimage(0, 0, &gameImage);
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			PutChessPiece(i, j);
		}
	}
	if (!ittakestwo) {
		if (currBotColor == -1) {
			aai = 0, bai = 1;
			while (1) {
				bool playabletomove = 0;
				for (y0 = 0; y0 < 7; y0++) {
					for (x0 = 0; x0 < 7; x0++)
					{
						if (gridInfo[x0][y0] != -currBotColor)
							continue;
						for (int dir = 0; dir < 24; dir++)
						{
							x1 = x0 + delta[dir][0];
							y1 = y0 + delta[dir][1];
							if (!inMap(x1, y1))
								continue;
							if (gridInfo[x1][y1] != 0)
								continue;
							else
								playabletomove = 1;
						}
					}
				}
				if (!playabletomove)
					goto sid;
				click();
				if (toreturn)
				{
					inital();
					goto stop2;
				}
				if (toload)
				{
					lb.seekp(0);
					lb << aai << ' ' << bai;
					for (int i = 0; i < 7; i++)
					{
						for (int j = 0; j < 7; j++)
						{
							lb << ' ' << gridInfo[i][j];
						}
					}
					goto stop2;
				}
				if (toexit)
					return 0;
			sid:
				bool abletomove = 0;
				int startX, startY, resultX, resultY;
				int overAll = -200303;
				for (y0 = 0; y0 < 7; y0++) {
					for (x0 = 0; x0 < 7; x0++)
					{
						if (gridInfo[x0][y0] != currBotColor)
							continue;
						for (int dir = 0; dir < 24; dir++)
						{
							x1 = x0 + delta[dir][0];
							y1 = y0 + delta[dir][1];
							if (!inMap(x1, y1))
								continue;
							if (gridInfo[x1][y1] != 0)
								continue;
							if (needSearch(x0, y0, gridInfo[x0][y0]) < 8)
							{
								abletomove = 1;
								if (getValue(x0, y0, gridInfo[x0][y0]) == 0 && getValue(x1, y1, gridInfo[x0][y0]) == 0)
									continue;
								int maxNode = getValue(x1, y1, -currBotColor);
								int minNode = 0, x2, y2;
								for (int i = 0; i < 8; i++)
								{
									x2 = x1 + delta[i][0];
									y2 = y1 + delta[i][1];
									if (gridInfo[x2][y2] == 0 && danger(x2, y2, gridInfo[x0][y0]))
										minNode = minNode > getValue(x2, y2, currBotColor) ? minNode : getValue(x2, y2, currBotColor);
								}
								if ((abs((x0 - x1)) == 2 || abs((y0 - y1)) == 2) && danger(x0, y0, gridInfo[x0][y0]))
								{
									minNode = minNode > getValue(x0, y0, gridInfo[x0][y0]) ? minNode : getValue(x0, y0, gridInfo[x0][y0]);
								}
								if ((maxNode - minNode - 1) > overAll)
								{
									overAll = maxNode - minNode - 1;
									startX = x0;
									startY = y0;
									resultX = x1;
									resultY = y1;
								}
							}
						}
					}
				}

				if (!abletomove)
				{
					startX = -1;
					startY = -1;
					resultX = -1;
					resultY = -1;
				}
				ProcStep(startX, startY, resultX, resultY, currBotColor);
				this_thread::sleep_for(chrono::milliseconds(1000));
				for (int i = 0; i < 7; i++)
				{
					for (int j = 0; j < 7; j++)
					{
						PutChessPiece(i, j);
					}
				}
				if ((!abletomove || !playabletomove) || (blackPieceCount + whitePieceCount == 49) || blackPieceCount == 0 || whitePieceCount == 0) {
					if (!abletomove||blackPieceCount > whitePieceCount) {
						putimage(0, 0, &winpage);
						while (1)
						{
							MOUSEMSG msg = GetMouseMsg();
							if (msg.uMsg == WM_LBUTTONDOWN) {
								return 0;
							}
							else if (msg.uMsg == WM_RBUTTONDOWN) {
								inital();
								goto stop2;
							}
						}
					}
					else if(!playabletomove|| blackPieceCount < whitePieceCount) {
						putimage(0, 0, &losepage);
						while (1)
						{
							MOUSEMSG msg = GetMouseMsg();
							if (msg.uMsg == WM_RBUTTONDOWN) {
								return 0;
							}
							else if (msg.uMsg == WM_LBUTTONDOWN) {
								inital();
								goto stop2;
							}
						}
					}
				}
			}
		}
		else if (currBotColor == 1)
		{
		aai = 1, bai = 0;
			while (1) {
				bool abletomove = 0;
				int startX, startY, resultX, resultY;
				int overAll = -200303;

				this_thread::sleep_for(chrono::milliseconds(1000));
				for (y0 = 0; y0 < 7; y0++) {
					for (x0 = 0; x0 < 7; x0++)
					{
						if (gridInfo[x0][y0] != currBotColor)
							continue;
						for (int dir = 0; dir < 24; dir++)
						{
							x1 = x0 + delta[dir][0];
							y1 = y0 + delta[dir][1];
							if (!inMap(x1, y1))
								continue;
							if (gridInfo[x1][y1] != 0)
								continue;
							if (needSearch(x0, y0, gridInfo[x0][y0]) < 8)
							{
								abletomove = 1;
								if (getValue(x0, y0, gridInfo[x0][y0]) == 0 && getValue(x1, y1, gridInfo[x0][y0]) == 0)
									continue;
								int maxNode = getValue(x1, y1, -currBotColor);
								int minNode = 0, x2, y2;
								for (int i = 0; i < 8; i++)
								{
									x2 = x1 + delta[i][0];
									y2 = y1 + delta[i][1];
									if (gridInfo[x2][y2] == 0 && danger(x2, y2, gridInfo[x0][y0]))
										minNode = minNode > getValue(x2, y2, currBotColor) ? minNode : getValue(x2, y2, currBotColor);
								}
								if ((abs((x0 - x1)) == 2 || abs((y0 - y1)) == 2) && danger(x0, y0, gridInfo[x0][y0]))
								{
									minNode = minNode > getValue(x0, y0, gridInfo[x0][y0]) ? minNode : getValue(x0, y0, gridInfo[x0][y0]);
								}
								if ((maxNode - minNode - 1) > overAll)
								{
									overAll = maxNode - minNode - 1;
									startX = x0;
									startY = y0;
									resultX = x1;
									resultY = y1;
								}
							}
						}
					}
				}

				if (!abletomove)
				{
					startX = -1;
					startY = -1;
					resultX = -1;
					resultY = -1;
				}
				bool playabletomove = 0;
				ProcStep(startX, startY, resultX, resultY, currBotColor);

				for (int i = 0; i < 7; i++)
				{
					for (int j = 0; j < 7; j++)
					{
						PutChessPiece(i, j);
					}
				}
				for (y0 = 0; y0 < 7; y0++) {
					for (x0 = 0; x0 < 7; x0++)
					{
						if (gridInfo[x0][y0] != -currBotColor)
							continue;
						for (int dir = 0; dir < 24; dir++)
						{
							x1 = x0 + delta[dir][0];
							y1 = y0 + delta[dir][1];
							if (!inMap(x1, y1))
								continue;
							if (gridInfo[x1][y1] != 0)
								continue;
							else
								playabletomove = 1;
						}
					}
				}
				if (!playabletomove)
					goto sid1;
				click();
				if (toreturn)
				{
					inital();
					goto stop2;
				}
				if (toload)
				{
					lb.seekp(0);
					lb<<aai<<' '<<bai;
					for (int i = 0; i < 7; i++)
					{
						for (int j = 0; j < 7; j++)
						{
							lb << ' ' << gridInfo[i][j];
						}
					}
					goto stop2;
				}
				if (toexit)
					return 0;
			sid1:
				if ((!abletomove || !playabletomove) || (blackPieceCount + whitePieceCount == 49) || whitePieceCount == 0 || blackPieceCount == 0) {
					if (!abletomove||blackPieceCount < whitePieceCount) {
						putimage(0, 0, &winpage);
						while (1)
						{
							MOUSEMSG msg = GetMouseMsg();
							if (msg.uMsg == WM_LBUTTONDOWN) {
								return 0;
							}
							else if (msg.uMsg == WM_RBUTTONDOWN) {
								inital();
								goto stop2;
							}
						}
					}
					else if(!playabletomove||whitePieceCount<blackPieceCount) {
						putimage(0, 0, &losepage);
						while (1)
						{
							MOUSEMSG msg = GetMouseMsg();
							if (msg.uMsg == WM_RBUTTONDOWN) {
								return 0;
							}
							else if (msg.uMsg == WM_LBUTTONDOWN) {
								inital();
								goto stop2;
							}
						}
					}
				}

			}
		}
	}
	else
	{
	aai = bai = 0;
		while (1) {
			currBotColor = -1;//当一个玩家操作时，直接把另一个玩家视为bot
			bool playabletomoveb = 0;
			for (y0 = 0; y0 < 7; y0++) {
				for (x0 = 0; x0 < 7; x0++)
				{
					if (gridInfo[x0][y0] != -currBotColor)
						continue;
					for (int dir = 0; dir < 24; dir++)
					{
						x1 = x0 + delta[dir][0];
						y1 = y0 + delta[dir][1];
						if (!inMap(x1, y1))
							continue;
						if (gridInfo[x1][y1] != 0)
							continue;
						else
							playabletomoveb = 1;
					}
				}
			}
			if (!playabletomoveb)
				goto sd;
			click();
			if (toreturn)
			{
				inital();
				goto stop2;
			}
			if (toload)
			{
				lb.seekp(0);
				lb << aai << ' ' << bai;
				for (int i = 0; i < 7; i++)
				{
					for (int j = 0; j < 7; j++)
					{
						lb << ' ' << gridInfo[i][j];
					}
				}
				goto stop2;
			}
			if (toexit)
				return 0;
		sd:
			currBotColor = 1;
			bool playabletomovew = 0;
			for (y0 = 0; y0 < 7; y0++) {
				for (x0 = 0; x0 < 7; x0++)
				{
					if (gridInfo[x0][y0] != -currBotColor)
						continue;
					for (int dir = 0; dir < 24; dir++)
					{
						x1 = x0 + delta[dir][0];
						y1 = y0 + delta[dir][1];
						if (!inMap(x1, y1))
							continue;
						if (gridInfo[x1][y1] != 0)
							continue;
						else
							playabletomovew = 1;
					}
				}
			}
			if (!playabletomovew)
				goto sdd;
			click();
			if (toreturn)
			{
				inital();
				goto stop2;
			}
			if (toload)
			{
				 lb.seekp(0);
				lb << aai << ' ' << bai;
				for (int i = 0; i < 7; i++)
				{
					for (int j = 0; j < 7; j++)
					{
						lb << ' ' << gridInfo[i][j];
					}
				}
				goto stop2;
			}
			if (toexit)
				return 0;
		sdd:
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					PutChessPiece(i, j);
				}
			}
			if ((!playabletomoveb || !playabletomovew) || (blackPieceCount + whitePieceCount == 49) || blackPieceCount == 0 || whitePieceCount == 0) {
				if (!playabletomovew||blackPieceCount > whitePieceCount) {
					putimage(0, 0, &blackwin);
					while (1)
					{
						MOUSEMSG msg = GetMouseMsg();
						if (msg.uMsg == WM_LBUTTONDOWN) {
							return 0;
						}
						else if (msg.uMsg == WM_RBUTTONDOWN) {
							inital();
							goto stop2;
						}
					}
				}
				else if (!playabletomoveb || blackPieceCount < whitePieceCount) {
					putimage(0, 0, &whitewin);
					while (1)
					{
						MOUSEMSG msg = GetMouseMsg();
						if (msg.uMsg == WM_RBUTTONDOWN) {
							return 0;
						}
						else if (msg.uMsg == WM_LBUTTONDOWN) {
							inital();
							goto stop2;
						}
					}
				}
			}
		}
	}
 

	closegraph();//关闭绘图窗口
	cleardevice();//清空绘图设备
	lb.close();
	return 0;
}













































IMAGE* defi(int x, int y)
{
	if (gridInfo[x][y] == 1)
		return &blackImage;
	else if (gridInfo[x][y] == -1)
		return &whiteImage;
	else
		return &wipepage;
}
inline bool inMap(int x, int y)
{
	if (x < 0 || x > 6 || y < 0 || y > 6)
		return false;
	return true;
}
void PutChessPiece(const int x, const int y)
{
	IMAGE* img = defi(x, y);
	int xPosition = X_POSITION[x];
	int yPosition = Y_POSITION[y];
	DWORD* imgPtr = GetImageBuffer(img);
	DWORD* bkgPtr = GetImageBuffer();
	int width = img->getwidth();
	int height = img->getheight();
	int bkgWidth = getwidth();
	COLORREF bkgColor = BACKGROUND_COLOR + 0xff000000;
	int i, j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if (imgPtr[i * width + j] != bkgColor)
				bkgPtr[(i + yPosition) * bkgWidth + j + xPosition] = imgPtr[i * width + j];
		}
	}
}



void click()
{
	int px = -1, py = 6, tx = -1, ty = 6;
	MOUSEMSG msg;
sign:
	while (true) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			for (int i = 0; i < 7; i++)
			{
				if (X_POSITION[i] > msg.x)
				{
					px = i - 1;
					break;
				}
			}
			if (msg.x < gap&&px==-1)
				px = 6;
			else if (msg.x > gap)
			{
				if (msg.x > 658 && msg.x < 840 && msg.y>64 && msg.y < 151)
				{
					toreturn = 1; return;
				}
				else if (msg.x > 658 && msg.x < 840 && msg.y>231 && msg.y < 329)
				{
					toload = 1; return;
				}
				else if (msg.x > 658 && msg.x < 840 && msg.y>412 && msg.y < 517)
				{
					toexit = 1; return;
				}
				else
					goto sign;
			}
			for (int i = 0; i < 7; i++)
			{
				if (Y_POSITION[i] > msg.y)
				{
					py = i - 1;
					break;
				}
			}
			if (gridInfo[px][py] == -currBotColor)
			{
				break;
			}
			else {
				goto sign;
			}
		}
	}
sign1:
	while (true) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			for (int i = 0; i < 7; i++)
			{
				if (X_POSITION[i] > msg.x)
				{
					tx = i - 1;
					break;
				}
			}
			if (msg.x < gap && tx == -1)
				tx = 6;
			else if (msg.x > gap)
			{
				if (msg.x > 658 && msg.x < 840 && msg.y>64 && msg.y < 151)
				{
					toreturn = 1; return;
				}
				else if (msg.x > 658 && msg.x < 840 && msg.y>231 && msg.y < 329)
				{
					toload = 1; return;
				}
				else if (msg.x > 658 && msg.x < 840 && msg.y>412 && msg.y < 517)
				{
					toexit = 1; return;
				}
				else
					goto sign1;
			}
			for (int i = 0; i < 7; i++)
			{
				if (Y_POSITION[i] > msg.y)
				{
					ty = i - 1;
					break;
				}
			}
			if (gridInfo[tx][ty] == 0)
			{
				break;
			}
			else {
				goto sign1;
			}
		}
	}
	if (abs(px - tx) <= 2 && abs(py - ty) <= 2) {
		ProcStep(px, py, tx, ty, -currBotColor);
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				PutChessPiece(i, j);
			}
		}
		return;
	}
	else {
		goto sign1;
	}
}

void loadit()
{
	lb.seekg(0);
	lb >> aai >> bai;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			lb >> gridInfo[i][j];
		}
	}
}


// 判断是否在地图内

// 向Direction方向改动坐标，并返回是否越界
inline bool MoveStep(int& x, int& y, int Direction)
{
	x = x + delta[Direction][0];
	y = y + delta[Direction][1];
	return inMap(x, y);
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int color)
{
	if (color == 0)
		return false;
	if (x1 == -1) // 无路可走，跳过此回合
		return true;
	if (!inMap(x0, y0) || !inMap(x1, y1)) // 超出边界
		return false;
	if (gridInfo[x0][y0] != color)
		return false;
	int dx, dy, x, y, currCount = 0, dir;
	int effectivePoints[8][2]{};
	dx = abs((x0 - x1)), dy = abs((y0 - y1));
	if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) // 保证不会移动到原来位置，而且移动始终在5×5区域内
		return false;
	if (gridInfo[x1][y1] != 0) // 保证移动到的位置为空
		return false;
	if (dx == 2 || dy == 2) // 如果走的是5×5的外围，则不是复制粘贴
		gridInfo[x0][y0] = 0;
	else
	{
		if (color == 1)
			blackPieceCount++;
		else
			whitePieceCount++;
	}

	gridInfo[x1][y1] = color;
	for (dir = 0; dir < 8; dir++) // 影响邻近8个位置
	{
		x = x1 + delta[dir][0];
		y = y1 + delta[dir][1];
		if (!inMap(x, y))
			continue;
		if (gridInfo[x][y] == -color)
		{
			effectivePoints[currCount][0] = x;
			effectivePoints[currCount][1] = y;
			currCount++;
			gridInfo[x][y] = color;
		}
	}
	if (currCount != 0)
	{
		if (color == 1)
		{
			blackPieceCount += currCount;
			whitePieceCount -= currCount;
		}
		else
		{
			whitePieceCount += currCount;
			blackPieceCount -= currCount;
		}
	}
	return true;
}

int needSearch(int x0, int y0, int type) {
	int cnt = 0, x1, y1;
	for (int d = 0; d < 8; d++) {
		x1 = x0 + delta[d][0];
		y1 = y0 + delta[d][1];
		if (x1 < 0 || x1 > 6 || y1 < 0 || y1 > 6
			|| gridInfo[x1][y1] == type)
			cnt++;
	}
	return cnt;
}

int getValue(int x0, int y0, int type) {
	int cnt = 0, x1, y1;
	for (int d = 0; d < 8; d++) {
		x1 = x0 + delta[d][0];
		y1 = y0 + delta[d][1];
		if (inMap(x1, y1) && gridInfo[x1][y1] == type)
			cnt++;
	}
	return cnt;
}



inline bool danger(int x0, int y0, int type)
{

	int x1, y1;
	for (int d = 0; d < 24; d++) {
		x1 = x0 + delta[d][0];
		y1 = y0 + delta[d][0];
		if (x1 >= 0 && x1 <= 6 && y1 >= 0 && y1 <= 6
			&& gridInfo[x1][y1] == -type)
			return true;
	}
	return false;

}