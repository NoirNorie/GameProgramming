#include "stdafx.h"
#include "DrawRectangle.h"

// ������
DemoApp::DemoApp() :
	m_hwnd(NULL), m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL), m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{

}
//Ŭ���� �������� NULL�� �ʱ�ȭ

// �Ҹ���
DemoApp::~DemoApp()
{
	DiscardDeviceResources();
	SAFE_RELEASE(m_pDirect2dFactory);
}
//��� �������̽� ��ü���� �ݳ�
//���� ���α׷��� ���� �ÿ� ��� �ڿ����� �ݳ��Ѵ�
//DiscardDeviceResources �Լ� = ��� ��ġ ������ �ڿ��� �ݳ��ϴ� �Լ�
//��ġ ������ �ڿ��� D2D ���丮�� �ʿ� �����Ƿ� �ݳ�

//�޽��� ���� �Լ�
void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) //â���� ���� �޽����� �޾� ������ ���ν����� ������ ������ �ݺ��ȴ�.
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//�ʱ�ȭ �Լ�
HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateDeviceIndependentResources();
	//��ġ ������ �ڿ����� ����

	if (SUCCEEDED(hr)) //��ġ ������ �ڿ� ������ �����Ѵٸ� ������ ����
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";
		RegisterClassEx(&wcex);
		//�����츦 �����ϴ� Win32 �Լ� 1

		m_hwnd = CreateWindow(L"D2DDemoApp", L"Direct2D Demo Applicatio",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			640, 480, NULL, NULL, HINST_THISCOMPONENT, this);
		//�����츦 �����ϴ� Win32 �Լ� 2

		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			//�����츦 �����ϴ� Win32 �Լ� 3
			UpdateWindow(m_hwnd);
			//�����츦 �����ϴ� Win32 �Լ� 4
		}
	}

	return hr;
}

//������ �Լ�
//DemoApp Ŭ������ �ν��Ͻ� ���� �� Initialize, �޽��� ���� �Լ��� ���������� ȣ��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	//CoInitialize : �����忡�� COM ���̺귯���� ����� ���� �Լ� 1
	//COM ���̺귯���� ����� �� �ֵ��� �ε� �� �ʱ�ȭ
	{
		DemoApp app;
		
		if (SUCCEEDED(app.Initialize(hInstance)))
		{
			app.RunMessageLoop();
		}
		CoUninitialize();
		//CoUninitialize : �����忡�� COM ���̺귯���� ����� ���� �Լ� 2
		//����� ����� �� �����忡�� COM ���̺귯���� ��ε�, �ڿ��� �ݳ�
	}
	return 0;
}


//��ġ ������ �ڿ����� ����
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	// D2D1CreateFactory�� 1�� ���� : ���� �����尡 �� ��ġ ���� �ڿ��� �����ϴ� �Ϳ� ���� ����ȭ ���� ���θ� ���
	// SINGLE_THREADED : ���� ������ ȯ�濡�� ������ ������ ������ �� ��� ���
	// MULTI_THREADED : CPU���� ���� �����尡 �� ��ġ ���� �ڿ��� ���ÿ� ó���ϴ� ��� ���
	return hr;
}

//��ġ ������ �ڿ����� ����
/*
	ID2D1HwndRenderTarget : ���� ��ǥ���� ���� Ÿ��
	��ũ���� �Ϻο� �������ϴ� ����� �����Ѵ�

	���� Ÿ���� �۾� ������ ���� �����ϸ� GPU�� ���, �Ұ����� �� CPU�� ���

	���� Ÿ���� ���� Ư���� D2D1_RENDER_TARGET_TYPE_XXX �� ����Ѵ�
	- D2D1_RENDER_TARGET_TYPE_DEFAULT : ����Ʈ ������ GPU, CPU�� ���
	- D2D1_RENDER_TARGET_TYPE_HARDWARE : GPU ����
	- D2D1_RENDER_TARGET_TYPE_SOFTWARE : CPU ����
*/

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	// ���� Ÿ�� : �׸��� ������ �����ϴ� �߿��� �߻�ȭ�� ��ġ
	// ���� ���� �����Ǿ�� �� ��ġ ������ �ڿ�
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		//GetClientRect �Լ� : Ŭ���̾�Ʈ ������ ũ�� rc�� ���Ѵ�
		
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		//�׸��� ������ ũ�⸦ ���� ������ size �ʱ�ȭ

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);
		/*
			CreateHwndRenderTarget : ���� Ÿ���� �����ϴ� �Լ�
			1�� ���� : D2D1_RENDER_TARGET_PROPERTIES ����ü
			- ���� ���÷��� �ɼ� ���
			- ���� ��ü(SW / HW), DPI ���� ����Ѵ�
			- �ɼ��� ���� ������� �ʰ� ����Ʈ ���� Ÿ�� �Ӽ��� ����� �� D2D1::RenderTargetProperties�� ���
		 
			2�� ���� : D2D1_HWND_RENDER_TARGET_PROPERTIES ����ü
			- �������� ������ HWND, ���� Ÿ���� �ʱ� ũ��(�ȼ� ����), ǥ�� �ɼ��� ���
			- HWND�� �ʱ� ũ�⸦ ����ϴ� ���� �Լ��� D2D1::HwndRenderTargetProperties�� ��� �� ��
			- Ŭ���̾�Ʈ ������ ���� ũ��� �ʱ� ũ�⸦ ������ش�

			3�� ���� : ���ϵ� ���� Ÿ�� ��ü �������� �ּ�

			���� Ÿ���� ���� �Լ��� ȣ��, ���� Ÿ���� ������ �� �ڿ����� GPU���� �Ҵ�ȴ�
			- HW ���� ������ ������ ��쿡 ���ؼ��� �ȴ�
			���� Ÿ���� ������ �� ���� ���α׷��� ����Ǳ� ������ ������ ���� Ÿ���� �����ؾ� �Ѵ�.

			���� �߰��� D2DERR_RECREATE_TARGET ���� �߻� �� ���� Ÿ�� �� ���� �ڿ����� ������ؾ� �Ѵ�

			���� Ÿ���� ������ �� ���� Ÿ���� ����� �귯�ø� �����ؾ� �Ѵ�
		*/

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush);
		}
		//CreateSolidColorBrush �Լ� : �귯�ø� �����ϴ� �Լ�


		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush);
		}
	}
	return hr;
}

//��ġ ������ �ڿ��� �ݳ��ϴ� �Լ�
//CreateDeviceResources �Լ����� ������ ��� �ڿ����� �ݳ��ؾ� �Ѵ�.
//���� Ÿ���� �ݳ��� ��� ���� Ÿ������ ������ ��� �ڿ��鵵 �ݵ�� �ݳ��ؾ� �Ѵ�.
void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pLightSlateGrayBrush);
	SAFE_RELEASE(m_pCornflowerBlueBrush);
}

//������ �޽��� ó���� ���� WndProc(������ ���ν���)�Լ� ����
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	//�����찡 ������ �� ó������ �Լ��� ����� �� if������ �̵��Ѵ�
	if (message == WM_CREATE)
	{
		//Ŭ���� ��ü�� �����͸� �����Ѵ�
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		//lParam�� lpCreateParams �ʵ带 ���� Ŭ���� ��ü�� �����͸� ��´�.
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));
		//Ŭ���� ��ü�� �����͸� �����ϴ� ����� ������ ���ؼ� ����ϴ� �Լ�
		//SetWindowLongPtrW �Լ� : �ΰ����� ������ �޸��� �ش� �����¿� �־��� ���� ����
		//1�� ���� : �ش� �����츦 ���
		//2�� ���� : ������ ���, GWLP_USERDATA�� ����� ���� �����͸� ���� ������ ������ �ǹ�

		result = 1;
	}
	//�����찡 ������ ���� �߻��Ǵ� �޽����鿡 ���ؼ� else ���� �����ȴ�.
	else
	{
		
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;
		//GetWindowLongPtrW �Լ� : �ΰ� ������ �޸��� ���� ���´�

		if (pDemoApp)
		{
			//Ŭ���� ��ü�� �����Ͱ� Ȯ���� �� �߻��Ǵ� �޽����鿡 ���� �۾� ����
			switch (message)
			{
			case WM_SIZE: //�� â�� ũ�⸦ ������ �� ���� â�� ũ�⸦ ������
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0; wasHandled = true; break;
			case WM_DISPLAYCHANGE: //WM_PAINT�� �߻���Ű���� �ϴ� �۾� ����
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0; wasHandled = true; break;
			case WM_PAINT: // ȭ�鿡 ������ �ٽ� �׸����� �ϴ� �۾� ����
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
				//ValidateRect : �ٷ� �����Ͽ� WM_PAINT�� �ݺ� �߻����� �ʵ��� �ϴ� �Լ�
			}
			result = 0; wasHandled = true; break;
			case WM_DESTROY: //
			{
				PostQuitMessage(0);
			}
			result = 1; wasHandled = true; break;
			}// switch
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}

	}
	return result;
}


HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();
	/*
		CreateDeviceResources �Լ��� ������ �׻� ȣ��
		- ù ��° ȣ�� : ���� Ÿ���� ����� ��ġ ������ �ڿ����� ����
		- ������ ȣ�� : ���� Ÿ���� ��ȿ�ϹǷ� ���� ����
		
		CreateDeviceResources �Լ� ȣ�� ���Ŀ��� ������ ���� Ÿ���� �׻� ��ȿ�ؾ� �Ѵ�.
	*/
	
	if (SUCCEEDED(hr)) //���� Ÿ���� ��ȿ�� �� ����
	{
		//��� �׸��� �Լ����� �ݵ�� BeginDraw() �Լ��� EndDraw() �Լ� ���̿��� ȣ��Ǿ� �Ѵ�

		m_pRenderTarget->BeginDraw(); //�׸��⸦ �����ϴ� �Լ�
		
		ID2D1StrokeStyle* m_pStrokeStyleDash;
		float dashes[] = { 10.0f, 2.0f };

		m_pDirect2dFactory->CreateStrokeStyle
		(D2D1::StrokeStyleProperties
		(D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_LINE_JOIN_MITER,
			10.0f,
			D2D1_DASH_STYLE_CUSTOM,
			0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&m_pStrokeStyleDash
		);

		ID2D1SolidColorBrush* m_pOriginalShapeBrush, * m_pFillBrush, * m_pTransformedShapeBrush;
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkBlue), &m_pOriginalShapeBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pFillBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkBlue), &m_pTransformedShapeBrush);

		D2D1_RECT_F rectangle = D2D1::Rect(438.0f, 301.5f, 498.0f, 361.5f);
		m_pRenderTarget->DrawRectangle(rectangle, m_pOriginalShapeBrush, 1.0f, m_pStrokeStyleDash);

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(45.0f, D2D1::Point2F(468.0f, 331.5f)));
		m_pRenderTarget->FillRectangle(rectangle, m_pFillBrush);
		m_pRenderTarget->DrawRectangle(rectangle, m_pTransformedShapeBrush);


		hr = m_pRenderTarget->EndDraw(); //�׸��⸦ ������ �Լ�
		/*
			EndDraw �Լ��� ���� ���� D2DERR_RECREATE_TARGET�� ��� ���� Ÿ���� ��ȿȭ �� ���� �ǹ�
			- ���÷��� �ػ󵵰� ����ǰų� ���÷��� ������� ����� �����Ǵ� ���� ��� ��ȿȭ �ȴ�
			- ��ȿȭ �� ��� ���� Ÿ�ٰ� �� ���� ��� ��ġ ������ �ڿ����� �ٽ� �����ؾ� �Ѵ�
			- DiscardDeviceResources �Լ��� ȣ���� ��ġ ������ �ڿ��� �ݳ�
			- ���� CreateDeviceResources �Լ��� ����Ǿ� ���� Ÿ���� ��ȿ���� �����Ƿ� ��ġ ������ �ڿ����� �����ϰ� �ȴ�.

			EndDraw �Լ��� ���� ���� S_OK�� ��� ���������� �׸��Ⱑ ����� ��
		*/
		
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

//â�� ũ�Ⱑ ������ ��� ���� Ÿ���� ũ�⿡ ���� �����ϴ� �Լ�
void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
		//Resize() �Լ��� ������ ��� ���� ó���� �� �ʿ� ����
		//�����ϴ� ��� ���� EndDraw ȣ�⿡�� ������ ���� �ڿ����� ������ϱ� ����
	}
}