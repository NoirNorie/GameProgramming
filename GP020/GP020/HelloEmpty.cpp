// GP020 : �⺻���� Win32 ���� ���α׷��� ����
// char* ������ �μ��� LPCWSTR ������ �Ű������� ȣȯ���� �ʽ��ϴ�
// -> ������Ʈ/�Ӽ�/�����Ӽ�/���/��������/�����ڵ� ���� ��� -> ��Ƽ����Ʈ ���� ������� ����

#include <windows.h>
HWND hWndMain = 0;

// ������ ���ν��� �Լ�
// �޽����� ó���ϴ� �Լ��� �ڵ�� ���뿡 ���� ������ �ۼ��ϸ� �ȴ�
// ���� ��ư�� �������� �޽��� ���ڸ� ǥ���ϰ� ESC�� ������ â�� �����ϵ��� �ۼ���
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
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

// ������ ���� �Լ�
bool initWin(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;
	// �����쿡 Ŭ������ ����ϴ� Win32 �Լ� ȣ��
	// ������ �������� ���� Ư������ ������ ����ü�̸�
	// �����츦 �����ϱ� ���� ������ Ŭ������ ���� ��ϵǾ�� �Ѵ�.

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// ������ Ŭ���� ��Ͻ� ����� ������ ���ν��� �Լ��� lpfnWndProc �ʵ忡 ����
	// ������ ���ν��� �Լ��� ������� ������ �޽������� ó���ϴ� �Լ�

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
	// �����츦 �����ϴ� Win32 �Լ�


	if (!hWndMain)
	{
		MessageBox(0, "CreateWindow - failed", 0, 0);
		return false;
	}

	ShowWindow(hWndMain, nCmdShow);
	// �����츦 ȭ�鿡 ǥ�õǵ��� �ϴ� �Լ�
	UpdateWindow(hWndMain);
	// WM_PAINT�޽����� �߻����� ������ Ŭ���̾�Ʈ ������ ������Ʈ �ǵ��� �ϴ� �Լ�
	return true;
}



// �޽��� ���� �Լ�
/*
�Լ� �޽��� ������ �����ϴ� �Լ��� �޽��� ������ â�� ������ ����
â���� ���� �޽����� �޾Ƽ� �̸� ������ ���ν����� ������ ������
â�� ����� ������ �ݺ��ϴ� ����
*/
int runWin()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	// GetMessage : �޽��� ť�κ��� �޽����� �ϳ� �����´�.
	// �޽��� ť�� ó������ ���� �޽����� �ֵ��� �� �޽����� ������ ����
	// ť�� ��������� ���ο� �޽����� �߻��� ������ �������� �ʰ� ���
	{
		TranslateMessage(&msg);
		// Translate Message : ����Ű�� �������� �߻��� �޽����� ���� WM_CHAR �޽����� ���� �߻�
		// ���� �޽����� ���ϰ� ó���� �� �ֵ��� �Ѵ�
		DispatchMessage(&msg);
		// DispatchMessgae : ������ ���ν��� �Լ��� ȣ��
	}
	return (int)msg.wParam;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
//WinMain�� ���� �ּ��� ��ġ���� �ʽ��ϴ� -> ������Ʈ/�Ӽ�/��Ŀ/�ý���/�����ý���/�ܼ� -> â���� ����
{
	if (!initWin(hInstance, nShowCmd))
	{
		MessageBox(0, "initWin - failed", "Error", MB_OK);
		return 0;
	}
	return runWin();
}

/*
������Ʈ�� �ҽ� �ڵ�� �ݵ�� �䱸�Ǵ� �ϳ��� ������ �Լ��� �����ؾ� �Ѵ�
������ �Լ� : ���� ���α׷��� ���۽ÿ� ����Ǵ� ������ ��ɾ �����ϴ� �Լ�
- �ܼ� ���� ���α׷� = main
- ������ ����ũ�� ���� ���α׷� = WinMain
-> �ش� �Լ����� �ٲ� �� ����
*/


//// DirectX ���� ���α׷����� �������� �ʾ� PeekMessage �Լ��� �����
//// ������� �ʰ� �޽��� �߻��� �����ϰ� ��� ���ϵ�
//int runWin()
//{
//	MSG msg;
//	ZeroMemory(&msg, sizeof(MSG));
//
//	while (msg.message != WM_QUIT)
//	{
//		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			render();
//			// DirectX ���� ���α׷����� ȭ���� �ѹ� �׸��� �Լ�
//		}
//	}
//}