#include "Food.h"

Point &Food::GetFood()
{
	return m_p;
}

void Food::SetFood()
{
	Random(m_p.m_x, m_p.m_y);
}

void Food::DrawFood(HWND hwnd, HDC hdc)
{
	HPEN pen = (HPEN)GetStockObject(WHITE_PEN);
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, g_nBoard + g_nSideOfGrid * m_p.m_x, g_nBoard + g_nSideOfGrid * m_p.m_y,
		g_nBoard + g_nSideOfGrid * (m_p.m_x + 1), g_nBoard + g_nSideOfGrid * (m_p.m_y + 1));
	DeleteObject(pen);
	DeleteObject(brush);
}