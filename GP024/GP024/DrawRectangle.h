#pragma once
#include "stdafx.h"

class DemoApp
{
	//변수부
	HWND m_hwnd; // 생성된 윈도우 핸들을 가진다
	ID2D1Factory* m_pDirect2dFactory;
	// 생성된 D2D 팩토리를 소유
	// 팩토리 : D2D 사용을 위한 출발점이 되는 인터페이스
	ID2D1HwndRenderTarget* m_pRenderTarget;
	// 생성된 렌더 타겟을 소유
	// 렌더 타겟 : 그리기를 위해 필요한 자원을 생성하는 일을 수행
	// 실제 그리는 연산도 수행한다

	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	// 붓 객체 : 그리기에 있어 색칠하는 방법을 명시하는 객체


	//함수부
	HRESULT CreateDeviceIndependentResources();
	//장치에서 독립적인 자원을 생성한다 

	HRESULT CreateDeviceResources();
	//장치에 의존적인 자원을 생성한다
	void DiscardDeviceResources();
	//장치에 의존적인 자원을 반납한다.

	HRESULT OnRender();
	//화면에 표시할 내용들을 그리는 함수

	void OnResize(UINT width, UINT height);
	//렌더 타켓의 크기를 변경하는 함수

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//윈도우 프로시저 함수로 정적으로 선언됨
	//생성 시점에서 WNDCLASS 구조체의 ipfnWndProc 필드에 지정되어야 한다

public:
	// 생성, 소멸자
	DemoApp();
	~DemoApp();


	HRESULT Initialize(HINSTANCE hInstance);
	//윈도우 클래스를 등록하고 그리기 자원들을 생성하는 함수 호출

	void RunMessageLoop();
	// 메시지 처리를 위한 메인 메시지 루프 함수

};