#include "stdafx.h"
#include "DrawRectangle.h"

// 생성자
DemoApp::DemoApp() :
	m_hwnd(NULL), m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL), m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{

}

// 소멸자
DemoApp::~DemoApp()
{
	DiscardDeviceResources();
	SAFE_RELEASE(m_pDirect2dFactory);
}

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
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

		m_hwnd = CreateWindow(L"D2DDemoApp", L"Direct2D Demo Applicatio",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			640, 480, NULL, NULL, HINST_THISCOMPONENT, this);

		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		DemoApp app;
		
		if (SUCCEEDED(app.Initialize(hInstance)))
		{
			app.RunMessageLoop();
		}
		CoUninitialize();
	}
	return 0;
}


HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush);
		}
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pLightSlateGrayBrush);
	SAFE_RELEASE(m_pCornflowerBlueBrush);
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));
		result = 1;
	}
	else
	{
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0; wasHandled = true; break;
			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0; wasHandled = true; break;
			case WM_PAINT:
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0; wasHandled = true; break;
			case WM_DESTROY:
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
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		// 그리기 함수를 호출하는 부분

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(D2D1::Point2F(static_cast<FLOAT>(x), 0.0f), D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height), m_pLightSlateGrayBrush, 0.5f);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(D2D1::Point2F(static_cast<FLOAT>(y), 0.0f), D2D1::Point2F(static_cast<FLOAT>(y), rtSize.width), m_pLightSlateGrayBrush, 0.5f);
		}

		D2D1_RECT_F rectangle1 = D2D1::RectF(rtSize.width / 2 - 50.0f, rtSize.height / 2 - 50.0f, rtSize.width / 2 + 50.0f, rtSize.height / 2 + 50.0f);
		D2D1_RECT_F rectangle2 = D2D1::RectF(rtSize.width / 2 - 100.0f, rtSize.height / 2 - 100.0f, rtSize.width / 2 + 100.0f, rtSize.height / 2 + 100.0f);

		m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);
		m_pRenderTarget->FillRectangle(&rectangle2, m_pLightSlateGrayBrush);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}