#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h" // �̸� �����ϵ� ���
#include "HW2.h" // D2D ���

//������ �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
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
/*
	WINAPI WinMain(): ������ �Լ�

	DemoApp Ŭ������ �ν��Ͻ� ����
	- Ŭ���� �ʱ�ȭ �Լ� Initialize()
	- �޽��� ���� �Լ� RunMessageLoop()

	�����忡�� COM ���̺귯�� ����� ���� �Լ�
	- CoInitialize(): �����尡 COM ���̺귯���� ����� ���� �ε��� �ʱ�ȭ ����
	- CoUninitialize(): ��� ���� �� �����忡�� COM ���̺귯�� ��ε�, �ڿ� �ݳ�

*/