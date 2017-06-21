#ifndef SNAKE_H
#define SNAKE_H

#include "resource.h"

//�ṹ��
struct Point{
	int x;
	int y;
};

//�Զ���ȫ�ֱ���
extern const int g_nBoard;
extern const int g_nSideOfGrid;
extern const int g_nSideOfGame;
extern const int g_nInfoX;

//��������
void Align(HWND hWnd);
void DrawBK(HDC hdc);
void DrawInfo(HDC hdc);
void Random(int &x, int &y);
#endif