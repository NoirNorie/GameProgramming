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
//��ü�� ������ �ݳ��� ���� ��ũ��
//p->Release ��� SAFE_RELEASE�� ����Ѵ�

//template <typename Interface>
//inline void SafeRelease(Interfase8& pInterfaceToRelease)
//{
//    if (pInterfaceToRelease != NULL)
//    {
//        (*ppInterfaceToRelease != NULL)->Release();
//        (*ppInterfaceToRelease) = NULL;
//    }
//}
////��ũ�� ��� �Լ��� ����ϴ� �͵� �����ϴ�

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
//Win32 ���� �Լ��� ��뿡 �־� ���� ���� ����� HINSTANCE�� �ʿ��� ���
//HINSTANCE = ���� ���� ����� �ּ�, WinMain �Լ��� 1�� ���� ������ �־�����
// 