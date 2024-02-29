#ifndef LWE__DX_HPP__
#define LWE__DX_HPP__

#define _CRT_SECURE_NO_WARNINGS

#include "../../common/common/common.hpp"
#include "file.hpp"
#include "input.hpp"

#pragma comment(lib, "WinMM")

#ifdef _WIN64
#    pragma comment(lib, "d3d/d3d/lib/x64/d3d9")
#    pragma comment(lib, "d3d/d3d/lib/x64/d3dx9")
#else
#    pragma comment(lib, "d3d/d3d/lib/x86/d3d9")
#    pragma comment(lib, "d3d/d3d/lib/x86/d3dx9")
#endif

class DirectXUtil
{
    DirectXUtil() = delete;

public:
    static bool InitD3D(IN D3DDEVTYPE, IN size_t w, IN size_t h, IN bool);
    static void Setup();
    static void Release();
    static int  EnterMessageLoop(bool (*)(const float));

    static LONG_PTR CALLBACK WndProc(IN HWND, IN UINT, IN WPARAM, IN LPARAM);

    static D3DXVECTOR2 ToUV(IN const D3DXVECTOR2&);
    static D3DXVECTOR2 ToXY(IN const D3DXVECTOR2&);

public:
    static const LPDIRECT3DDEVICE9& Device();
    static const float              ScreenWitdh();
    static const float              ScreenHeight();

private:
    static struct DeviceInfo
    {
        LPDIRECT3DDEVICE9 device = NULL;
        float             width  = 1280;
        float             height = 720;
    } info;
};

#include "dx.ipp"
#endif;