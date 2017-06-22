#include "SnakeC.h"

SnakeC::SnakeC(int l, UINT sp, int sc, DIRECTION d)
	:m_nLength(l), m_nSpeed(sp), m_nScore(sc), m_eDir(d)
{
	int i;
	for (i = 0; i < m_nLength; i++)
	{
		m_lBody.push_front(Point(i + 4, 4));
	}
	m_pTmpTail.SetPoint(m_lBody.back());
}

void SnakeC::EatFood(Food &f)
{
	m_nLength++;
	m_nScore += 10;
	if (m_nSpeed > 100)
		m_nSpeed -= 25;
	m_lBody.push_back(m_pTmpTail);

	f.SetFood();
	std::list<Point>::const_iterator pos = m_lBody.cbegin();
	std::list<Point>::const_iterator end = m_lBody.cend();
	while (1)
	{
		for (pos; pos != end; ++pos)
		{
			if (f.GetFood() == *pos)
			{
				f.SetFood();
				break;
			}
		}
		if (pos == end)
			break;
	}
}

void SnakeC::Move(HWND hwnd, Food &f)
{
	Point p(m_lBody.front());
	switch (m_eDir)
	{
	case UP:
		p.SetPoint(p.m_x, p.m_y - 1);
		break;
	case DOWN:
		p.SetPoint(p.m_x, p.m_y + 1);
		break;
	case LEFT:
		p.SetPoint(p.m_x - 1, p.m_y);
		break;
	case RIGHT:
		p.SetPoint(p.m_x + 1, p.m_y);
		break;
	}
	if (Check(p))
	{
		m_lBody.push_front(p);
		m_pTmpTail = m_lBody.back();
		m_lBody.pop_back();
		if (p == f.GetFood())
		{
			EatFood(f);
			KillTimer(hwnd, TIMER);
			SetTimer(hwnd, TIMER, m_nSpeed, NULL);
		}
	}
	else
	{
		Dead(hwnd);
	}
}

bool SnakeC::Check(const Point &p)
{
	if (p.m_x < 0 || p.m_x >= g_nSideOfGame || p.m_y < 0 || p.m_y >= g_nSideOfGame)
	{
		return false;
	}
	std::list<Point>::const_iterator pos = m_lBody.cbegin();
	std::list<Point>::const_iterator end = m_lBody.cend();
	for (pos; pos != end; ++pos)
	{
		if (p == *pos)
			return false;
	}
	return true;
}

void SnakeC::Dead(HWND hwnd)
{
	OverGame(hwnd);
}

void SnakeC::SetDirection(DIRECTION dir)
{
	m_eDir = dir;
}

void SnakeC::DrawSnake(HWND hwnd, HDC hdc)
{
	std::list<Point>::const_iterator pos = m_lBody.cbegin();
	std::list<Point>::const_iterator end = m_lBody.cend();
	HPEN pen = CreatePen(BS_SOLID, 1, RGB(255, 0, 255));
	HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	for (pos; pos != end; ++pos)
	{
		Rectangle(hdc, g_nBoard + g_nSideOfGrid*pos->m_x, g_nBoard + g_nSideOfGrid*pos->m_y,
			g_nBoard + g_nSideOfGrid*(pos->m_x + 1), g_nBoard + g_nSideOfGrid*(pos->m_y + 1));
	}
	DeleteObject(pen);
	DeleteObject(brush);
}