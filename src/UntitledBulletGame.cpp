#include "Common.h"

GameState GlobalState;

void Outf(const char* Fmt, ...)
{
    constexpr size_t BufferLength = 1024;
    char MsgBuffer[BufferLength] = {};

    va_list Args;
    va_start(Args, Fmt);
    vsprintf_s(MsgBuffer, BufferLength, Fmt, Args);
    va_end(Args);

    OutputDebugStringA(MsgBuffer);
}

void Init(HINSTANCE _hInst)
{
    UNUSED_VAR(_hInst);
    Outf("UntitledBulletGame -- INIT\n");

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
    Outf("UntitledBulletGame -- TERM\n");
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
