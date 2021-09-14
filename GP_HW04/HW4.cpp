#include "stdafx.h"
#include "HW4.h"
#include "Character.h"
//#include "sound.h"
#include "SoundManager.h"

#define PI 3.141
#define NUM_ENEMIES 2
// 사운드 추가


// ---------------선언부---------------
// ------전역 변수------
// 투사체 기본 입력값
int Angle = 30;
int Pow = 20;
// 포 좌표
int MOT_X = 50;
int MOT_Y = 15;
// 시작 위치
int startX = 100;
int startY = 400;
// 바닥 위치
float Bottom = 480.0f;
// 바람 상수
int windDirection = 0;
float windspeed = 0.0f;
// 적 좌표
float MetoolPos_x = 650.0f;
float MetoolPos_y = 445.0f;
// 타겟 관련 변수
int dist = 0; // 거리
int score = 0; // 점수
// 포탄 수
int cnt = 3;

// AI

D2D_POINT_2F currentMousePosition = { 0, 100 };

ID2D1PathGeometry* pEnemy[5];
ID2D1PathGeometry* pViewRange[5];
ID2D1PathGeometry* pAttackRange[5];

/* 캐릭터 움직임을 위한 좌표 */
D2D1_POINT_2F move = { 0, 0 };

bool isAiShow = false;
bool MuteOn = false;


UINT windowWidth;

// 애니메이션 시간 함수
static float anim_time_launch = 0.0f;
float timeDelta;

// 플래그 변수들
bool isShow = false;

bool isStarted = true;
bool isStop = false;


bool Ready = false; // 각도 변경 상태
bool Shoot = false; // 발사 상태

bool Lost = false; // 관측 불능 판정
bool Land = false;
bool Hold = false;
bool Check = false;

bool Clear = false;

//사운드
SHORT input_str;
LONG volume;
//LPDIRECTSOUNDBUFFER g_lpDSBG[2] = { NULL, };
CSoundManager* soundManager; //사운드매니저

// ------전역 변수------

// ------전역 함수------
float WindVector();										// 바람의 벡터를 구할 함수
float Expected_Calc(int input_angle, int input_pow);	// 예상 경로 계산용
float ExpPoint_Calc(int input_angle, int input_pow);	// 예상 베지에 포인트 계산용
float Result_Calc(int input_angle, int input_pow);		// 실제 경로 계산용

// ------전역 함수------
// ---------------선언부---------------


// ---------------구현부---------------

// 생성자
DemoApp::DemoApp() : 
// 필수 변수들
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pWICFactory(NULL),
	m_pRenderTarget(NULL),
// 비트맵
	m_pCannonBitmap(NULL), // 포 비트맵
	m_pBGBitmap(NULL), // 배경 비트맵
	m_pGroundBitmap(NULL), // 상황판 겸 땅 비트맵
	m_pExplosiveBitmap(NULL), // 폭발 이펙트 비트맵
	m_pCannonBallBitmap(NULL), // 포탄 비트맵
	m_pBitmapMask(NULL), // 비트맵 마스크
	m_pMetoolBitmap(NULL), // 적을 그릴 비트맵
	metoolBitmap(NULL),
	m_GameOverBitmap(NULL), // 게임오버 비트맵
	m_GameClearBitmap(NULL), // 게임 클리어 비트맵
// 비트맵 브러쉬
	m_pGroundBitmapBrush(NULL),
	m_ExplosiveBrush(NULL),
	m_pExplosiveBitmapBrush(NULL),
	metoolBitmapBrush(NULL),
// 브러쉬
	m_pBlackBrush(NULL),
	m_pRangeBrush(NULL),
	m_pRealBrush(NULL),
	m_pRedBrush(NULL),
	m_pGreenBrush(NULL),
	m_pSolidColorBrush(NULL),
	m_pSceneBrush(NULL),
// Gradients
	m_pRadialGradientBrush(NULL),
// 경로 기하
	m_pExpectedGeometry(NULL), // 예상 경로 출력용
	m_pCannonGeometry(NULL), // 실제 경로 출력용
	m_pProjectileGeometry(NULL), // 투사체
// 텍스트
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL),

// 애니메이션
	m_Animation()
{}
// 소멸자
DemoApp::~DemoApp()
{
// 필수 변수들
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pRenderTarget);
// 비트맵
	SAFE_RELEASE(m_pCannonBitmap); // 포 비트맵
	SAFE_RELEASE(m_pBGBitmap); // 배경 비트맵
	SAFE_RELEASE(m_pGroundBitmap); // 상황판 겸 땅 비트맵
	SAFE_RELEASE(m_pExplosiveBitmap); // 폭발 이펙트 비트맵
	SAFE_RELEASE(m_pCannonBallBitmap); // 포탄 비트맵
	SAFE_RELEASE(m_pBitmapMask); // 비트맵 마스크
	SAFE_RELEASE(m_pMetoolBitmap); // 적을 그릴 비트맵
	SAFE_RELEASE(metoolBitmap);
	SAFE_RELEASE(m_GameOverBitmap); // 게임오버 비트맵
	SAFE_RELEASE(m_GameClearBitmap); // 게임 클리어 비트맵
// 비트맵 브러쉬
	SAFE_RELEASE(m_pGroundBitmapBrush);
	SAFE_RELEASE(m_ExplosiveBrush);
	SAFE_RELEASE(m_pExplosiveBitmapBrush);
	SAFE_RELEASE(metoolBitmapBrush);
// 브러쉬
	SAFE_RELEASE(m_pBlackBrush);
	SAFE_RELEASE(m_pRangeBrush);
	SAFE_RELEASE(m_pRealBrush);
	SAFE_RELEASE(m_pRedBrush);
	SAFE_RELEASE(m_pGreenBrush);
	SAFE_RELEASE(m_pSolidColorBrush);
	SAFE_RELEASE(m_pSceneBrush);
// Gradients
	SAFE_RELEASE(m_pRadialGradientBrush);
// 경로 기하
	SAFE_RELEASE(m_pExpectedGeometry); // 예상 경로 출력용
	SAFE_RELEASE(m_pCannonGeometry); // 실제 경로 출력용
	SAFE_RELEASE(m_pProjectileGeometry); // 투사체
}

// 초기화 함수
HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateDeviceIndependentResources();
	//hr = CreateDeviceResources();
	

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
		wcex.lpszClassName = "D2DDemoApp";
		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			"D2DDemoApp", "Direct2D Demo Application",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			1280, 720, NULL, NULL, hInstance, this);
		hr = m_hwnd ? S_OK : E_FAIL;


//// 사운드 추가
		if (SUCCEEDED(hr))
		{
			soundManager = new CSoundManager;
			if (!soundManager->init(m_hwnd))
				return FALSE;

			// 사운드 파일을 추가함 (id=0부터 시작함)
			int id;

			if (!soundManager->add(const_cast<char*>("Arabian_night.wav"), &id)) // 배경음악
				return FALSE;
			if (!soundManager->add(const_cast<char*>("Shoot.wav"), &id)) // 발사음
				return FALSE;
			if (!soundManager->add(const_cast<char*>("Explosion.wav"), &id)) // 폭발음
				return FALSE;
			if (!soundManager->add(const_cast<char*>("Angle_CTRL.wav"), &id)) // 각도 조절음
				return FALSE;
			if (!soundManager->add(const_cast<char*>("POW_CTRL.wav"), &id)) // 장약 조절음
				return FALSE;
			if (!soundManager->add(const_cast<char*>("RELOAD.wav"), &id)) // 장약 조절음
				return FALSE;
		}

		volume = 50;
		// 배경음악 재생
		if (SUCCEEDED(hr))
		{
			//soundManager->pDSBPrimary->SetVolume(vol);
			soundManager->pDSBPrimary->SetVolume(DSVOLUME_TO_DB(volume));
			//enemyCharacter->soundManager = soundManager;
			soundManager->play(0, TRUE);
		}

		if (SUCCEEDED(hr))
		{
			float length = 0;
			hr = m_pExpectedGeometry->ComputeLength(NULL, &length);

			if (SUCCEEDED(hr))
			{
				m_Animation.SetStart(0); //start at beginning of path
				m_Animation.SetEnd(length); //length at end of path
				m_Animation.SetDuration(2.0f); //seconds

				QueryPerformanceFrequency(&m_nFrequency);
				QueryPerformanceCounter(&m_nPrevTime);			
			}
			if (SUCCEEDED(hr))
			{
				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);
			}

			for (int i = 0; i < NUM_ENEMIES; i++)
			{
				DWORD enemyStateTransitions[][3] = 
				{
					{ Character::STATE_STAND, Character::EVENT_FINDTARGET, Character::STATE_RUNAWAY },
					{ Character::STATE_STAND, Character::EVENT_DUBIOUS, Character::STATE_MOVE },
					{ Character::STATE_MOVE, Character::EVENT_STOPWALK, Character::STATE_STAND },
				};

				Character* enemy = new Character(Character::TYPE_AI, enemyStateTransitions, 11);
				character.push_back(enemy);
			}
			setRandomSkullPosition();

			DWORD myStateTransitions[][3] =
			{
				{ Character::STATE_MOVE, Character::EVENT_UNDEFINED, Character::STATE_MOVE }
			};
			myCharacter = new Character(Character::TYPE_HUMAN, myStateTransitions, 1);
		}
	}
	return hr;
}

void DemoApp::DiscardDeviceResources()
{
// 필수 변수들
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pRenderTarget);
// 비트맵
	SAFE_RELEASE(m_pCannonBitmap); // 포 비트맵
	SAFE_RELEASE(m_pBGBitmap); // 배경 비트맵
	SAFE_RELEASE(m_pGroundBitmap); // 상황판 겸 땅 비트맵
	SAFE_RELEASE(m_pExplosiveBitmap); // 폭발 이펙트 비트맵
	SAFE_RELEASE(m_pCannonBallBitmap); // 포탄 비트맵
	SAFE_RELEASE(m_pBitmapMask); // 비트맵 마스크
	SAFE_RELEASE(m_pMetoolBitmap); // 적을 그릴 비트맵
// 비트맵 브러쉬
	SAFE_RELEASE(m_pGroundBitmapBrush);
	SAFE_RELEASE(m_ExplosiveBrush);
	SAFE_RELEASE(m_pExplosiveBitmapBrush);
// 브러쉬
	SAFE_RELEASE(m_pBlackBrush);
	SAFE_RELEASE(m_pRangeBrush);
	SAFE_RELEASE(m_pRealBrush);
	SAFE_RELEASE(m_pGreenBrush);
	SAFE_RELEASE(m_pSolidColorBrush);
	SAFE_RELEASE(m_pSceneBrush);
// Gradients
	SAFE_RELEASE(m_pRadialGradientBrush);
// 경로 기하
	SAFE_RELEASE(m_pExpectedGeometry); // 예상 경로 출력용
	SAFE_RELEASE(m_pCannonGeometry); // 실제 경로 출력용
	SAFE_RELEASE(m_pProjectileGeometry); // 투사체

	//delete[] g_lpDSBG;
	//DeleteDirectSound();
}

// 장치 의존적 자원 생성 함수
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 23;
	ID2D1GeometrySink* pSink = NULL;
	HRESULT hr;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	// D2D 팩토리를 생성함.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	
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

	//투사체 예상 경로 기하(곡선)
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
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 5; i++)
		{
			hr = m_pD2DFactory->CreatePathGeometry(&pViewRange[i]);
			hr = m_pD2DFactory->CreatePathGeometry(&pAttackRange[i]);
			SAFE_RELEASE(pSink);
			//ViewRange
			if (SUCCEEDED(hr))
			{
				hr = pViewRange[i]->Open(&pSink);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F StartPos = D2D1::Point2F(-90, 0);
				pSink->BeginFigure(StartPos, D2D1_FIGURE_BEGIN_FILLED);

				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(90, 0), D2D1::SizeF(90, 90), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-90, 0), D2D1::SizeF(90, 90), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
			//AttackRange
			if (SUCCEEDED(hr))
			{
				hr = pAttackRange[i]->Open(&pSink);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F StartPos = D2D1::Point2F(-30, 0);
				pSink->BeginFigure(StartPos, D2D1_FIGURE_BEGIN_FILLED);

				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(30, 0), D2D1::SizeF(30, 30), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-30, 0), D2D1::SizeF(30, 30), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}

	return hr;
}


// 메시지 루프 함수
void DemoApp::RunMessageLoop()
{
	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// 그리기 함수
HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F RTSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		// 배경
		D2D1_SIZE_F size = m_pBGBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(m_pBGBitmap,
			D2D1::RectF(0.0f, 0.0f, size.width, size.height));
		// 박격포
		D2D1_POINT_2F pos_MOT = D2D1::Point2F(75.0f, 375.0f);
		size = m_pCannonBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(
			m_pCannonBitmap,
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
		// 텍스트 출력
		int ws = WindVector();
		WCHAR currentTarget[37];
		swprintf_s(currentTarget,
			L"목표 거리 : %d\n\n바람 방향 : %s\n\n바람 속도 : %d ",
			dist, ws >= 0 ? (ws == 0 ? L"무풍" : L"순풍") : L"역풍", abs(ws)
		);
		// 타겟 관련
		m_pRenderTarget->DrawText(
			currentTarget, ARRAYSIZE(currentTarget) - 1,
			m_pTextFormat, D2D1::RectF(160, 510, RTSize.width, RTSize.height),
			m_pRedBrush
		);
		// 발사 정보 출력
		WCHAR currentMOT[36];
		swprintf_s(currentMOT,
			L"남은 포탄 : %d발\n\n포탄 장약 : %d\n\n포신 각도 : %d ",
			cnt, Pow, Angle
		);
		// 조준 관련
		m_pRenderTarget->DrawText(
			currentMOT, ARRAYSIZE(currentMOT) - 1,
			m_pTextFormat, D2D1::RectF(470, 510, RTSize.width, RTSize.height),
			m_pRedBrush
		);

		// 상황에 따른 텍스트 출력
		WCHAR currentBall[7];
		if (Shoot) // 발사 시 텍스트
		{
			swprintf_s(currentBall,
				L"포탄 발사");
		}
		else if (Shoot && Lost) // 관측 불능 텍스트
		{
			swprintf_s(currentBall,
				L"관측 불능");
		}
		else // 발사 정보 변경 시 텍스트
		{
			swprintf_s(currentBall,
				L"포탄 준비");
		}
		// 점수 관련
		m_pRenderTarget->DrawText(
			currentBall, ARRAYSIZE(currentBall) - 1,
			m_pTextFormat, D2D1::RectF(750, 550, RTSize.width, RTSize.height),
			m_pRedBrush
		);

		// 인공지능 그리기
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		drawAICharacter();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// 애니메이션
		D2D1_POINT_2F point;
		D2D1_POINT_2F tangent;
		D2D1_MATRIX_3X2_F triangleMatrix;
		float minWidthHeightScale = min(RTSize.width, RTSize.height) / 512;


		// 그래프
		D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(minWidthHeightScale, minWidthHeightScale);
		D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(pos_MOT.x, pos_MOT.y);


		// 포탄 발사 관련 출력
		if (isShow)
		{
			if (Ready) // 준비 상태 출력
			{
				// 곡선 출력 부분
				ID2D1GeometrySink* pSink = NULL;
				hr = m_pD2DFactory->CreatePathGeometry(&m_pExpectedGeometry);
				hr = m_pExpectedGeometry->Open(&pSink);

				D2D_POINT_2F MOT_Location = D2D1::Point2F(50, 18);
				D2D_POINT_2F Exp_Mid_Location = D2D1::Point2F(ExpPoint_Calc(Angle, Pow), -500);
				D2D_POINT_2F Expected_Location = D2D1::Point2F(Expected_Calc(Angle, Pow), 70);
				pSink->BeginFigure(MOT_Location, D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(Exp_Mid_Location, Expected_Location));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);
				hr = pSink->Close();
				SAFE_RELEASE(pSink);

				m_pRenderTarget->SetTransform(scale * translation);
				m_pRenderTarget->DrawGeometry(m_pExpectedGeometry, m_pRangeBrush, 5);

				static float anim_time = 0.0f;
				float length = m_Animation.GetValue(anim_time);
				m_pExpectedGeometry->ComputePointAtLength(length, NULL, &point, &tangent);
				triangleMatrix = D2D1::Matrix3x2F(tangent.x, tangent.y, -tangent.y, tangent.x, point.x, point.y);

				// 삼각형을 초록색으로 그림.
				m_pRenderTarget->SetTransform(triangleMatrix * scale * translation);
				m_pRenderTarget->FillGeometry(m_pProjectileGeometry, m_pGreenBrush);
				m_pRenderTarget->DrawGeometry(m_pProjectileGeometry, m_pGreenBrush);

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

			if (Shoot) // 발사 상태 출력
			{
				// 곡선 출력 부분
				ID2D1GeometrySink* pSink = NULL;
				hr = m_pD2DFactory->CreatePathGeometry(&m_pCannonGeometry);
				hr = m_pCannonGeometry->Open(&pSink);

				D2D_POINT_2F Launch_Start = D2D1::Point2F(50, 18);
				D2D_POINT_2F Launch_Bezier_Point = D2D1::Point2F(ExpPoint_Calc(Angle, Pow), -500);
				D2D_POINT_2F Launch_End = D2D1::Point2F(Result_Calc(Angle, Pow), 70);

				pSink->BeginFigure(Launch_Start, D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(Launch_Bezier_Point, Launch_End));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);
				hr = pSink->Close();
				SAFE_RELEASE(pSink);

				m_pRenderTarget->SetTransform(scale * translation);
				m_pRenderTarget->DrawGeometry(m_pCannonGeometry, m_pRealBrush, 5);

				//static float anim_time_launch = 0.0f;
				float length_launch = m_Animation.GetValue(anim_time_launch);
				m_pCannonGeometry->ComputePointAtLength(length_launch, NULL, &point, &tangent);

				triangleMatrix = D2D1::Matrix3x2F(tangent.x, tangent.y, -tangent.y, tangent.x, point.x, point.y);
				// 삼각형을 초록색으로 그림.
				m_pRenderTarget->SetTransform(triangleMatrix * scale * translation);
				m_pRenderTarget->FillGeometry(m_pProjectileGeometry, m_pGreenBrush);
				m_pRenderTarget->DrawGeometry(m_pProjectileGeometry, m_pGreenBrush);

				D2D1_POINT_2F pos_Ball = D2D1::Point2F(-15.0f, -15.0f);
				size = m_pCannonBallBitmap->GetSize();
				m_pRenderTarget->DrawBitmap(
					m_pCannonBallBitmap,
					D2D1::RectF(
						pos_Ball.x, pos_Ball.y,
						pos_Ball.x + size.width, pos_Ball.y + size.height)
				);
				LARGE_INTEGER CurrentTime;
				QueryPerformanceCounter(&CurrentTime);

				float elapsedTime =
					(float)((double)(CurrentTime.QuadPart - m_nPrevTime.QuadPart)
						/ (double)(m_nFrequency.QuadPart));
				m_nPrevTime = CurrentTime;
				anim_time_launch += elapsedTime;
				


				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				if (anim_time_launch >= m_Animation.GetDuration()) // 포탄이 끝까지 도착하면
				{
					Land = true; // 포탄이 땅에 도달함

						//score = 100 - abs((Result_Calc(Angle, Pow) + 150) - dist);
					score = 100 - abs((Result_Calc(Angle, Pow) * minWidthHeightScale) - dist);
					//float minWidthHeightScale = min(RTSize.width, RTSize.height) / 512;

					m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
					D2D1_MATRIX_3X2_F Exp;
					Exp = D2D1::Matrix3x2F::Translation(Launch_End.x, 250);
					m_pRenderTarget->SetTransform(Exp * scale);
					m_pRenderTarget->DrawBitmap(m_pExplosiveBitmap);

					// 측정
					WCHAR currentState[20];
					if (Land)
					{
						if (score >= 50)
						{
							swprintf_s(currentState, L"포탄 명중\t 정밀도 : %d   ", score);
							Clear = true;
						}
						else if (score < 50 && score > 9)
						{
							swprintf_s(currentState, L"효력 범위\t 정밀도 : %d   ", score);
						}
						else if (score < 9 && score > 0)
						{
							swprintf_s(currentState, L"효력 범위\t 정밀도 : %d    ", score);
						}
						else
						{
							swprintf_s(currentState, L"포탄 낙하\t 정밀도 : %d     ", 0);
						}
					}
					else
					{
						swprintf_s(currentState,
							L"발사 준비\t 정밀도 : --    ");
					}
					// 점수 관련
					m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
					m_pRenderTarget->DrawText(
						currentState, ARRAYSIZE(currentState) - 1,
						m_pTextFormat, D2D1::RectF(750, 610, RTSize.width, RTSize.height),
						m_pRedBrush
					);



					if (Check)
					{
						soundManager->play(2, FALSE);
						soundManager->play(5, FALSE);
						Check = false;
					}
				}
			}

			if (cnt == 0 && !Clear) // 명중하지 못했고 총알을 다 쓴 경우 게임 오버
			{
				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				D2D1_MATRIX_3X2_F cen = D2D1::Matrix3x2F::Translation(250, 100);
				m_pRenderTarget->SetTransform(cen);
				m_pRenderTarget->DrawBitmap(m_GameOverBitmap);
				for (int i = 0; i < 6; i++)
					soundManager->stop(i);
			}

			if (Clear) // 명중한 경우
			{
				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				D2D1_MATRIX_3X2_F cen = D2D1::Matrix3x2F::Translation(250, 100);
				m_pRenderTarget->SetTransform(cen);
				m_pRenderTarget->DrawBitmap(m_GameClearBitmap);
				for (int i = 0; i < 6; i++)
					soundManager->stop(i);
			}
		}




		// 그리기 연산 제출
		hr = m_pRenderTarget->EndDraw();


		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
	return hr;
}

// 윈도우 크기 재설정 함수
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

// 윈도우 프로시저 = 조작 관련
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
				windowWidth = width;
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

			case WM_MOUSEMOVE:
			{
				currentMousePosition.x = LOWORD(lParam);
				currentMousePosition.y = HIWORD(lParam);
				pDemoApp->myCharacter->setDestPosition(currentMousePosition);
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}

			case WM_KEYDOWN:
			{
				input_str = static_cast<SHORT>(wParam);
				switch (wParam)
				{
				// 배경음악 끄기
				case 'p':
				case 'P':
				{
					if (MuteOn)
					{
						soundManager->stop(0);
						MuteOn = false;
					}
					else
					{
						soundManager->play(0, TRUE);
						MuteOn = true;
					}
					break;
				}
				// 소리 증감
				case 'o':
				case 'O':
				{
					if (volume < 150)
					{
						volume += 5;
						soundManager->pDSBPrimary->SetVolume(DSVOLUME_TO_DB(volume));
					}			
					break;
				}
				case 'i':
				case 'I':
				{
					if (volume > 0)
					{
						volume -= 5;
						soundManager->pDSBPrimary->SetVolume(DSVOLUME_TO_DB(volume));
					}
					break;
				}

				// 포 각도 조절
				case VK_UP: // 각도 증가
				{
					isShow = true;
					Ready = true;
					Shoot = false;
					Hold = true;
					Land = false;
					if (Angle < 75) // 한계각(고각 = 75)을 넘어서는 각도 이상으로는 조절 불가
					{
						soundManager->play(3, FALSE);
						Angle++;
					}
					break;
				}
				case VK_DOWN: // 각도 감소
				{
					isShow = true;
					Ready = true;
					Shoot = false;
					Hold = true;
					Land = false;
					if (Angle > 30) // 한계각(저각 = 30)을 넘어서는 각도 이하로는 조절 불가
					{
						soundManager->play(3, FALSE);
						Angle--;
					}
					break;
				}
				// 장약량 조절
				case VK_RIGHT: // 장약 증가
				{
					isShow = true;
					Ready = true;
					Shoot = false;
					Hold = true;
					Land = false;
					if (Pow < 20) // 최대 장약은 20
					{
						soundManager->play(4, FALSE);
						Pow++;
					}
					break;
				}
				case VK_LEFT: // 장약 감소
				{
					isShow = true;
					Ready = true;
					Shoot = false;
					Hold = true;
					Land = false;
					if (Pow > 1) // 최소 장약은 1
					{
						soundManager->play(4, FALSE);
						Pow--;
					}
					break;
				}
				// 준비 + 발사
				case VK_SPACE:
				{
					if (Ready)
					{
						if (cnt > 0)
						{
							soundManager->play(1, FALSE);
							anim_time_launch = 0.0f;
							Ready = false;
							isShow = true;
							Shoot = true;
							Hold = true;
							Check = true;
							cnt--;
						}
					}
					return 0;
				}
				}

			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				if (!Clear)
				{
					pDemoApp->OnRender();
				}

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
float WindVector()
{
	srand((unsigned int)time(NULL));

	float result;
	if (Hold == true) // 바람 고정 상태(조작 중)
	{
		result = windspeed;
		return result;
	}
	else // 가변 상태
	{
		windspeed = rand() % 20 - 10;
		result = windspeed;
		return result;
	}
}
// 예상 경로 계산용
float Expected_Calc(int input_angle, int input_pow)
{
	float result = 200 + (30 * input_pow) + (150 * sin((60 + input_angle) * (PI / 180)));

	return result;
}
// 예상 베지에 포인트 계산용
float ExpPoint_Calc(int input_angle, int input_pow)
{
	float result = 100 + (15 * input_pow) + (75 * sin((60 + input_angle) * (PI / 180)));

	return result;
}
// 실제 경로 계산용
float Result_Calc(int input_angle, int input_pow)
{
	float result = 200 + (30 * input_pow) + (150 * sin((60 + input_angle) * (PI / 180)));

	result = result + WindVector() * 10 * sin((10 * abs(WindVector()) * (PI / 180)));

	return result;
}


void DemoApp::drawAICharacter()
{
	/* 메툴 그리기*/
	if (Character::isAttacked == true) {
		if ((time(NULL) - Character::lastAttackedTime) <= 5)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(1100, 100));
			m_pRenderTarget->FillRectangle(D2D1::RectF(0, 0, 200, 200), metoolBitmapBrush);
		}
		else {
			Character::isAttacked = false;
			setRandomSkullPosition();
		}
	}

	for (size_t i = 0; i < character.size(); i++)
	{
		WCHAR str[10];
		switch (character[i]->getColor())
		{
		case 0://Character.Black:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			swprintf_s(str, L"Stop    ");
			break;
		//case 1://Character.Red:
		//	m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		//	swprintf_s(str, L"run away");
		//	break;
		//case 2://Character.Blue:
		//	m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
		//	swprintf_s(str, L"following");
		//	break;
		case 3: //Character.Green
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
			swprintf_s(str, L"moving  ");
			break;
		default:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			break;
		}

		D2D1_SIZE_F mobSize = m_pRenderTarget->GetSize();
		m_pRenderTarget->FillRectangle(D2D1::RectF(0, 0, 20, 20), metoolBitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(character[i]->getPosition().x + 8, character[i]->getPosition().y + 12));
		m_pRenderTarget->DrawText(str, ARRAYSIZE(str) - 1,
			m_pTextFormat, D2D1::RectF(0, -15, mobSize.width, mobSize.height),
			m_pSceneBrush
		);
		
		dist = character[0]->getPosition().x;
		m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
		m_pRenderTarget->DrawGeometry(pViewRange[i], m_pSceneBrush, 0.3f);

		m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		m_pRenderTarget->DrawGeometry(pAttackRange[i], m_pSceneBrush, 0.3f);
	}
	for (std::vector<Character*>::iterator iter = character.begin(); iter != character.end(); iter++)
	{
		Character* ch = *iter;
		ch->update(myCharacter, 0.01);
	}
	

	//for (std::vector<Character*>::iterator iter = character.begin(); iter != character.end(); iter++)
	//{
	//	Character* ch = *iter;
	//	ch->update(myCharacter, 0.01);
	//}
	//myCharacter->update(myCharacter, 0.01);
						//{ Character::STATE_MOVE, Character::EVENT_FINDTARGET, Character::STATE_FOLLOW },
					//{ Character::STATE_STAND, Character::EVENT_BEATTACKED, Character::STATE_ATTACK },
					//{ Character::STATE_ATTACK, Character::EVENT_LOSTTARGET, Character::STATE_STAND },
					//{ Character::STATE_ATTACK, Character::EVENT_HEALTHDRAINED, Character::STATE_RUNAWAY },
					//{ Character::STATE_ATTACK, Character::EVENT_OUTOFATTACK, Character::STATE_FOLLOW },
					//{ Character::STATE_FOLLOW, Character::EVENT_WITHINATTACK, Character::STATE_ATTACK },
					//{ Character::STATE_FOLLOW, Character::EVENT_LOSTTARGET, Character::STATE_STAND },
					//{ Character::STATE_RUNAWAY, Character::EVENT_LOSTTARGET, Character::STATE_STAND },

}

void DemoApp::setRandomSkullPosition()
{
	int posx[NUM_ENEMIES], posy[NUM_ENEMIES], cx, cy;
	int k = 0;
	bool duplicated;
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		do {
			cx = rand() % 700 + 300;
			cy = 440;
			duplicated = false;
			if (cx < 0 || cy < 0 || cx > 1400 || cy> 800)
				duplicated = true;
			if (i != 0)
			{
				for (int j = 0; j < i; j++)
				{
					if (sqrt((float)(cx - posx[i]) * (cx - posx[i]) + (float)(cy - posy[i]) * (cy - posy[i])) < 10)
					{
						duplicated = true;
						break;
					}
				}
			}
		} while (duplicated);
		posx[k] = cx; posy[k] = cy; k++;
	}
	for (int i = 0; i < character.size(); i++) {
		character[i]->setPosition(D2D1::Point2F((float)posx[i], (float)posy[i]));
	}
}