#pragma once
#include <string>
#include <cstdlib>
#include <ctime>
#include<graphics.h>
#include<fstream>
using namespace std;
//��˲��ֳ�ʼ��
int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[7][7] = { }; // ��x��y����¼����״̬
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


//���ڴ�С
constexpr int WIDTH{ 885 };
constexpr int HEIGHT{ 607 };

IMAGE backgroundImage, blackImage, whiteImage, gameImage, aboutImage, selectFrameImage, pickmode, pickfirst, original, winpage, losepage, introduction; // ͼƬ����ȡΪȫ�ֱ���
//��ҳ�����壬���壬��Ϸ�н��棬���ܽ���


// �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x > 6 || y < 0 || y > 6)
		return false;
	return true;
}

// ��Direction����Ķ����꣬�������Ƿ�Խ��
inline bool MoveStep(int& x, int& y, int Direction)
{
	x = x + delta[Direction][0];
	y = y + delta[Direction][1];
	return inMap(x, y);
}

// �����괦���ӣ�����Ƿ�Ϸ���ģ������
bool ProcStep(int x0, int y0, int x1, int y1, int color)
{
	if (color == 0)
		return false;
	if (x1 == -1) // ��·���ߣ������˻غ�
		return true;
	if (!inMap(x0, y0) || !inMap(x1, y1)) // �����߽�
		return false;
	if (gridInfo[x0][y0] != color)
		return false;
	int dx, dy, x, y, currCount = 0, dir;
	int effectivePoints[8][2];
	dx = abs((x0 - x1)), dy = abs((y0 - y1));
	if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) // ��֤�����ƶ���ԭ��λ�ã������ƶ�ʼ����5��5������
		return false;
	if (gridInfo[x1][y1] != 0) // ��֤�ƶ�����λ��Ϊ��
		return false;
	if (dx == 2 || dy == 2) // ����ߵ���5��5����Χ�����Ǹ���ճ��
		gridInfo[x0][y0] = 0;
	else
	{
		if (color == 1)
			blackPieceCount++;
		else
			whitePieceCount++;
	}

	gridInfo[x1][y1] = color;
	for (dir = 0; dir < 8; dir++) // Ӱ���ڽ�8��λ��
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