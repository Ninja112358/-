#include "MyGraphics.h"
#include <windows.h>
#include "debug.h"

#define WM_GAMEOVER 12345

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND hWnd;
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef SHOW_CONSOLE
	InitConsole();
#endif
	WNDCLASS wndclass;
	wchar_t szAppName[] = L"Ninja";
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)CreateSolidBrush(BACKGROUND_COLOR);
	wndclass.hInstance = hInstance;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, L"窗口注册失败", L"WARNING", MB_ICONERROR);
		exit(0);
	}
	//通过设定的视口大小，获取实际窗口的大小
	int realWidth = WINDOW_WIDTH;
	int realHeight = WINDOW_HEIGHT;
	RECT rect;
	rect.left = 0;
	rect.right = realWidth;
	rect.top = 0;
	rect.bottom = realHeight;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);
	//实际窗口的大小
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;
	//创建一个窗口并获取窗口句柄
	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		WINDOW_X, WINDOW_Y, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	//显示并更新窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	//消息机制
	MSG msg;
	while (GetMessage(&msg, 0, NULL, NULL)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
	return 0;
}
//此线程是游戏逻辑的主线程
DWORD WINAPI myThread(LPVOID lpParameter) {
	unsigned fuck = (unsigned)time(NULL);
	std::cout << fuck << std::endl;
	srand(fuck);
	bool flag = true;
	bool gameOverFlag = true;
	Update();
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	InvalidateRect(hWnd, NULL, TRUE);
	while (1) {
		if (flag) {
			if (!Update() && gameOverFlag) {
				//此时游戏结束,将游戏结束的消息发给WM_GAMEOVER,此消息是自定义消息
				SendMessage(hWnd, WM_GAMEOVER, NULL, NULL);
				gameOverFlag = false;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			if (PLAY_MODE == 2)
				flag = false;
		}
		if (PLAY_MODE == 0) {
			Sleep(DELAY);
			KeyDown();
		}
		else if (PLAY_MODE == 1)
			Sleep(AUTO_DELAY);
		else {
			Sleep(HAND_DELAY);
			if (GetAsyncKeyState(VK_RETURN))
				flag = true;
		}
	}
	return 0;
}
//此线程是游戏结束逻辑的线程
DWORD WINAPI gameOverThread(LPVOID lpParameter) {
	MessageBox(hWnd,L"撞墙了,游戏结束",L"提示",MB_ICONERROR);
	exit(0);
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_GAMEOVER:
		CreateThread(0, 0, gameOverThread, 0, 0, 0);
		break;
	case WM_ERASEBKGND:	//自动清屏的消息,这里不处理此消息是因为有双缓冲机制
		return 1;
	case WM_CREATE: {
		Start(hWnd);
		HANDLE handle = CreateThread(0, 0, myThread, 0, 0, 0);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		////双缓冲机制
		RECT rect;
		GetClientRect(hWnd, &rect);
		//创建内存DC
		HDC hMdc = CreateCompatibleDC(hdc);
		HBITMAP hBitMap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		SelectObject(hMdc, hBitMap);
		HBRUSH hBrush = CreateSolidBrush(BACKGROUND_COLOR);
		FillRect(hMdc, &rect, hBrush);
		//绘制图形
		Draw(hMdc);

		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMdc, 0, 0, SRCCOPY);
		DeleteObject(hBitMap);
		DeleteObject(hBrush);
		ReleaseDC(hWnd, hMdc);
		DeleteObject(hMdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE: {
		if (MessageBox(hWnd, L"您确定要关闭窗口吗", L"提示", MB_YESNO) == IDNO)
			return 1;
		else
			DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}