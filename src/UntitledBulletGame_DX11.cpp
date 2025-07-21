#include "Common.h"

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
