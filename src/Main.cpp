#include "Common.h"

struct GameState
{
    bool bRunning;
    int Width;
    int Height;

    // Graphics (DX11)
    ID3D11Device* Device;
    ID3D11DeviceContext* Context;
    D3D_FEATURE_LEVEL FeatureLevel;
    IDXGISwapChain1* SwapChain;
    ID3D11Texture2D* BackBuffer;
    ID3D11RenderTargetView* RenderTargetView;

    // Platform (Win32)
    HINSTANCE hInstance;
    HWND Window;
};
GameState GlobalState;

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
                GlobalState.bRunning = false;
            }
        }
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT:
        {
            GlobalState.bRunning = false;
        } break;
        default:
        {
            lResult = DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
    }

    return lResult;
}

void InitGraphics_DX11(GameState* _State)
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
        &_State->Device,
        &_State->FeatureLevel,
        &_State->Context
    );

    if (!_State->Device)
    {
        Outf("[error] Call to D3D11CreateDevice failed\n");
        return;
    }


    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
    SwapChainDesc1.Width = _State->Width;
    SwapChainDesc1.Height = _State->Height;
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
        _State->Device,
        _State->Window,
        &SwapChainDesc1,
        nullptr,
        nullptr,
        &_State->SwapChain
    );

    (void)_State->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_State->BackBuffer);

    (void)_State->Device->CreateRenderTargetView(_State->BackBuffer, nullptr, &_State->RenderTargetView);

    _State->Context->OMSetRenderTargets(1, &_State->RenderTargetView, nullptr);
}

void Draw_DX11(GameState* _State)
{
    _State->Context->OMSetRenderTargets(1, &_State->RenderTargetView, nullptr);

    constexpr float ClearColor[4] = { 242.0f / 255.0f, 80.0f / 255.0f, 34.0f / 255.0f, 1.0f };
    _State->Context->ClearRenderTargetView(_State->RenderTargetView, ClearColor);

    _State->SwapChain->Present(0, 0);
}

void InitPlatform_Win32(GameState* _State)
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
    WindowClass.hInstance = _State->hInstance;
    WindowClass.lpszClassName = WindowClassName;
    (void)RegisterClassExA(&WindowClass);

    _State->Window = CreateWindowExA(
        WindowExStyle,
        WindowClassName,
        WindowClassName,
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth, WindowHeight,
        nullptr, nullptr, nullptr, nullptr
    );

    ShowWindow(_State->Window, SW_SHOWNORMAL);
}

void TickPlatform_Win32(GameState* _State)
{
    if (_State && _State->Window)
    {
        MSG Msg = {};
        while (PeekMessageA(&Msg, _State->Window, 0, 0, TRUE))
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

    GlobalState = { };
    GlobalState.hInstance = _hInst;
    InitPlatform_Win32(&GlobalState);
    InitGraphics_DX11(&GlobalState);

    GlobalState.bRunning = true;
}

void GameLoop()
{
    while (GlobalState.bRunning)
    {
        TickPlatform_Win32(&GlobalState);
        Draw_DX11(&GlobalState);
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