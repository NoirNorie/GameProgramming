// GP022.cpp : GDI 사용하기
#include <Windows.h>
HWND hWndMain = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWndMain);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool initWin(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Hello";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass - failed", 0, 0);
		return false;
	}

	hWndMain = CreateWindow(L"Hello", L"Hello", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (!hWndMain)
	{
		MessageBox(0, L"CreateWindow - failed", 0, 0);
		return false;
	}

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	return true;

}

int runWin()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{

	if (!initWin(hInstance, nShowCmd))
	{
		MessageBox(0, L"initWin - failed", L"Error", MB_OK);
		return 0;
	}
	return runWin();
}