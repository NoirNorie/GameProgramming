#define _CRT_SECURE_NO_WARNINGS // 경고를 무시하기 위해 사용
#include "HW2.h"

// --------------- 선언부 --------------------
// 변수
int maxsize = 8; // 최대 개수
int gap = 30; // 상단과 하단의 거리 차이

int centerx, centery = 0; // 전체 스크린의 중심x,y좌표 
int Vectorsize = 0; // 현재 학생정보 vector의 사이즈 

float Angle = 0; // 각도 조절 인자
float Size = 0; // 사이즈 조절 인자 

D2D_POINT_2F lastdown; // 가장 최근에 눌려진 좌표
D2D_POINT_2F current; // 현재 마우스 좌표 
D2D1_RECT_F insertrectangle;

// 플래그 변수
boolean inserttingflag = false; // 삽입 중
boolean inserttedflag = false; // 삽입 완료
boolean deletingflag = false; // 삭제 중
boolean deletedflag = false; // 삭제 완료

vector <student> vec; // 학생 데이터들이 담길 vector

// 함수
void dataPush(); // 데이터를 벡터에 삽입
D2D1_RECT_F getNextRectangle(int Vectorsize); // 다음 사각형의 정보를 반환할 함수

// --------------- 구현부 --------------------

// 생성자: 클래스 변수 초기화
DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pRenderTarget(NULL),
	m_pStrokeStyleDash(NULL),
	m_pOriginalShapeBrush(NULL),
	m_boundaryBrush(NULL),
	m_RectangleBrush(NULL),
	m_pFillBrush(NULL),
	m_pTextBrush(NULL),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL)
{
}

// 소멸자: 모든 인터페이스 객체를 반납
DemoApp::~DemoApp()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pStrokeStyleDash);
	SAFE_RELEASE(m_pOriginalShapeBrush);
	SAFE_RELEASE(m_boundaryBrush);
	SAFE_RELEASE(m_RectangleBrush);
	SAFE_RELEASE(m_pFillBrush);
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pTextFormat);
}

// 메시지 루프 함수: 윈도우 프로시저로 메시지를 보내는 역할
void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		// 윈도우에서 오는 메시지를 윈도우 프로시저로 보내는 과정 속에서 반복 호출
	}
}

// 초기화 함수
HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr;

	hr = CreateDeviceIndependentResources(); // 장치 독립적 자원 생성
	if (SUCCEEDED(hr))
	{
		// 자원 생성에 성공한 경우
		// RegisterClassEx, CreateWindow, ShowWindow, UpdateWindow를 순차적으로 호출한다.

		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName = L"HW2_201601607(이명진)";

		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow
		(
			L"HW2_201601607(이명진)", L" HW2_201601607(이명진)",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			800, 900, NULL, NULL, hInstance, this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}
	return hr;
}

// 진입점 함수는 Entry.CPP로 분리함

// 장치 독립적 자원을 생성하는 함수
// 팩토리 객체를 생성함
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat
		(
			L"Verdana", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			13.0f, L"en-us", &m_pTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// 점선 획 스타일 객체를 생성함.
		float dashes[] = { 10.0f, 2.0f };
		hr = m_pD2DFactory->CreateStrokeStyle(D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER,
				10.0f, D2D1_DASH_STYLE_CUSTOM, 0.0f),
			dashes, ARRAYSIZE(dashes), &m_pStrokeStyleDash
		);
	}
	return hr;
}

// 장치 의존적 자원을 생성하는 함수
HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_pD2DFactory->CreateHwndRenderTarget
		(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			// 변환 전 모양의 외곽선을 위한 붓을 생성.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_pOriginalShapeBrush);
		}

		if (SUCCEEDED(hr))
		{
			// 변환 후 영역을 채우는 용도의 붓을 생성.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pFillBrush);
		}

		if (SUCCEEDED(hr))
		{
			// 텍스트를 쓰기 위한 붓을 생성.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pTextBrush);
		}

		if (SUCCEEDED(hr))
		{
			// 변환 후 모양의 외곽선을 위한 붓을 생성.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Navy), &m_boundaryBrush);
		}
		if (SUCCEEDED(hr))
		{
			// 변환 후 모양의 외곽선을 위한 붓을 생성.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSkyBlue), &m_RectangleBrush);
		}
	}
	return hr;
}

// 장치 의존적 자원을 반납하는 함수
// CreateDeviceResources() 함수에서 생성한 모든 자원을 반납
void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_boundaryBrush);
	SAFE_RELEASE(m_pFillBrush);
	SAFE_RELEASE(m_pOriginalShapeBrush);
	SAFE_RELEASE(m_pTextBrush);
}

//윈도우 프로시저 함수
// 내용을 그리는 함수
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));
		/*	클래스 객체의 포인터를 접근하는 기능의 구현을 위한 함수
			부가적인 윈도우 메모리의 해당 오프셋에 주어진 값을 저장함
			- 1번 인자: 해당 윈도우
			- 2번 인자: 오프셋
		*/
		return 1;
	}

	DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
	/*
		GetWindowLongPtrW(): 부가적으로 윈도우 메모리의 값을 얻는 함수
	*/
	
	// 발생되는 메시지들에 대해 의도하는 작업을 수행함
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
		return 0;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
		return 0;

		// 좌클릭 해제에 대한 동작
		case WM_LBUTTONUP:
		{
			// 삽입, 삭제 동작을 표시하는 flag를 모두 초기화
			inserttingflag = false; 
			deletingflag = false;

			// 각도, 크기 초기화
			Angle = 0;
			Size = 0;
			break;
		}
		// 좌클릭에 대한 동작
		case WM_LBUTTONDOWN:
		{
			if (LOWORD(lParam) >= insertrectangle.left && LOWORD(lParam) <= insertrectangle.right
				&& HIWORD(lParam) >= insertrectangle.top && HIWORD(lParam) <= insertrectangle.bottom) { // 만약 상단의 가운데 사각형이 눌려졌으면 
				inserttingflag = true; // insertting모드를 켜준다.
				lastdown.x = LOWORD(lParam);
				lastdown.y = HIWORD(lParam);
			}
			else if (vec.size() == 0)
				break;
			else
			{
				D2D1_RECT_F lastRectangle = getNextRectangle(Vectorsize - 1); // 이전의 vector를 가리킨 경우라면
				if (LOWORD(lParam) >= lastRectangle.left && LOWORD(lParam) <= lastRectangle.right
					&& HIWORD(lParam) >= lastRectangle.top && HIWORD(lParam) <= lastRectangle.bottom)
				{
					// deleting모드를 켜준다.
					deletingflag = true;  
					lastdown.x = LOWORD(lParam);
					lastdown.y = HIWORD(lParam);
				}
			}
			break;
		}

		case WM_PAINT:
		{
			pDemoApp->OnRender();
			ValidateRect(hwnd, NULL);
			return 0;
		}

		// 마우스의 움직임에 따른 동작
		case WM_MOUSEMOVE:
		{
			// 현재 마우스의 윈도우 상 좌표
			current.x = LOWORD(lParam);
			current.y = HIWORD(lParam);

			// 삽입 동작이라면 
			if (inserttingflag == true)
			{
				// 만약 maxsize를 초과한다면
				if (Vectorsize >= maxsize)
				{
					inserttingflag = false; // inserttingflag를 false로 처리해준다. 
					::MessageBox(0, L"the number of box is 8, it's limit", L"Fatal Error", MB_OK);//메세지를 띄워준다.
					::MessageBeep(MB_OK);
					break;
				}
				D2D1_RECT_F currentVector = getNextRectangle(Vectorsize);
				if (LOWORD(lParam) >= currentVector.left && 
					LOWORD(lParam) <= currentVector.right &&
					HIWORD(lParam) >= currentVector.top &&
					HIWORD(lParam) <= currentVector.bottom)
				{
					// inserttingflag가 true이고 들어와야할 스택에 마우스 커서가 들어왔으면
					inserttedflag = true; // inserttedflag를 true로 맞춰준다. 
					Angle = 0; //Angle ,Size 초기화 
					Size = 0;
					inserttingflag = false;
				}
				InvalidateRect(hwnd, NULL, false);
			}
			// 제거 동작이라면
			else if (deletingflag == true)
			{
				if (LOWORD(lParam) >= insertrectangle.left && LOWORD(lParam) <= insertrectangle.right
					&& HIWORD(lParam) >= insertrectangle.top && HIWORD(lParam) <= insertrectangle.bottom)
				{
					// deletingflag가 true이고 insertingrectangle에 마우스 커서가 들어왔으면
					deletedflag = true; // deletedflag를 true로 맞춰준다.
					Angle = 0; //Angle,Size 초기화 
					Size = 0;
					deletingflag = false;
				}
				InvalidateRect(hwnd, NULL, false);
			}
			else
			{
				RECT rect;
				rect.left = 10; rect.top = 10.5; rect.right = 236; rect.bottom = 190.5;
				InvalidateRect(hwnd, &rect, true);
			}

			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 1;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = CreateDeviceResources();
	/*
		렌더링 시 항상 호출되는 함수
		- 최초 호출에는 장치 의존적 자원들을 생성
		- 이후에는 렌더 타겟이 유효, 동작하지 않음
	*/

	if (SUCCEEDED(hr)) // 렌더 타겟이 유요한 경우 실행됨
	{
		// 그리기를 시작함.
		m_pRenderTarget->BeginDraw();
		// 렌더타겟 변환을 항등 변환으로 리셋함.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		// 렌더타겟 내용을 클리어함.
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// deletingflag 또는 deletedflag가 true이면 잠시 vectorsize를 줄여준다.
		if (deletingflag == true || deletedflag == true)
		{ 
			Vectorsize--;
		}

		// 커스텀 비교자를 사용해서 student 구조체를 사용하는 vector를 정렬한다
		sort(vec.begin(), vec.end(), compare);

		// 정렬된 내용을 차례로 출력
		// vector의 내용이 변경된다면 그 내용이 반영되어 출력된다
		for (int i = 0; i < Vectorsize; i++)
		{
			m_pRenderTarget->FillRectangle(getNextRectangle(i), m_RectangleBrush); // 그다음 그려질 스택 위치를 얻어내서 사각형을 그려준다. 

			const char* studentName = vec[i].name.c_str(); // v[i].name을 const char*형으로 바꾼다. 
			
			TCHAR temp[15]; // const char*을 TCHAR형으로 바꿔야 하기 때문에 temp변수를 선언한다. 	
			memset(temp, 0, sizeof(temp)); // 초기화
			MultiByteToWideChar(CP_ACP, MB_COMPOSITE, studentName, -1, temp, 15);
			
			static WCHAR studentInfo[100]; // name과 점수를 한번에 넣어아 햐므로 WCHAR 배열형 변수를 선언한다. 
			swprintf_s(studentInfo, L"%s %d\n", temp, vec[i].score);

			m_pRenderTarget->DrawText(studentInfo, wcslen(studentInfo), m_pTextFormat, getNextRectangle(i), m_pTextBrush); // Text를 그려준다.
			m_pRenderTarget->DrawRectangle(getNextRectangle(i), m_boundaryBrush); // 상자 테두리를 그려준다.
		}

		// 줄어든 VectorSize를 증가
		if (deletingflag == true || deletedflag == true)
		{ 
			Vectorsize++;
		}

		drawInit();
		// 만약 삽입하고 있는과정이라면 드래그하는 동안의 상자를 그려준다. 
		if (inserttingflag == true)
		{ 
			drawTempInsertingRectangle(); 
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pRenderTarget->DrawRectangle(getNextRectangle(Vectorsize), m_pOriginalShapeBrush); 
			// 상자 위치를 표시해준다. 
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		if (deletingflag == true)
		{
			drawTempdeletingRectangle();

		}

		// 삽입 완료 시 
		if (inserttedflag == true)
		{
			Vectorsize++; // vectorsize 증가 
			dataPush(); // 벡터에 data를 넣어준다. 
			inserttedflag = false;
		}

		// 삭제 완료 시
		if (deletedflag == true)
		{ 
			Vectorsize--;
			vec.pop_back(); // 벡터에서 값을 빼온다. 
			deletedflag = false;
		}

		// 그리기 연산들을 제출함.
		m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
		// 자원 반환
	}
	return hr;
}

// 사각형을 그릴 부분
void DemoApp::drawTempInsertingRectangle()
{
	// 이동 조절
	D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(current.x - lastdown.x, current.y - lastdown.y);
	// 최근 눌려진 좌표에서 현재 좌표까지 거리만큼 이동시켜준다. 
	Size = ((current.y - lastdown.y) / (getNextRectangle(Vectorsize).top - 30)) + 1;
	// 본래 크기에 추가적인 크기를 더해야 하므로 +1 을 해줘야 
	// 목표 지점까지 얼마만큼 왔는지 비율을 구해서 Size를 구해줍니다.
	
	// 스케일 조절
	D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(Size, 1.0f), D2D1::Point2F(insertrectangle.left, insertrectangle.top));

	// 회전 조절
	Angle = (current.y - lastdown.y) / (getNextRectangle(Vectorsize).top - 30) * 360 * -1;
	D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(Angle, D2D1::Point2F((insertrectangle.right + insertrectangle.left) / 2, (insertrectangle.top + insertrectangle.bottom) / 2));
	
	// 조절을 적용
	m_pRenderTarget->SetTransform(scale * rotation * translation); // scale->rotation->translation 순
	m_pRenderTarget->FillRectangle(insertrectangle, m_pOriginalShapeBrush); //insertrectangled을 변환
}

void DemoApp::drawTempdeletingRectangle()
{
	Vectorsize--;
	D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(current.x - lastdown.x, current.y - lastdown.y); // 최근 눌려진 좌표에서 현재 좌표까지 거리만큼 이동시켜준다.
	
	if (current.y <= lastdown.y) // 만약 위로 드래깅 한다면
	{
		Size = (getNextRectangle(Vectorsize).top - 30) / ((lastdown.y - current.y) + (getNextRectangle(Vectorsize).top - 30));
		// 이 경우에는 가장 최근의 vector에서 축소되는 것이므로 가장 최근의 vector의 위치를 읽어와야 한다.
		// 목표 지점까지 얼마만큼 왔는지 비율을 구해서 Size를 구해줍니다. 
	}
	else //만약 아래로 드래깅 한다면
	{
		Size = ((current.y - lastdown.y) / (getNextRectangle(Vectorsize).top - 30)) + 1;
		// 삽입 할때와 동일한 공식을 적용 
	}
	
	D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(Size, 1.0f), D2D1::Point2F(lastdown.x, lastdown.y));
	// 읽어온 값을 기준으로 확대, 축소 동작을 수행
	
	Angle = (current.y - lastdown.y) / (getNextRectangle(Vectorsize).top - 30) * 360 * -1;
	D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(Angle, D2D1::Point2F(lastdown.x, lastdown.y));

	m_pRenderTarget->SetTransform(scale * rotation * translation);
	m_pRenderTarget->FillRectangle(getNextRectangle(Vectorsize), m_pOriginalShapeBrush); // 최근 입력된 박스를 기준으로 변환하여 그려줍니다.
	Vectorsize++;
}

void DemoApp::drawInit()
{
	m_pOriginalShapeBrush->SetOpacity(0.3); // brush에 투명도를 조정

	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize(); // rendertarget의 size를 이용하여 
	centerx = rtSize.width / 2; //전체 스크린의 중앙 점을 구해낸다.
	centery = rtSize.height / 2;

	// 렌더타겟 변환을 항등 변환으로 리셋함. 사각형을 그림.
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	insertrectangle = D2D1::Rect(centerx - 30, gap, centerx + 30, gap + 30); // 가운데 상단에
	m_pRenderTarget->FillRectangle(insertrectangle, m_pOriginalShapeBrush); // 삽입을 위한 조그만한 사각형을 그려줌 

	// 캡션 텍스트를 표시함.
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	WCHAR currentState[100];

	swprintf_s(currentState, L"마우스 X:%.2f\n마우스 Y:%.2f\n회전각도:%.2f\n크기조정 인자 :%.2f\n박스개수 : %d\n", current.x, current.y, Angle, Size, vec.size());

	m_pRenderTarget->DrawText(currentState, wcslen(currentState), m_pTextFormat, D2D1::RectF(10.0f, 10.5f, 236.0f, 190.5f), m_pTextBrush);
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

// 다음에 그려질 사각형 좌표를 얻는 함수
D2D1_RECT_F getNextRectangle(int Vectorsize) 
{
	float top = 870 - 30 * (Vectorsize + 2);
	return D2D1::RectF(centerx - 60, top, centerx + 60, top + 30);

	// 중심을 기준으로 좌-우로 120
	// 높이가 30
	// 1 : 4 비율이 나온다
}

// 데이터를 vector에 삽입
void dataPush() 
{
	student tmp = { rtnScore(), rtnName() };
	vec.push_back(tmp);
	// 임시로 student 구조체 변수를 하나 생성한 후 vector에 넣는 동작을 수행한다
}