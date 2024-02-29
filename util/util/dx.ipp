#include "dx.hpp"

DirectXUtil::DeviceInfo DirectXUtil::info;

bool DirectXUtil::InitD3D(D3DDEVTYPE deviceType, IN size_t w, IN size_t h, bool bWindowed)
{
    info.width  = w;
    info.height = h;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Register Window
    WNDCLASS wndClass{ CS_HREDRAW | CS_VREDRAW,
                       WndProc,
                       NULL,
                       NULL,
                       hInstance,
                       LoadIcon(NULL, IDI_APPLICATION),
                       LoadCursor(NULL, IDC_ARROW),
                       (HBRUSH)GetStockObject(WHITE_BRUSH),
                       NULL,
                       TEXT(WINDOW_NAME) };
    RegisterClass(&wndClass);

    // create Window
    HWND hWnd = CreateWindow(TEXT(WINDOW_NAME),
                             TEXT(WINDOW_NAME),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             info.width,
                             info.height,
                             NULL,
                             NULL,
                             hInstance,
                             NULL);

    if(!hWnd) return false;

    // set Window
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    // create DirectX9
    LPDIRECT3D9 dx = Direct3DCreate9(D3D_SDK_VERSION);
    if(dx == nullptr) {
        MessageBox(nullptr, TEXT("Direct3DCreate9()"), TEXT("Error"), MB_OK);
        return false;
    }

    // create caps
    D3DCAPS9 caps;
    dx->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

    int vp = 0;
    if(caps.DeclTypes & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    // DirectX Parameter
    D3DPRESENT_PARAMETERS dxParam;
    dxParam.Flags = 0;

    // Back buffer
    dxParam.BackBufferWidth  = info.width;
    dxParam.BackBufferHeight = info.height;
    dxParam.BackBufferFormat = D3DFMT_A8R8G8B8;
    dxParam.BackBufferCount  = 1;
    dxParam.SwapEffect       = D3DSWAPEFFECT_DISCARD;

    // Multi Sampling
    dxParam.MultiSampleType    = D3DMULTISAMPLE_NONE;
    dxParam.MultiSampleQuality = 0;

    // Window
    dxParam.hDeviceWindow              = hWnd;
    dxParam.Windowed                   = bWindowed;
    dxParam.EnableAutoDepthStencil     = true;
    dxParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    dxParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

    dxParam.AutoDepthStencilFormat = D3DFMT_D24S8;
    if(FAILED(dx->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, vp, &dxParam, &info.device))) {
        dxParam.AutoDepthStencilFormat = D3DFMT_D16;
        if(FAILED(dx->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, vp, &dxParam, &info.device))) {
            MessageBox(nullptr, TEXT("CreateDevice()"), TEXT("Error"), MB_OK);
            SAFE_RELEASE(dx);
            return false;
        }
    }

    return true;
}

void DirectXUtil::Release()
{
    SAFE_RELEASE(info.device);
}

int DirectXUtil::EnterMessageLoop(bool (*fpDisplay)(const float))
{
    float deltaTime = 0;
    MSG   msg       = { 0 };
    DWORD last      = (float)timeGetTime();

    while(msg.message != WM_QUIT) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else {
            DWORD current   = (float)timeGetTime();
            float deltaTime = (float)(current - last) * 0.001f;
            last            = current;
            fpDisplay(deltaTime);
        }
    }

    return msg.wParam;

    return 0;
}

LONG_PTR CALLBACK DirectXUtil::WndProc(IN HWND hWnd, IN UINT msg, IN WPARAM wParam, IN LPARAM lParam)
{
    static HMENU hMenu, hSubMenu;
    const int    ID_FILE_OPEN = 12345;

    switch(msg) {
    case WM_CREATE:
        hMenu = CreateMenu();

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, TEXT("Open"));
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, TEXT("File"));

        SetMenu(hWnd, hMenu);
        break;

    case WM_MOUSEWHEEL:
        if((SHORT)HIWORD(wParam) > 0) {
            InputUtil::SetWheelUp();
        }
        else {
            InputUtil::SetWheelDown();
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case ID_FILE_OPEN:
            if(!FileUtil::IsFileOpen()) {
                FileUtil::SetFileOpen();
            }
        }
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        else {
            break;
        }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

D3DXVECTOR2 DirectXUtil::ToUV(IN const D3DXVECTOR2& vec)
{
    float u = vec.x * 0.5f + 0.5f;
    float v = vec.y * -0.5f + 0.5f;
    return D3DXVECTOR2{ u, v };
}

D3DXVECTOR2 DirectXUtil::ToXY(IN const D3DXVECTOR2& vec)
{
    float x = (vec.x - 0.5f) * 2.f;
    float y = (0.5 - vec.y) * 2.f;
    return D3DXVECTOR2{ x, y };
}

const LPDIRECT3DDEVICE9& DirectXUtil::Device()
{
    return info.device;
}

void DirectXUtil::Setup()
{
    D3DXVECTOR3 pos    = { 0, 0.5, -5 };
    D3DXVECTOR3 target = { 0, 0.5, 0 };
    D3DXVECTOR3 up     = { 0, 1, 0 };

    D3DXMATRIXA16 mat;
    D3DXMatrixLookAtLH(&mat, &pos, &target, &up);
    info.device->SetTransform(D3DTS_VIEW, &mat);

    D3DXMatrixPerspectiveFovLH(&mat, D3DX_PI * 0.5f, info.width / info.height, 1, 1000);
    info.device->SetTransform(D3DTS_PROJECTION, &mat);

    info.device->SetRenderState(D3DRS_ZENABLE, TRUE);
    info.device->SetRenderState(D3DRS_AMBIENT, -1);
}