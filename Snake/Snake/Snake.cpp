// Snake.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Snake.h"
#include <ctime>

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//�Զ���ȫ�ֱ���
extern const int g_nBoard = 10;
extern const int g_nSideOfGrid = 10;
extern const int g_nSideOfGame = 30;
extern const int g_nInfoX = 10;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SNAKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKE));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SNAKE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		Align(hWnd);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		DrawBK(hdc);
		DrawInfo(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Align(HWND hWnd)
{
	int startX, startY, width, height;
	int screenW, screenH, gameW, gameH, clientW, clientH;
	RECT rect;

	screenW = GetSystemMetrics(SM_CXSCREEN);
	screenH = GetSystemMetrics(SM_CYSCREEN);

	GetWindowRect(hWnd, &rect);
	gameW = rect.right - rect.left;
	gameH = rect.bottom - rect.top;
	GetClientRect(hWnd, &rect);
	clientW = rect.right - rect.left;
	clientH = rect.bottom - rect.top;
	width = gameW - clientW + g_nBoard * 3 + g_nSideOfGrid*(g_nSideOfGame + g_nInfoX);
	height = gameH - clientH + g_nBoard * 2 + g_nSideOfGrid*g_nSideOfGame;

	startX = (screenW - width) / 2;
	startY = (screenH - height) / 2;

	MoveWindow(hWnd, startX, startY, width, height, TRUE);
}

void DrawBK(HDC hdc)
{
	HPEN penFrame = CreatePen(BS_SOLID, 2, RGB(0, 255, 255));
	HBRUSH brushGame = CreateSolidBrush(RGB(128, 128, 128));
	
	SelectObject(hdc, penFrame);
	SelectObject(hdc, brushGame);
	Rectangle(hdc, g_nBoard, g_nBoard, g_nBoard + g_nSideOfGrid*g_nSideOfGame, g_nBoard + g_nSideOfGrid*g_nSideOfGame);
	DeleteObject(penFrame);
	DeleteObject(brushGame);
}

void DrawInfo(HDC hdc)
{
	HPEN penFrame = CreatePen(BS_SOLID, 2, RGB(255, 255, 0));
	HBRUSH brushInfo = CreateSolidBrush(RGB(150, 200, 255));

	SelectObject(hdc, penFrame);
	SelectObject(hdc, brushInfo);
	Rectangle(hdc, g_nBoard * 2 + g_nSideOfGrid*g_nSideOfGame, g_nBoard,
		g_nBoard * 2 + g_nSideOfGrid*(g_nSideOfGame + g_nInfoX), g_nBoard + g_nSideOfGrid*g_nSideOfGame);
	DeleteObject(penFrame);
	DeleteObject(brushInfo);
}

void Random(int &x, int &y)
{
	srand(time(0));
	x = rand() % 30;
	y = rand() % 30;
}