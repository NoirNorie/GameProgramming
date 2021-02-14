#pragma once
#include "stdafx.h"

class DemoApp
{
	//������
	HWND m_hwnd; // ������ ������ �ڵ��� ������
	ID2D1Factory* m_pDirect2dFactory;
	// ������ D2D ���丮�� ����
	// ���丮 : D2D ����� ���� ������� �Ǵ� �������̽�
	ID2D1HwndRenderTarget* m_pRenderTarget;
	// ������ ���� Ÿ���� ����
	// ���� Ÿ�� : �׸��⸦ ���� �ʿ��� �ڿ��� �����ϴ� ���� ����
	// ���� �׸��� ���굵 �����Ѵ�

	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	// �� ��ü : �׸��⿡ �־� ��ĥ�ϴ� ����� ����ϴ� ��ü


	//�Լ���
	HRESULT CreateDeviceIndependentResources();
	//��ġ���� �������� �ڿ��� �����Ѵ� 

	HRESULT CreateDeviceResources();
	//��ġ�� �������� �ڿ��� �����Ѵ�
	void DiscardDeviceResources();
	//��ġ�� �������� �ڿ��� �ݳ��Ѵ�.

	HRESULT OnRender();
	//ȭ�鿡 ǥ���� ������� �׸��� �Լ�

	void OnResize(UINT width, UINT height);
	//���� Ÿ���� ũ�⸦ �����ϴ� �Լ�

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//������ ���ν��� �Լ��� �������� �����
	//���� �������� WNDCLASS ����ü�� ipfnWndProc �ʵ忡 �����Ǿ�� �Ѵ�

public:
	// ����, �Ҹ���
	DemoApp();
	~DemoApp();


	HRESULT Initialize(HINSTANCE hInstance);
	//������ Ŭ������ ����ϰ� �׸��� �ڿ����� �����ϴ� �Լ� ȣ��

	void RunMessageLoop();
	// �޽��� ó���� ���� ���� �޽��� ���� �Լ�

};