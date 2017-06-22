// Snake.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Snake.h"
#include "SnakeC.h"
#include <ctime>

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//�Զ���ȫ�ֱ���
extern const int TIMER = 1;
extern const int g_nBoard = 10;
extern const int g_nSideOfGrid = 10;
extern const int g_nSideOfGame = 30;
extern const int g_nInfoX = 10;
int g_nWidth;
int g_nHeight;
SnakeC *g_snake;
Food *g_food;
wchar_t g_cScore[10];
wchar_t g_cSpeed[10];
HWND g_hScore, g_hScoreEdit, g_hSpeed, g_hSpeedEdit, g_hStop;
bool g_bStop;

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
	HDC hdc, memDc;
	HBITMAP bitmap;

	switch (message)
	{
	case WM_CREATE:
		Align(hWnd);
		InitGame();
		g_hScore = CreateWindow(L"STATIC", L"Score: ", WS_CHILD | WS_VISIBLE | BS_CENTER, 
			g_nBoard * 2 + g_nSideOfGame*g_nSideOfGrid + 10, g_nBoard + 30, 50, 20, 
			hWnd, NULL, GetModuleHandle(NULL), NULL);
		g_hScoreEdit = CreateWindow(L"EDIT", g_cScore, WS_CHILD | WS_VISIBLE | ES_RIGHT,
			g_nBoard * 2 + g_nSideOfGame*g_nSideOfGrid + 60, g_nBoard + 33, 30, 14,
			hWnd, NULL, GetModuleHandle(NULL), NULL);
		g_hSpeed = CreateWindow(L"STATIC", L"Speed: ", WS_CHILD | WS_VISIBLE | BS_CENTER,
			g_nBoard * 2 + g_nSideOfGame*g_nSideOfGrid + 10, g_nBoard + 100, 50, 20,
			hWnd, NULL, GetModuleHandle(NULL), NULL);
		g_hSpeedEdit = CreateWindow(L"EDIT", g_cSpeed, WS_CHILD | WS_VISIBLE | ES_RIGHT,
			g_nBoard * 2 + g_nSideOfGame*g_nSideOfGrid + 60, g_nBoard + 103, 30, 14,
			hWnd, NULL, GetModuleHandle(NULL), NULL);
		g_hStop = CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE | BS_CENTER,
			g_nBoard * 2 + g_nSideOfGame*g_nSideOfGrid + 25, g_nBoard + 170, 50, 20,
			hWnd, (HMENU)IDC_BTN_STOP, GetModuleHandle(NULL), NULL);
		SetTimer(hWnd, TIMER, g_snake->GetSpeed(), NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDC_BTN_STOP:
			StopGame(hWnd);
			break;
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
	case WM_TIMER:
		g_snake->Move(hWnd, *g_food);
		wsprintf(g_cScore, L"%d", g_snake->GetScore());
		SetWindowText(g_hScoreEdit, g_cScore);
		wsprintf(g_cSpeed, L"%d", g_snake->GetSpeed());
		SetWindowText(g_hSpeedEdit, g_cSpeed);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			g_snake->SetDirection(UP);
			break;
		case VK_LEFT:
			g_snake->SetDirection(LEFT);
			break;
		case VK_RIGHT:
			g_snake->SetDirection(RIGHT);
			break;
		case VK_DOWN:
			g_snake->SetDirection(DOWN);
			break;
		case VK_SPACE:
			StopGame(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		memDc = CreateCompatibleDC(hdc);
		bitmap = CreateCompatibleBitmap(hdc, g_nWidth, g_nHeight);
		SelectObject(memDc, bitmap);
		DrawBK(memDc);
		DrawInfo(memDc);
		g_snake->DrawSnake(hWnd, memDc);
		g_food->DrawFood(hWnd, memDc);
		BitBlt(hdc, 0, 0, g_nWidth, g_nHeight, memDc, 0, 0, SRCCOPY);
		DeleteObject(bitmap);
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
	int startX, startY;
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
	g_nWidth = gameW - clientW + g_nBoard * 3 + g_nSideOfGrid*(g_nSideOfGame + g_nInfoX);
	g_nHeight = gameH - clientH + g_nBoard * 2 + g_nSideOfGrid*g_nSideOfGame;

	startX = (screenW - g_nWidth) / 2;
	startY = (screenH - g_nHeight) / 2;

	MoveWindow(hWnd, startX, startY, g_nWidth, g_nHeight, TRUE);
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
	HBRUSH brushInfo = (HBRUSH)GetStockObject(WHITE_BRUSH);

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

void InitGame()
{
	g_snake = new SnakeC();
	g_food = new Food();
	g_bStop = false;
	ZeroMemory(g_cScore, sizeof(g_cScore));
	wsprintf(g_cScore, L"%d", g_snake->GetScore());
	ZeroMemory(g_cSpeed, sizeof(g_cSpeed));
	wsprintf(g_cSpeed, L"%d", g_snake->GetSpeed());
}

void OverGame(HWND hwnd)
{
	UINT res;

	KillTimer(hwnd, TIMER);
	delete g_snake;
	delete g_food;
	res = MessageBox(hwnd, L"You lost, start again?", L"Result", MB_YESNO);
	if (res == IDYES)
	{
		InitGame();
		SetTimer(hwnd, TIMER, g_snake->GetSpeed(), NULL);
	}
	else
		PostQuitMessage(0);
}

void StopGame(HWND hwnd)
{
	if (g_bStop)
	{
		SetTimer(hwnd, TIMER, g_snake->GetSpeed(), NULL);
		SetWindowText(g_hStop, L"Stop");
		g_bStop = false;
	}
	else
	{
		KillTimer(hwnd, TIMER);
		SetWindowText(g_hStop, L"Start");
		g_bStop = true;
	}
}