#ifndef SNAKE_H
#define SNAKE_H

#include "resource.h"

//结构体
struct Point{
	int x;
	int y;
};

//自定义全局变量
extern const int g_nBoard;
extern const int g_nSideOfGrid;
extern const int g_nSideOfGame;
extern const int g_nInfoX;

//函数声明
void Align(HWND hWnd);
void DrawBK(HDC hdc);
void DrawInfo(HDC hdc);
void Random(int &x, int &y);
#endif