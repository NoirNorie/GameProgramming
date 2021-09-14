#include "stdafx.h"
#include "HW4.h"


// ��ġ ������ �ڿ� ���� �Լ�
HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// D2D ����Ÿ���� ������.
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		// �ܺ� ���Ϸκ��� ��Ʈ�� ��ü�� ����
		if (SUCCEEDED(hr)) // ���
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\Background.png", 1280, 500,
				&m_pBGBitmap);
		}
		if (SUCCEEDED(hr)) // ����
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\MOT.png", 100, 0,
				&m_pCannonBitmap);
		}
		if (SUCCEEDED(hr)) // �ٴ�
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\asdf.png", 1265, 200,
				&m_pGroundBitmap);
		}
		// ���� �̹���
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\explosive.png", 0, 0,
				&m_pExplosiveBitmap);
		}
		// ��ź �̹���
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\Cannonball.png", 25, 25,
				&m_pCannonBallBitmap);
		}
		// �� �̹���
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\mob.png", 30, 30,
				&m_pMetoolBitmap);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\mob.png", 20, 20,
				&metoolBitmap
			);
		}
		// ���� ���� ����
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\Gameover.png", 0, 0,
				&m_GameOverBitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(
				m_pRenderTarget, m_pWICFactory,
				L".\\Clear.png", 0, 0,
				&m_GameClearBitmap
			);
		}

		// �귯��
		if (SUCCEEDED(hr)) // ���� ���� ǥ��
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LawnGreen), &m_pRangeBrush);
		}
		if (SUCCEEDED(hr)) // ���� ���� ǥ��
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &m_pRealBrush);
		}
		if (SUCCEEDED(hr)) // ������ ��
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
		}
		if (SUCCEEDED(hr)) // �ʷϻ� ��
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Green), &m_pGreenBrush);
		}
		if (SUCCEEDED(hr)) // ������ ��
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::IndianRed), &m_pRedBrush);
		}
		if (SUCCEEDED(hr))
		{
			// Create a scene brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black, 1.f), &m_pSceneBrush);
		}
		// ������ ����ũ
		if (SUCCEEDED(hr))
		{
			D2D1_BITMAP_BRUSH_PROPERTIES propertiesXClampYClamp = D2D1::BitmapBrushProperties(
				D2D1_EXTEND_MODE_CLAMP, D2D1_EXTEND_MODE_CLAMP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(m_pGroundBitmap, propertiesXClampYClamp, &m_pGroundBitmapBrush);
			}
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					m_pExplosiveBitmap,
					propertiesXClampYClamp,
					&m_pExplosiveBitmapBrush
				);
			}


			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					metoolBitmap, propertiesXClampYClamp,
					&metoolBitmapBrush);
			}
		}
		// Gradient
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
	}
	return hr;
}