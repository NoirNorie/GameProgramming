#pragma once

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
// DX Header Files:
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
// Animation
#include "Animation.h"


// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define PI 3.141

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	HRESULT CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush);

	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 비트맵 불러오기
	HRESULT LoadBitmapFromResource(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	IWICImagingFactory* m_pWICFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	//비트맵 생성
	ID2D1Bitmap* m_pBitmap;
	ID2D1Bitmap* m_pAnotherBitmap;
	ID2D1Bitmap* m_pGroundBitmap;
	ID2D1Bitmap* m_pExpectedLocBitmap;
	ID2D1Bitmap* m_pExplosiveBitmap;
	ID2D1Bitmap* m_pBitmapMask;

	ID2D1BitmapBrush* m_pGroundBitmapBrush;
	ID2D1BitmapBrush* m_ExplosiveBrush;
	ID2D1BitmapBrush* m_pExplosiveBitmapBrush;
	ID2D1BitmapBrush* m_pExpectedLocBitmapBrush;
	
	// gradients
	//ID2D1Bitmap* m_pRadialFadeExplosiveBitmap;

	ID2D1RadialGradientBrush* m_pRadialGradientBrush;


	// 경로 기하
	ID2D1PathGeometry* m_pExpectedGeometry;
	ID2D1PathGeometry* m_pProjectileGeometry; // 투사체
	ID2D1PathGeometry* m_pTestGeometry;
	ID2D1RectangleGeometry* m_pExpectedTGeometry; // 예상 지점

	// 브러쉬
	ID2D1SolidColorBrush* m_pGreenBrush;

	// 텍스트
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pDarkBrush;

	// 애니메이션
	//EaseInOutExponentialAnimation<float> m_Animation;
	LinearAnimation<float> m_Animation;


	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;
};
