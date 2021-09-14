#pragma once
#pragma warning(disable:4996)

#include "stdafx.h" // �̸� �����ϵ� ���
#include "Animation.h"
#include "Character.h"


class DemoApp
{
public:
	DemoApp(); // �⺻ ������
	~DemoApp(); // �⺻ �Ҹ���
	HRESULT Initialize(HINSTANCE hInstance); // �ʱ�ȭ �Լ�
	void RunMessageLoop(); // �޽��� ���� �Լ�

private:
	HRESULT CreateDeviceResources();// ��ġ ������ �ڿ� ���� �Լ�
	void DiscardDeviceResources(); // �ڿ� ���� �Լ�

	HRESULT CreateDeviceIndependentResources(); // ��ġ ������ �ڿ� ���� �Լ�

	HRESULT OnRender(); //ȭ�鿡 ����ϴ� �Լ�

	void OnResize(UINT width, UINT height); // ȭ��ũ�� ���� �Լ�
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ������ ���ν���

	void drawAICharacter();
	void setRandomSkullPosition();

	// ��Ʈ���� �о���� �Լ�
	HRESULT LoadBitmapFromResource(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

private:
	// �ʼ� ������
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	IWICImagingFactory* m_pWICFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	// ��Ʈ��
	ID2D1Bitmap* m_pCannonBitmap; // �� ��Ʈ��
	ID2D1Bitmap* m_pBGBitmap; // ��� ��Ʈ��
	ID2D1Bitmap* m_pGroundBitmap; // ��Ȳ�� �� �� ��Ʈ��
	ID2D1Bitmap* m_pExplosiveBitmap; // ���� ����Ʈ ��Ʈ��
	ID2D1Bitmap* m_pCannonBallBitmap; // ��ź ��Ʈ��
	ID2D1Bitmap* m_pBitmapMask; // ��Ʈ�� ����ũ
	ID2D1Bitmap* m_pMetoolBitmap; // ���� �׸� ��Ʈ��
	ID2D1Bitmap* metoolBitmap;
	ID2D1Bitmap* m_GameOverBitmap; // ���ӿ��� ��Ʈ��
	ID2D1Bitmap* m_GameClearBitmap; // ���� Ŭ���� ��Ʈ��

	// ��Ʈ�� �귯��
	ID2D1BitmapBrush* m_pGroundBitmapBrush;
	ID2D1BitmapBrush* m_ExplosiveBrush;
	ID2D1BitmapBrush* m_pExplosiveBitmapBrush;
	ID2D1BitmapBrush* metoolBitmapBrush;
	
	// �귯��
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pRangeBrush;
	ID2D1SolidColorBrush* m_pRealBrush;
	ID2D1SolidColorBrush* m_pRedBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pSolidColorBrush;
	ID2D1SolidColorBrush* m_pSceneBrush;

	// Gradients
	ID2D1RadialGradientBrush* m_pRadialGradientBrush;

	// ��� ����
	ID2D1PathGeometry* m_pExpectedGeometry; // ���� ��� ��¿�
	ID2D1PathGeometry* m_pCannonGeometry; // ���� ��� ��¿�
	ID2D1PathGeometry* m_pProjectileGeometry; // ����ü

	// �ؽ�Ʈ
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	// �ΰ�����
	std::vector<Character*> character;
	Character* myCharacter;

	//// ����
	//LPDIRECTSOUNDBUFFER g_lpDSBG[2] = { NULL };
	//CSoundManager* soundManager;

	// �ִϸ��̼�
	LinearAnimation<float> m_Animation;

	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;
};