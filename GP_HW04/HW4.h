#pragma once
#pragma warning(disable:4996)

#include "stdafx.h" // 미리 컴파일된 헤더
#include "Animation.h"
#include "Character.h"


class DemoApp
{
public:
	DemoApp(); // 기본 생성자
	~DemoApp(); // 기본 소멸자
	HRESULT Initialize(HINSTANCE hInstance); // 초기화 함수
	void RunMessageLoop(); // 메시지 루프 함수

private:
	HRESULT CreateDeviceResources();// 장치 의존적 자원 생성 함수
	void DiscardDeviceResources(); // 자원 제거 함수

	HRESULT CreateDeviceIndependentResources(); // 장치 독립적 자원 생성 함수

	HRESULT OnRender(); //화면에 출력하는 함수

	void OnResize(UINT width, UINT height); // 화면크기 변경 함수
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 윈도우 프로시저

	void drawAICharacter();
	void setRandomSkullPosition();

	// 비트맵을 읽어오는 함수
	HRESULT LoadBitmapFromResource(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

private:
	// 필수 변수들
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	IWICImagingFactory* m_pWICFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	// 비트맵
	ID2D1Bitmap* m_pCannonBitmap; // 포 비트맵
	ID2D1Bitmap* m_pBGBitmap; // 배경 비트맵
	ID2D1Bitmap* m_pGroundBitmap; // 상황판 겸 땅 비트맵
	ID2D1Bitmap* m_pExplosiveBitmap; // 폭발 이펙트 비트맵
	ID2D1Bitmap* m_pCannonBallBitmap; // 포탄 비트맵
	ID2D1Bitmap* m_pBitmapMask; // 비트맵 마스크
	ID2D1Bitmap* m_pMetoolBitmap; // 적을 그릴 비트맵
	ID2D1Bitmap* metoolBitmap;
	ID2D1Bitmap* m_GameOverBitmap; // 게임오버 비트맵
	ID2D1Bitmap* m_GameClearBitmap; // 게임 클리어 비트맵

	// 비트맵 브러쉬
	ID2D1BitmapBrush* m_pGroundBitmapBrush;
	ID2D1BitmapBrush* m_ExplosiveBrush;
	ID2D1BitmapBrush* m_pExplosiveBitmapBrush;
	ID2D1BitmapBrush* metoolBitmapBrush;
	
	// 브러쉬
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pRangeBrush;
	ID2D1SolidColorBrush* m_pRealBrush;
	ID2D1SolidColorBrush* m_pRedBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pSolidColorBrush;
	ID2D1SolidColorBrush* m_pSceneBrush;

	// Gradients
	ID2D1RadialGradientBrush* m_pRadialGradientBrush;

	// 경로 기하
	ID2D1PathGeometry* m_pExpectedGeometry; // 예상 경로 출력용
	ID2D1PathGeometry* m_pCannonGeometry; // 실제 경로 출력용
	ID2D1PathGeometry* m_pProjectileGeometry; // 투사체

	// 텍스트
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	// 인공지능
	std::vector<Character*> character;
	Character* myCharacter;

	//// 사운드
	//LPDIRECTSOUNDBUFFER g_lpDSBG[2] = { NULL };
	//CSoundManager* soundManager;

	// 애니메이션
	LinearAnimation<float> m_Animation;

	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;
};