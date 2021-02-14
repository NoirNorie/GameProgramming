#include "stdafx.h"
#include "DrawRectangle.h"

// 생성자
DemoApp::DemoApp() :
	m_hwnd(NULL), m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL), m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{

}
//클래스 변수들을 NULL로 초기화

// 소멸자
DemoApp::~DemoApp()
{
	DiscardDeviceResources();
	SAFE_RELEASE(m_pDirect2dFactory);
}
//모든 인터페이스 객체들을 반납
//응용 프로그램의 종료 시에 모든 자원들을 반납한다
//DiscardDeviceResources 함수 = 모든 장치 의존적 자원을 반납하는 함수
//장치 독립적 자원인 D2D 팩토리도 필요 없으므로 반납

//메시지 루프 함수
void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) //창으로 오는 메시지를 받아 윈도우 프로시저로 보내는 과정이 반복된다.
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//초기화 함수
HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateDeviceIndependentResources();
	//장치 독립적 자원들을 생성

	if (SUCCEEDED(hr)) //장치 독립적 자원 생성에 성공한다면 윈도우 생성
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
		//윈도우를 생성하는 Win32 함수 1

		m_hwnd = CreateWindow(L"D2DDemoApp", L"Direct2D Demo Applicatio",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			640, 480, NULL, NULL, HINST_THISCOMPONENT, this);
		//윈도우를 생성하는 Win32 함수 2

		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			//윈도우를 생성하는 Win32 함수 3
			UpdateWindow(m_hwnd);
			//윈도우를 생성하는 Win32 함수 4
		}
	}

	return hr;
}

//진입점 함수
//DemoApp 클래스의 인스턴스 생성 후 Initialize, 메시지 루프 함수를 순차적으로 호출
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	//CoInitialize : 스레드에서 COM 라이브러리의 사용을 위한 함수 1
	//COM 라이브러리를 사용할 수 있도록 로딩 및 초기화
	{
		DemoApp app;
		
		if (SUCCEEDED(app.Initialize(hInstance)))
		{
			app.RunMessageLoop();
		}
		CoUninitialize();
		//CoUninitialize : 스레드에서 COM 라이브러리의 사용을 위한 함수 2
		//사용이 종료된 후 스레드에서 COM 라이브러리를 언로드, 자원을 반납
	}
	return 0;
}


//장치 독립적 자원들을 생성
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	// D2D1CreateFactory의 1번 인자 : 여러 스레드가 이 장치 관련 자원을 접근하는 것에 대한 동기화 지원 여부를 명시
	// SINGLE_THREADED : 단일 스레드 환경에서 최적의 성능을 가지게 할 경우 사용
	// MULTI_THREADED : CPU에서 여러 스레드가 이 장치 관련 자원을 동시에 처리하는 경우 사용
	return hr;
}

//장치 의존적 자원들을 생성
/*
	ID2D1HwndRenderTarget : 가장 대표적인 렌더 타겟
	스크린의 일부에 렌더링하는 기능을 제공한다

	렌더 타겟은 작업 수행을 위해 가능하면 GPU를 사용, 불가능할 시 CPU를 사용

	렌더 타겟의 행위 특성은 D2D1_RENDER_TARGET_TYPE_XXX 로 사용한다
	- D2D1_RENDER_TARGET_TYPE_DEFAULT : 디폴트 값으로 GPU, CPU를 사용
	- D2D1_RENDER_TARGET_TYPE_HARDWARE : GPU 전용
	- D2D1_RENDER_TARGET_TYPE_SOFTWARE : CPU 전용
*/

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	// 렌더 타겟 : 그리기 연산을 수행하는 중요한 추상화된 장치
	// 가장 먼저 생성되어야 할 장치 의존적 자원
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		//GetClientRect 함수 : 클라이언트 영역의 크기 rc를 구한다
		
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		//그리기 영역의 크기를 위한 변수인 size 초기화

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);
		/*
			CreateHwndRenderTarget : 렌더 타겟을 생성하는 함수
			1번 인자 : D2D1_RENDER_TARGET_PROPERTIES 구조체
			- 원격 디스플레이 옵션 명시
			- 렌더 주체(SW / HW), DPI 등을 명시한다
			- 옵션을 각각 명시하지 않고 디폴트 렌더 타겟 속성을 사용할 시 D2D1::RenderTargetProperties를 사용
		 
			2번 인자 : D2D1_HWND_RENDER_TARGET_PROPERTIES 구조체
			- 컨텐츠가 렌더될 HWND, 렌더 타겟의 초기 크기(픽셀 단위), 표현 옵션을 명시
			- HWND와 초기 크기를 명시하는 도움 함수인 D2D1::HwndRenderTargetProperties를 사용 시 편리
			- 클라이언트 영역과 동일 크기로 초기 크기를 명시해준다

			3번 인자 : 리턴될 렌더 타겟 객체 포인터의 주소

			렌더 타겟의 생성 함수를 호출, 렌더 타겟이 생성될 때 자원들은 GPU에서 할당된다
			- HW 가속 가능이 가용한 경우에 대해서만 된다
			렌더 타겟이 생성된 후 응용 프로그램이 종료되기 전까지 생성된 렌더 타겟을 유지해야 한다.

			실행 중간에 D2DERR_RECREATE_TARGET 에러 발생 시 렌더 타겟 및 관련 자원들을 재생성해야 한다

			렌더 타겟이 생성된 후 렌더 타겟을 사용해 브러시를 생성해야 한다
		*/

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush);
		}
		//CreateSolidColorBrush 함수 : 브러시를 생성하는 함수


		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush);
		}
	}
	return hr;
}

//장치 의존적 자원을 반납하는 함수
//CreateDeviceResources 함수에서 생성한 모든 자원들을 반납해야 한다.
//렌더 타겟을 반납할 경우 렌더 타겟으로 생성한 모든 자원들도 반드시 반납해야 한다.
void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pLightSlateGrayBrush);
	SAFE_RELEASE(m_pCornflowerBlueBrush);
}

//윈도우 메시지 처리를 위한 WndProc(윈도우 프로시저)함수 구현
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	//윈도우가 생성될 때 처음으로 함수가 실행될 시 if문으로 이동한다
	if (message == WM_CREATE)
	{
		//클래스 객체의 포인터를 저장한다
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		//lParam의 lpCreateParams 필드를 통해 클래스 객체의 포인터를 얻는다.
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));
		//클래스 객체의 포인터를 접근하는 기능의 구현을 위해서 사용하는 함수
		//SetWindowLongPtrW 함수 : 부가적인 윈도우 메모리의 해당 오프셋에 주어진 값을 저장
		//1번 인자 : 해당 윈도우를 명시
		//2번 인자 : 오프셋 명시, GWLP_USERDATA는 사용자 제공 데이터를 위한 영역의 오프셋 의미

		result = 1;
	}
	//윈도우가 생성된 이후 발생되는 메시지들에 대해서 else 문이 생성된다.
	else
	{
		
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;
		//GetWindowLongPtrW 함수 : 부가 윈도우 메모리의 값을 얻어온다

		if (pDemoApp)
		{
			//클래스 객체의 포인터가 확보된 후 발생되는 메시지들에 대해 작업 수행
			switch (message)
			{
			case WM_SIZE: //새 창의 크기를 가져온 후 다음 창의 크기를 재조절
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0; wasHandled = true; break;
			case WM_DISPLAYCHANGE: //WM_PAINT를 발생시키도록 하는 작업 수행
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0; wasHandled = true; break;
			case WM_PAINT: // 화면에 내용을 다시 그리도록 하는 작업 수행
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
				//ValidateRect : 바로 연속하여 WM_PAINT가 반복 발생하지 않도록 하는 함수
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
		CreateDeviceResources 함수는 렌더시 항상 호출
		- 첫 번째 호출 : 렌더 타겟을 비롯한 장치 의존적 자원들을 생성
		- 이후의 호출 : 렌더 타겟이 유효하므로 동작 없음
		
		CreateDeviceResources 함수 호출 이후에는 언제든 렌더 타겟이 항상 유효해야 한다.
	*/
	
	if (SUCCEEDED(hr)) //렌더 타겟이 유효할 시 동작
	{
		//모든 그리기 함수들은 반드시 BeginDraw() 함수와 EndDraw() 함수 사이에서 호출되야 한다

		m_pRenderTarget->BeginDraw(); //그리기를 시작하는 함수
		
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


		hr = m_pRenderTarget->EndDraw(); //그리기를 끝내는 함수
		/*
			EndDraw 함수의 리턴 값이 D2DERR_RECREATE_TARGET인 경우 렌더 타겟이 무효화 된 것을 의미
			- 디스플레이 해상도가 변경되거나 디스플레이 어댑터의 기능이 중지되는 등의 경우 무효화 된다
			- 무효화 된 경우 렌더 타겟과 그 외의 모든 장치 의존적 자원들을 다시 생성해야 한다
			- DiscardDeviceResources 함수를 호출해 장치 의존적 자원을 반납
			- 이후 CreateDeviceResources 함수가 수행되어 렌더 타겟이 유효하지 않으므로 장치 의존적 자원들을 생성하게 된다.

			EndDraw 함수의 리턴 값이 S_OK인 경우 정상적으로 그리기가 수행된 것
		*/
		
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

//창의 크기가 수정된 경우 렌더 타겟을 크기에 맞춰 수정하는 함수
void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
		//Resize() 함수가 실패할 경우 예외 처리를 할 필요 없다
		//실패하는 경우 다음 EndDraw 호출에서 실패해 관련 자원들을 재생성하기 때문
	}
}