#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h" // 미리 컴파일된 헤더
#include "HW2.h" // D2D 헤더

//진입점 함수
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
	WINAPI WinMain(): 진입점 함수

	DemoApp 클래스의 인스턴스 생성
	- 클래스 초기화 함수 Initialize()
	- 메시지 루프 함수 RunMessageLoop()

	스레드에서 COM 라이브러리 사용을 위한 함수
	- CoInitialize(): 스레드가 COM 라이브러리를 사용을 위한 로딩과 초기화 수행
	- CoUninitialize(): 사용 종료 후 스레드에서 COM 라이브러리 언로드, 자원 반납

*/