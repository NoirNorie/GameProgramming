#pragma once
#include "stdafx.h"
#include "StudentID.h"

using namespace std;

class DemoApp
{
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1StrokeStyle* m_pStrokeStyleDash;

	ID2D1SolidColorBrush* m_pOriginalShapeBrush;
	ID2D1SolidColorBrush* m_boundaryBrush;
	ID2D1SolidColorBrush* m_RectangleBrush;
	ID2D1SolidColorBrush* m_pFillBrush;
	ID2D1SolidColorBrush* m_pTextBrush;

	//DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

public:
	//持失,社瑚切
	DemoApp();
	~DemoApp();

	HRESULT Initialize(HINSTANCE hInstance);
	void RunMessageLoop();

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	HRESULT OnRender();

	void DiscardDeviceResources();

	void OnResize(UINT width, UINT height);

	void drawInit();
	void drawTempInsertingRectangle();
	void drawTempdeletingRectangle();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};