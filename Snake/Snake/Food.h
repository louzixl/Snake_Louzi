#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"

class Food{
private:
	Point m_p;
public:
	explicit Food() { Random(m_p.m_x, m_p.m_y); }
	~Food() {}
	Point &GetFood();
	void SetFood();
	void DrawFood(HWND hwnd, HDC hdc);
};

#endif