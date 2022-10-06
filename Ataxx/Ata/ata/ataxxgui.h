#pragma once
#include <string>
#include <cstdlib>
#include <ctime>
#include<graphics.h>
#include<fstream>
#include"ataxxai.h"
#include"ataxxgui.h"
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


IMAGE* defi(int x, int y)
{
	if (gridInfo[x][y] == 1)
		return &blackImage;
	else
		return &whiteImage;
}


void click(MOUSEMSG msg)
{
	int px=0, py=0;
	for (int i = 0; i < 7; i++)
	{
		if (X_POSITION[i] > msg.x)
		{
			px = i - 1;
			break;
		}
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
		MOUSEMSG msg1;
		int tx, ty;
		for (int i = 0; i < 7; i++)
		{
			if (X_POSITION[i] > msg1.x)
			{
				tx = i - 1;
				break;
			}
		}
		for (int i = 0; i < 7; i++)
		{
			if (Y_POSITION[i] > msg1.y)
			{
				ty = i - 1;
				break;
			}
		}
		int dx, dy, x, y, currCount = 0, dir;
		int effectivePoints[8][2];
		dx = abs((px - tx)), dy = abs((py - ty));
		if ((dx != 0 || dy != 0) && dx <= 2 && dy <= 2 && gridInfo[tx][ty] == 0) // 保证不会移动到原来位置，而且移动始终在5×5区域内 保证移动到的位置为空
		{
			if (dx == 2 || dy == 2) // 如果走的是5×5的外围，则不是复制粘贴
				gridInfo[px][py] = 0;
			else
			{
				if (-currBotColor == 1)
					blackPieceCount++;
				else
					whitePieceCount++;
			}

			gridInfo[tx][ty] = -currBotColor;
			for (dir = 0; dir < 8; dir++) // 影响邻近8个位置
			{
				x = tx + delta[dir][0];
				y = ty + delta[dir][1];
				if (!inMap(x, y))
					continue;
				if (gridInfo[x][y] == currBotColor)
				{
					effectivePoints[currCount][0] = x;
					effectivePoints[currCount][1] = y;
					currCount++;
					gridInfo[x][y] = -currBotColor;
				}
			}
			if (currCount != 0)
			{
				if (-currBotColor == 1)
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
			return ; 
		}
	}
}


