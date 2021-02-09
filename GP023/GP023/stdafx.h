#pragma once

//windows Header
#include <Windows.h>

//C Runtime Header
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

//DX Header
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL;}}
//객체의 안전한 반납을 위한 매크로
//p->Release 대신 SAFE_RELEASE를 사용한다

//template <typename Interface>
//inline void SafeRelease(Interfase8& pInterfaceToRelease)
//{
//    if (pInterfaceToRelease != NULL)
//    {
//        (*ppInterfaceToRelease != NULL)->Release();
//        (*ppInterfaceToRelease) = NULL;
//    }
//}
////매크로 대신 함수를 사용하는 것도 가능하다

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
//Win32 관련 함수의 사용에 있어 현재 실행 모듈의 HINSTANCE가 필요한 경우
//HINSTANCE = 현재 실행 모듈의 주소, WinMain 함수의 1번 인자 값으로 주어진다
// 