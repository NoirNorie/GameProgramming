#include "stdafx.h" //미리 컴파일된 헤더
#include "StudentID.h" //StudentID의 헤더

// --------------- 선언부 --------------------
// 전역 변수
HWND hWndMain = 0;
bool push = false;
bool pop = false;
std::string result;

//함수
bool initWin(HINSTANCE hInstance, int nCmdShow);
int runWin();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd);
bool initWin(HINSTANCE hInstance, int nCmdShow);
std::string inRECT(int x, int y); // 사각형 안을 터치했는지 아닌지를 판정하는 함수
// --------------- 선언부 --------------------


// --------------- 구현부 --------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) //진입점 함수: WinMain
{
	if (!initWin(hInstance, nShowCmd))
	{
		MessageBox(0, _T("initWin - failed"), _T("Error"), MB_OK);
		return 0;
	}
	return runWin();
}

bool initWin(HINSTANCE hInstance, int nCmdShow) //윈도우 생성 함수
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("Hello");
	if (!RegisterClass(&wc))
	{
		MessageBox(0, _T("RegisterClass - failed"), 0, 0);
		return false;
	}
	hWndMain = CreateWindow(_T("Hello"), _T("Hello"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (!hWndMain)
	{
		MessageBox(0, _T("CreateWindow - failed"), 0, 0);
		return false;
	}
	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	return true;
}

int runWin() //메시지 루프 함수
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) //윈도우 프로시저
{
	init(hWnd);
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		result = inRECT(LOWORD(lParam), HIWORD(lParam));
		if (result == "PUSH")
		{
			push = true;
			InvalidateRect(hWnd, NULL, false);
		}
		else
		{
			pop = true;
			std::pair<int, int> tmp = getlastPosition();
			RECT rect;
			rect.left = tmp.first - 1;
			rect.right = tmp.first + 200 + 1;
			rect.top = tmp.second - 1;
			rect.bottom = tmp.second + 50 + 1;
			vectorpop();
			InvalidateRect(hWnd, &rect, true);
		}
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
	case WM_PAINT:
		if (push == true) // push 동작
		{
			if (drawVector(hWnd) == -1) // -1을 리턴받을 경우 8개 이상의 박스가 존재하므로 메시지 출력
			{
				MessageBox(0, _T("최대 개수 초과"), _T("Be Advised"), MB_OK);
			}
			else //아닌 경우 vector의 값을 갱신한다
			{
				RewriteVector(hWnd);
			}
		}
		else if (pop == true) //pop 동작, 별도의 갱신 동작이 불필요하다
		{
			setlastPosition(getlastPosition().second + 50);
		}

		//사용한 값을 리셋
		push = false;
		pop = false;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}


std::string inRECT(int x, int y) // 사각형 안을 터치했는지 아닌지를 판정하는 함수
{
	std::pair<int, int> tmp = getlastPosition();

	if ((x >= tmp.first && x <= tmp.first + 200) && (y >= tmp.second && y <= tmp.second + 50))
	{
		return "POP";
	}
	return "PUSH";
}