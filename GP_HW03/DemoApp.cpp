#include "DemoApp.h"

// 투사체 관련 변수
int Angle = 30;
int Pow = 20;

int MOT_X = 50;
int MOT_Y = 15;

int startX = 100;
int startY = 400;


// 바람 상수
int windDirection = 0;
float windspeed = 0.0f;

// 바닥 위치
float Bottom = 480.0f;

// 타겟 관련 변수
int dist = 6000;

// 바람의 벡터를 구할 함수
float WindVector(float windspeed, int windDirection);
// 예상 경로 계산용
float Expected_Calc(int input_angle, int input_pow);
// 예상 베지에 포인트 계산용
float ExpPoint_Calc(int input_angle, int input_pow);
// 실제 경로 계산용
float Result_Calc(int input_angle, int input_pow, float windspeed, int windDirection);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize(hInstance)))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pWICFactory(NULL),
	m_pRenderTarget(NULL),
	// 비트맵
	m_pBitmap(NULL),
	m_pAnotherBitmap(NULL),
	m_pGroundBitmap(NULL),
	m_pExplosiveBitmap(NULL),
	m_pExpectedLocBitmap(NULL),
	m_pBitmapMask(NULL),

	m_pExpectedTGeometry(NULL),
	m_pExpectedLocBitmapBrush(NULL),

	m_pGroundBitmapBrush(NULL),
	m_ExplosiveBrush(NULL),
	m_pExplosiveBitmapBrush(NULL),

	// 경로 기하
	m_pExpectedGeometry(NULL),
	m_pProjectileGeometry(NULL),
	m_pTestGeometry(NULL),

	//브러쉬
	m_pGreenBrush(NULL),
	m_pDarkBrush(NULL),

	//m_pRadialFadeExplosiveBitmap(NULL),
	m_pRadialGradientBrush(NULL),

	//애니메이션
	m_Animation()
{
}

DemoApp::~DemoApp()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pRenderTarget);
	// 비트맵
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pAnotherBitmap);
	SAFE_RELEASE(m_pGroundBitmap);
	SAFE_RELEASE(m_pExpectedLocBitmap);

	SAFE_RELEASE(m_pExplosiveBitmap);
	SAFE_RELEASE(m_pBitmapMask);

	SAFE_RELEASE(m_pGroundBitmapBrush);
	SAFE_RELEASE(m_ExplosiveBrush);
	SAFE_RELEASE(m_pExplosiveBitmapBrush);

	SAFE_RELEASE(m_pExpectedTGeometry);
	SAFE_RELEASE(m_pExpectedLocBitmapBrush);

	// 경로 기하
	SAFE_RELEASE(m_pExpectedGeometry);
	SAFE_RELEASE(m_pProjectileGeometry);
	SAFE_RELEASE(m_pTestGeometry);

	//SAFE_RELEASE(m_pRadialFadeExplosiveBitmap);
	SAFE_RELEASE(m_pRadialGradientBrush);

	//브러쉬
	SAFE_RELEASE(m_pGreenBrush);
	SAFE_RELEASE(m_pDarkBrush);

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
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"D2DDemoApp";
		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			L"D2DDemoApp", L"Direct2D Demo Application",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			1280, 720, NULL, NULL, hInstance, this);
		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			float length = 0;
			hr = m_pExpectedGeometry->ComputeLength(NULL, &length);

			if (SUCCEEDED(hr))
			{
				m_Animation.SetStart(0); //start at beginning of path
				m_Animation.SetEnd(length); //length at end of path
				m_Animation.SetDuration(5.0f); //seconds

				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);
			}
		}
	}

	QueryPerformanceFrequency(&m_nFrequency);
	QueryPerformanceCounter(&m_nPrevTime);

	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 23;
	ID2D1GeometrySink* pSink = NULL;
	HRESULT hr;

	// D2D 팩토리를 생성함.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	// WIC 팩토리를 생성함.
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	}


	//DirectWrite 텍스트 포맷 객체 생성
	if (SUCCEEDED(hr))
	{
		// DirectWrite 팩토리를 생성함.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName, NULL, 
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize, L"", &m_pTextFormat
		);
	}
	SAFE_RELEASE(pSink);

	// 투사체 예상 경로 기하
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreatePathGeometry(&m_pExpectedGeometry);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pExpectedGeometry->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{

		D2D_POINT_2F MOT_Location = D2D1::Point2F(50, 18);
		D2D_POINT_2F Exp_Mid_Location = D2D1::Point2F(ExpPoint_Calc(Angle, Pow), -500);
		D2D_POINT_2F Expected_Location = D2D1::Point2F(Expected_Calc(Angle, Pow), 70);

		pSink->BeginFigure(MOT_Location, D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(Exp_Mid_Location, Expected_Location));
		pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		hr = pSink->Close();
	}
	SAFE_RELEASE(pSink);

	// 투사체 경로 기하 생성 (포탄)
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreatePathGeometry(&m_pProjectileGeometry);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pProjectileGeometry->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{
		pSink->BeginFigure(D2D1::Point2F(0.0f, 0.0f), D2D1_FIGURE_BEGIN_FILLED);

		const D2D1_POINT_2F ptTriangle[] = { {-10.0f, -10.0f}, {-10.0f, 10.0f}, {0.0f, 0.0f} };
		pSink->AddLines(ptTriangle, 3);

		pSink->EndFigure(D2D1_FIGURE_END_OPEN);

		hr = pSink->Close();
	}
	SAFE_RELEASE(pSink);

	// 예상 지점 경로 기하 생성
	if(SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				0.0f, -100.0f,
				200.0f, 0.0f
			),
			&m_pExpectedTGeometry
		);
	}

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

		// D2D 렌더타겟을 생성함.
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		// 응용 프로그램 리소스로부터 비트맵 객체 m_pBitmap를 생성함.
		hr = LoadBitmapFromResource(
			m_pRenderTarget, m_pWICFactory, 
			L"Background", L"Image", 1280, 500, 
			&m_pBitmap);

		// 외부 파일로부터 비트맵 객체 m_pAnotherBitmap를 생성함.
		// 머포 -> HW4에 다른 이미지로 대체함
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory, 
				L".\\MOT.png", 100, 0, 
				&m_pAnotherBitmap);
		}
		// 바닥 겸 상황판
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory, 
				L".\\asdf.jpg", 1265, 200, 
				&m_pGroundBitmap);
		}
		// 예상 도달지점 이미지
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\ExpectedLoc.png", 100, 100,
				&m_pExpectedLocBitmap);
		}
		// 폭발 이미지
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\explosive.png", 0, 0,
				&m_pExplosiveBitmap);
		}

		// 검은색 붓을 생성함
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black), &m_pDarkBrush);
		}

		// 불투명 마스크
		if (SUCCEEDED(hr))
		{
			D2D1_BITMAP_BRUSH_PROPERTIES propertiesXClampYClamp = D2D1::BitmapBrushProperties(
				D2D1_EXTEND_MODE_CLAMP, D2D1_EXTEND_MODE_CLAMP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(m_pGroundBitmap, propertiesXClampYClamp, &m_pGroundBitmapBrush);
			}	
		}
		if (SUCCEEDED(hr))
		{
			ID2D1GradientStopCollection* pGradientStops = NULL;

			static const D2D1_GRADIENT_STOP gradientStops[] =
			{
				{   0.f,  D2D1::ColorF(D2D1::ColorF::Black, 1.0f)  },
				{   1.f,  D2D1::ColorF(D2D1::ColorF::White, 0.0f)  },
			};

			hr = m_pRenderTarget->CreateGradientStopCollection(gradientStops, 2, &pGradientStops);

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(600, 75), D2D1::Point2F(0, 0), 1280, 450),
					pGradientStops, &m_pRadialGradientBrush);
			}
			pGradientStops->Release();
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateBitmapBrush(
				m_pExpectedLocBitmap, &m_pExpectedLocBitmapBrush
			);
			D2D1_RECT_F rect;
			m_pExpectedTGeometry->GetRect(&rect);
			m_pExpectedLocBitmapBrush->SetTransform(D2D1::Matrix3x2F::Translation({ rect.left, rect.top }));
		}


		// 텍스트에 사용할 붓 생성
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::IndianRed), 
				&m_pBlackBrush);
		}
		// 초록색 붓을 생성함.
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Green), 
				&m_pGreenBrush);
		}


	}

	return hr;
}


void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pRenderTarget);
	// 비트맵
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pAnotherBitmap);
	SAFE_RELEASE(m_pGroundBitmap);
	SAFE_RELEASE(m_pExpectedLocBitmap);

	SAFE_RELEASE(m_pExplosiveBitmap);
	SAFE_RELEASE(m_pBitmapMask);

	SAFE_RELEASE(m_pGroundBitmapBrush);
	SAFE_RELEASE(m_ExplosiveBrush);
	SAFE_RELEASE(m_pExplosiveBitmapBrush);

	SAFE_RELEASE(m_pExpectedTGeometry);
	SAFE_RELEASE(m_pExpectedLocBitmapBrush);

	// 경로 기하
	SAFE_RELEASE(m_pExpectedGeometry);
	SAFE_RELEASE(m_pProjectileGeometry);
	SAFE_RELEASE(m_pTestGeometry);

	//SAFE_RELEASE(m_pRadialFadeExplosiveBitmap);
	SAFE_RELEASE(m_pRadialGradientBrush);

	//브러쉬
	SAFE_RELEASE(m_pGreenBrush);
	SAFE_RELEASE(m_pDarkBrush);
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


HRESULT DemoApp::OnRender()
{
	HRESULT hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		// 타겟 정보 출력
		WCHAR currentTarget[37];
		swprintf_s(currentTarget,
			L"목표 거리 : %d\n\n바람 방향 : %s\n\n바람 속도 : %d",
			dist, L"무풍", 10 
			);

		// 포 정보 출력
		WCHAR currentMOT[36];
		swprintf_s(currentMOT,
			L"포탄 종류 : HE\n\n포탄 장약 : %d\n\n포신 각도 : %d ",
			Pow, Angle
		);

		// 점수 정보 출력
		WCHAR currentScore[32];
		swprintf_s(currentScore,
			L"점수 : %d\n\n타겟 명중 : %d\t  발사 탄수 : %d", 100, 5, 5);


		// 렌더타겟의 크기를 얻음.
		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		// 배경
		D2D1_SIZE_F size = m_pBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(m_pBitmap, 
			D2D1::RectF(0.0f, 0.0f, size.width, size.height));

		// 박격포
		D2D1_POINT_2F pos_MOT = D2D1::Point2F(75.0f, 375.0f);
		size = m_pAnotherBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(
			m_pAnotherBitmap,
			D2D1::RectF(
				pos_MOT.x, pos_MOT.y, 
				pos_MOT.x + size.width, pos_MOT.y + size.height)
		);

		// 상황판
		D2D1_POINT_2F pos_Ground = D2D1::Point2F(0.0f, 500.0f);
		size = m_pGroundBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(
			m_pGroundBitmap,
			D2D1::RectF(pos_Ground.x, pos_Ground.y,
				pos_Ground.x + size.width, pos_Ground.y + size.height)
		);


		// 타겟 관련
		m_pRenderTarget->DrawText(
			currentTarget, ARRAYSIZE(currentTarget) - 1,
			m_pTextFormat, D2D1::RectF(160, 510, renderTargetSize.width, renderTargetSize.height),
			m_pBlackBrush
		);

		// 조준 관련
		m_pRenderTarget->DrawText(
			currentMOT, ARRAYSIZE(currentMOT) - 1,
			m_pTextFormat, D2D1::RectF(470, 510, renderTargetSize.width, renderTargetSize.height),
			m_pBlackBrush
		);

		// 점수 관련
		m_pRenderTarget->DrawText(
			currentScore, ARRAYSIZE(currentScore) - 1,
			m_pTextFormat, D2D1::RectF(750, 550, renderTargetSize.width, renderTargetSize.height),
			m_pBlackBrush
		);

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(
			Expected_Calc(Angle, Pow) + 300, 520
		));
		m_pRenderTarget->FillGeometry(m_pExpectedTGeometry, m_pExpectedLocBitmapBrush, m_pRadialGradientBrush);

		// 애니메이션
		D2D1_POINT_2F point;
		D2D1_POINT_2F tangent;

		D2D1_MATRIX_3X2_F triangleMatrix;
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		float minWidthHeightScale = min(rtSize.width, rtSize.height) / 512;

		// 그래프 스케일
		D2D1::Matrix3x2F scale = 
			D2D1::Matrix3x2F::Scale(minWidthHeightScale, minWidthHeightScale);
		// 그래프 중앙점
		D2D1::Matrix3x2F translation = 
			D2D1::Matrix3x2F::Translation(pos_MOT.x, pos_MOT.y);


		m_pRenderTarget->SetTransform(scale * translation);
		m_pRenderTarget->DrawGeometry(m_pExpectedGeometry, m_pBlackBrush, 5);
		static float anim_time = 0.0f;
		float length = m_Animation.GetValue(anim_time);
		m_pExpectedGeometry->ComputePointAtLength(length, NULL, &point, &tangent);
		triangleMatrix = D2D1::Matrix3x2F(tangent.x, tangent.y, -tangent.y, tangent.x, point.x, point.y);

		// 삼각형을 초록색으로 그림.
		m_pRenderTarget->SetTransform(triangleMatrix*scale*translation);
		m_pRenderTarget->FillGeometry(m_pProjectileGeometry, m_pGreenBrush);
		m_pRenderTarget->DrawGeometry(m_pProjectileGeometry, m_pGreenBrush);


		// 그리기 연산 제출
		hr = m_pRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}

		// 애니메이션의 끝에 도달하면 다시 처음으로 되돌려서 반복되도록 함.
		if (anim_time >= m_Animation.GetDuration())
		{
			anim_time = 0.0f;
		}
		else
		{
			LARGE_INTEGER CurrentTime;
			QueryPerformanceCounter(&CurrentTime);

			float elapsedTime = 
				(float)((double)(CurrentTime.QuadPart - m_nPrevTime.QuadPart) 
					/ (double)(m_nFrequency.QuadPart));
			m_nPrevTime = CurrentTime;

			anim_time += elapsedTime;
		}
	}
	return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		D2D1_SIZE_U size;
		size.width = width;
		size.height = height;

		m_pRenderTarget->Resize(size);
	}
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
				result = 0;
				wasHandled = true;
				break;

				case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hwnd, NULL, FALSE);
				}
				result = 0;
				wasHandled = true;
				break;

				case WM_KEYDOWN:
				{
				switch (wParam)
				{
					// 포 각도 조절
				case VK_UP: // 각도 증가
				{
					if (Angle < 75) // 한계각(고각 = 75)을 넘어서는 각도 이상으로는 조절 불가
					{
						Angle++;
					}

					//InvalidateRect(hwnd, NULL, TRUE);
					break;
				}
				case VK_DOWN: // 각도 감소
				{
					if (Angle > 30) // 한계각(저각 = 30)을 넘어서는 각도 이하로는 조절 불가
					{
						Angle--;
					}
					//InvalidateRect(hwnd, NULL, TRUE);
					break;
				}
				// 장약량 조절
				case VK_RIGHT: // 장약 증가
				{
					if (Pow < 20) // 최대 장약은 20
					{
						Pow++;
					}
					//InvalidateRect(hwnd, NULL, TRUE);
					break;
				}
				case VK_LEFT: // 장약 감소
				{
					if (Pow > 1) // 최소 장약은 1
					{
						Pow--;
					}
					//InvalidateRect(hwnd, NULL, TRUE);
					break;
				}
				// 발사
				case VK_SPACE:
				{
					//InvalidateRect(hwnd, NULL, TRUE);
					return 0;
				}
				}
			}
				result = 0;
				wasHandled = true;
				break;

				case WM_PAINT:
				{
					pDemoApp->OnRender();
				}
				result = 0;
				wasHandled = true;
				break;

				case WM_DESTROY:
				{
					PostQuitMessage(0);
				}
				result = 1;
				wasHandled = true;
				break;

			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

// 바람의 벡터를 구할 함수
float WindVector(float windspeed, int windDirection)
{
	float result = 200 * sin((9*windspeed)*(PI/180)) * windDirection;

	return result;
}

// 예상 경로 계산용
float Expected_Calc(int input_angle, int input_pow)
{
	float result = 400 + (15 * input_pow) + (150 * sin((60 + input_angle) * (PI/180)));

	return result;
}
// 예상 베지에 포인트 계산용
float ExpPoint_Calc(int input_angle, int input_pow)
{
	float result = 200 + (7.5 * input_pow) + (75 * sin((60 + input_angle) * (PI / 180)));

	return result;
}

// 실제 경로 계산용
float Result_Calc(int input_angle, int input_pow, float windspeed, int windDirection)
{
	float result = Expected_Calc(input_angle, input_pow) + WindVector(windspeed, windDirection);

	return result;
}