#include "stdafx.h" //�̸� �����ϵ� ���
#include "StudentID.h" //StudentID�� ���

// --------------- ����� --------------------
// ���� ����
HWND hWndMain = 0;
bool push = false;
bool pop = false;
std::string result;

//�Լ�
bool initWin(HINSTANCE hInstance, int nCmdShow);
int runWin();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd);
bool initWin(HINSTANCE hInstance, int nCmdShow);
std::string inRECT(int x, int y); // �簢�� ���� ��ġ�ߴ��� �ƴ����� �����ϴ� �Լ�
// --------------- ����� --------------------


// --------------- ������ --------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) //������ �Լ�: WinMain
{
	if (!initWin(hInstance, nShowCmd))
	{
		MessageBox(0, _T("initWin - failed"), _T("Error"), MB_OK);
		return 0;
	}
	return runWin();
}

bool initWin(HINSTANCE hInstance, int nCmdShow) //������ ���� �Լ�
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

int runWin() //�޽��� ���� �Լ�
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) //������ ���ν���
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
		if (push == true) // push ����
		{
			if (drawVector(hWnd) == -1) // -1�� ���Ϲ��� ��� 8�� �̻��� �ڽ��� �����ϹǷ� �޽��� ���
			{
				MessageBox(0, _T("�ִ� ���� �ʰ�"), _T("Be Advised"), MB_OK);
			}
			else //�ƴ� ��� vector�� ���� �����Ѵ�
			{
				RewriteVector(hWnd);
			}
		}
		else if (pop == true) //pop ����, ������ ���� ������ ���ʿ��ϴ�
		{
			setlastPosition(getlastPosition().second + 50);
		}

		//����� ���� ����
		push = false;
		pop = false;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}


std::string inRECT(int x, int y) // �簢�� ���� ��ġ�ߴ��� �ƴ����� �����ϴ� �Լ�
{
	std::pair<int, int> tmp = getlastPosition();

	if ((x >= tmp.first && x <= tmp.first + 200) && (y >= tmp.second && y <= tmp.second + 50))
	{
		return "POP";
	}
	return "PUSH";
}