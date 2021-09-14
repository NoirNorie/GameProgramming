#pragma once

// 윈도우 헤더 파일
#include <windows.h>

// C 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <time.h>

// C++ 헤더 파일
#include <string>
#include <vector>

// Direct X 헤더 파일
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>

// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p)=NULL; } }
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_FREE(p) { if (p) { free(p); (p)=NULL; } }

// 에러 제어
//#pragma comment( lib, "Dxerr.lib" )


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif
#endif