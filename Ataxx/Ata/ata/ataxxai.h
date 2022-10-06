#pragma once
#include <string>
#include <cstdlib>
#include <ctime>
#include<graphics.h>
#include<fstream>
using namespace std;
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
constexpr COLORREF BACKGROUND_COLOR = 929035UL;


//窗口大小
constexpr int WIDTH{ 885 };
constexpr int HEIGHT{ 607 };

IMAGE backgroundImage, blackImage, whiteImage, gameImage, aboutImage, selectFrameImage, pickmode, pickfirst, original, winpage, losepage, introduction; // 图片对象，取为全局变量
//首页，黑棋，白棋，游戏中界面，介绍界面


// 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x > 6 || y < 0 || y > 6)
		return false;
	return true;
}

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
	int effectivePoints[8][2];
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