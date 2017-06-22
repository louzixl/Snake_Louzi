#ifndef SNAKE_H
#define SNAKE_H

#include "stdafx.h"
#include "resource.h"

//结构体
class Point{
public:
	int m_x;
	int m_y;

	explicit Point(int x = 0, int y = 0) :m_x(x), m_y(y){}
	Point(const Point &p){ m_x = p.m_x; m_y = p.m_y; }
	Point &operator=(Point &p){ m_x = p.m_x; m_y = p.m_y; return *this; }
	
	Point GetPoint(){ return Point(m_x, m_y); }
	void SetPoint(int x, int y){ m_x = x; m_y = y; }
	void SetPoint(Point &p){ m_x = p.m_x; m_y = p.m_y; }
	bool operator==(const Point &p)const{ return (m_x == p.m_x && m_y == p.m_y); }
};

//自定义全局变量
extern const int TIMER;
extern const int g_nBoard;
extern const int g_nSideOfGrid;
extern const int g_nSideOfGame;
extern const int g_nInfoX;

//函数声明
void Align(HWND hWnd);
void DrawBK(HDC hdc);
void DrawInfo(HDC hdc);
void Random(int &x, int &y);
void InitGame();
void OverGame(HWND hwnd);
void StopGame(HWND hwnd);
#endif