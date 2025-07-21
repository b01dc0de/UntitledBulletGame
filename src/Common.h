#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using f32 = float;
using f64 = double;

#include "UntitledBulletGame_Win32.h"
#include "UntitledBulletGame_DX11.h"

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
extern GameState GlobalState;

void Outf(const char* Fmt, ...);

#define DEBUG_BREAKPOINT() DebugBreak()
#define UNUSED_VAR(Var) (void)Var
#define ASSERT(Expr) if (!(Expr)) { Outf("[assert] FAILED\n\tExp: %s\n\tFile: %s\tLine: %d\n\tFunc: %s", #Expr, __FILE__, __LINE__, __FUNCSIG__); DEBUG_BREAKPOINT(); }


#endif // COMMON_H

