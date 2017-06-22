#ifndef SNAKEC_H
#define SNAKEC_H

#include <list>
#include "Snake.h"
#include "Food.h"

enum DIRECTION{UP, DOWN, LEFT, RIGHT};

class SnakeC{
private:
	int m_nLength;
	UINT m_nSpeed;
	int m_nScore;
	Point m_pTmpTail;
	DIRECTION m_eDir;
	std::list<Point> m_lBody;

public:
	explicit SnakeC(int l = 4, UINT sp = 500, int sc = 0, DIRECTION d = RIGHT);
	~SnakeC(){}
	UINT GetSpeed(){ return m_nSpeed; }
	int GetScore(){ return m_nScore; }
	void EatFood(Food &f);
	void Move(HWND hwnd, Food &f);
	bool Check(const Point &p);
	void Dead(HWND hwnd);
	void SetDirection(DIRECTION dir);
	void DrawSnake(HWND hwnd, HDC hdc);
};

#endif