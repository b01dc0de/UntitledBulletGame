#include "Common.h"

struct GameState
{
    bool bRunning;
    int Width;
    int Height;
};
GameState GlobalGameState;

struct GraphicsState
{
    ID3D11Device* Device;
    ID3D11DeviceContext* Context;
    D3D_FEATURE_LEVEL FeatureLevel;
    IDXGISwapChain1* SwapChain;
    ID3D11Texture2D* BackBuffer;
    ID3D11RenderTargetView* RenderTargetView;
};
GraphicsState GlobalGraphicsState;

struct Win32PlatformState
{
    HINSTANCE hInstance;
    HWND Window;
};

using PlatformState = Win32PlatformState;
PlatformState GlobalPlatformState;

LRESULT WndProc_Win32(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;

    switch (uMsg)
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            if (wParam == VK_ESCAPE)
            {
                GlobalGameState.bRunning = false;
            }
        }
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT:
        {
            GlobalGameState.bRunning = false;
        } break;
        default:
        {
            lResult = DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
    }

    return lResult;
}

void InitGraphics_DX11(GraphicsState* _GraphicsState)
{
    UINT CreateDeviceFlags = 0;
#if _DEBUG
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    (void)D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        CreateDeviceFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &_GraphicsState->Device,
        &_GraphicsState->FeatureLevel,
        &_GraphicsState->Context
    );

    if (!GlobalGraphicsState.Device)
    {
        Outf("[error] Call to D3D11CreateDevice failed\n");
        return;
    }


    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
    SwapChainDesc1.Width = GlobalGameState.Width;
    SwapChainDesc1.Height = GlobalGameState.Height;
    SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc1.Stereo = FALSE;
    SwapChainDesc1.SampleDesc = { 1, 0 };
    SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc1.BufferCount = 2;
    SwapChainDesc1.Scaling = DXGI_SCALING_NONE;
    SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    SwapChainDesc1.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH

    IDXGIFactory2* DxFactory2 = nullptr;
    (void)CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&DxFactory2));
    (void)DxFactory2->CreateSwapChainForHwnd(
        _GraphicsState->Device,
        GlobalPlatformState.Window,
        &SwapChainDesc1,
        nullptr,
        nullptr,
        &_GraphicsState->SwapChain
    );

    (void)_GraphicsState->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_GraphicsState->BackBuffer);

    (void)_GraphicsState->Device->CreateRenderTargetView(_GraphicsState->BackBuffer, nullptr, &_GraphicsState->RenderTargetView);

    _GraphicsState->Context->OMSetRenderTargets(1, &_GraphicsState->RenderTargetView, nullptr);
}

void Draw_DX11(GraphicsState* _GraphicsState)
{
    _GraphicsState->Context->OMSetRenderTargets(1, &_GraphicsState->RenderTargetView, nullptr);

    constexpr float ClearColor[4] = { 242.0f / 255.0f, 80.0f / 255.0f, 34.0f / 255.0f, 1.0f };
    _GraphicsState->Context->ClearRenderTargetView(_GraphicsState->RenderTargetView, ClearColor);

    _GraphicsState->SwapChain->Present(0, 0);
}

void InitPlatform_Win32(Win32PlatformState* _PlatformState)
{
    int WindowWidth = 1280;
    int WindowHeight = 720;
    RECT WorkArea = {};
    if (SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0))
    {
        WindowWidth = WorkArea.right - WorkArea.left;
        WindowHeight = WorkArea.bottom - WorkArea.top;
    }

    LPCSTR WindowClassName = "UntitledBulletGame";
    DWORD WindowStyle = WS_OVERLAPPEDWINDOW;
    DWORD WindowExStyle = 0;

    WNDCLASSEXA WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WndProc_Win32;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = _PlatformState->hInstance;
    WindowClass.lpszClassName = WindowClassName;
    (void)RegisterClassExA(&WindowClass);

    _PlatformState->Window = CreateWindowExA(
        WindowExStyle,
        WindowClassName,
        WindowClassName,
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth, WindowHeight,
        nullptr, nullptr, nullptr, nullptr
    );

    ShowWindow(_PlatformState->Window, SW_SHOWNORMAL);
}

void TickPlatform_Win32(Win32PlatformState* _PlatformState)
{
    if (_PlatformState && _PlatformState->Window)
    {
        MSG Msg = {};
        while (PeekMessageA(&Msg, _PlatformState->Window, 0, 0, TRUE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
    }
}

void Init(HINSTANCE _hInst)
{
    UNUSED_VAR(_hInst);
    OutputDebugStringA("UntitledBulletGame -- INIT\n");

    GlobalPlatformState = { _hInst };
    InitPlatform_Win32(&GlobalPlatformState);
    GlobalGraphicsState = { };
    InitGraphics_DX11(&GlobalGraphicsState);

    GlobalGameState.bRunning = true;
}

void GameLoop()
{
    while (GlobalGameState.bRunning)
    {
        TickPlatform_Win32(&GlobalPlatformState);
        Draw_DX11(&GlobalGraphicsState);
    }
}

void Term()
{
    OutputDebugStringA("UntitledBulletGame -- TERM\n");
}

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    UNUSED_VAR(hInstance);
    UNUSED_VAR(hPrevInstance);
    UNUSED_VAR(lpCmdLine);
    UNUSED_VAR(nShowCmd);

    Init(hInstance);
    GameLoop();
    Term();

    return 0;
}