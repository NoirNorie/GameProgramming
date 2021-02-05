// GP021.cpp : 문자열 표시하기
// 디버그 용도의 문자열을 출력창에 표시

#include <windows.h>
#include <stdio.h>
HWND hWndMain = 0;

#if defined(_DEBUG) && defined(WIN32) && !defined(_AFX) && !defined(_AFXDLL)
#define TRACE TRACE_WIN32
void TRACE_WIN32(LPCTSTR lpszFormat, ...)
{
	TCHAR lpszBuffer[0x160];
	va_list fmtList;
	va_start(fmtList, lpszFormat);
	vsprintf_s(lpszBuffer, lpszFormat, fmtList);
	va_end(fmtList);
	OutputDebugString(lpszBuffer);
}
#endif


// 윈도우 프로시저 함수
// 메시지를 처리하는 함수로 코드는 응용에 따라 적절히 작성하면 된다
// 왼쪽 버튼이 눌러지면 메시지 상자를 표시하고 ESC를 누르면 창을 종료하도록 작성함
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, "Hello, World!", "Hello", MB_OK);
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

	TRACE("좌표 = (%d, %d)\n", LOWORD(lParam), HIWORD(lParam));

	return DefWindowProc(hWnd, msg, wParam, lParam);

}

// 윈도우 생성 함수
bool initWin(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;
	// 윈도우에 클래스를 등록하는 Win32 함수 호출
	// 생성될 윈도우의 여러 특성들을 가지는 구조체이며
	// 윈도우를 생성하기 전에 윈도우 클래스가 먼저 등록되어야 한다.

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// 윈도우 클래스 등록시 사용할 윈도우 프로시저 함수를 lpfnWndProc 필드에 지정
	// 윈도우 프로시저 함수는 윈도우로 보내는 메시지들을 처리하는 함수

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Hello";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass - failed", 0, 0);
		return false;
	}

	hWndMain = CreateWindow("Hello", "Hello", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	// 윈도우를 생성하는 Win32 함수

	if (!hWndMain)
	{
		MessageBox(0, "CreateWindow - failed", 0, 0);
		return false;
	}

	ShowWindow(hWndMain, nCmdShow);
	// 윈도우를 화면에 표시되도록 하는 함수
	UpdateWindow(hWndMain);
	// WM_PAINT메시지를 발생시켜 윈도우 클라이언트 영역이 업데이트 되도록 하는 함수
	return true;
}



// 메시지 루프 함수
/*
함수 메시지 루프를 포함하는 함수로 메시지 루프는 창이 생성된 이후
창으로 오는 메시지를 받아서 이를 윈도우 프로시저로 보내는 과정을
창이 종료될 때까지 반복하는 형태
*/
int runWin()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0) != 0)
		// GetMessage : 메시지 큐로부터 메시지를 하나 가져온다.
		// 메시지 큐에 처리되지 않은 메시지가 있따면 그 메시지를 가지고 리턴
		// 큐가 비어있으면 새로운 메시지가 발생될 때까지 리턴하지 않고 대기
	{
		TranslateMessage(&msg);
		// Translate Message : 문자키가 눌러져서 발생된 메시지에 대해 WM_CHAR 메시지를 새로 발생
		// 문자 메시지를 편리하게 처리할 수 있도록 한다
		DispatchMessage(&msg);
		// DispatchMessgae : 윈도우 프로시저 함수를 호출
	}
	return (int)msg.wParam;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
//WinMain에 대한 주석이 일치하지 않습니다 -> 프로젝트/속성/링커/시스템/하위시스템/콘솔 -> 창으로 변경
{
	if (!initWin(hInstance, nShowCmd))
	{
		MessageBox(0, "initWin - failed", "Error", MB_OK);
		return 0;
	}

	return runWin();
}