#include "Common.h"

GameState GlobalState;

void Clock_Init(ClockT* _Clock)
{
    LARGE_INTEGER Timestamp = {};
    QueryPerformanceFrequency(&Timestamp);
    _Clock->Freq = Timestamp.QuadPart;

    QueryPerformanceCounter(&Timestamp);
    _Clock->Epoch = Timestamp.QuadPart;

    _Clock->CurrTs = _Clock->Epoch;
    _Clock->CurrTime = 0.0;
    _Clock->LastFrameDuration = 0.0f;
}

void Clock_Tick(ClockT* _Clock)
{
    LARGE_INTEGER Timestamp = {};
    QueryPerformanceCounter(&Timestamp);

    u64 LastFrameTs = Timestamp.QuadPart - _Clock->CurrTs;
    _Clock->LastFrameDuration = (double)LastFrameTs / (double)_Clock->Freq;
    _Clock->CurrTs = Timestamp.QuadPart;
    _Clock->CurrTime = (double)(_Clock->CurrTs - _Clock->Epoch) / (double)_Clock->Freq;
}

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

    Clock_Init(&GlobalState.Clock);

    GlobalState.bRunning = true;
}

void GameLoop()
{
    //double LastTimePrint = 0.0f;
    while (GlobalState.bRunning)
    {
        Clock_Tick(&GlobalState.Clock);
        /*
        if ((GlobalState.Clock.CurrTime - LastTimePrint) > 1.0f)
        {
            LastTimePrint = GlobalState.Clock.CurrTime;
            Outf("[time] CurrTime: %.02f, LastFrameDuration (ms): %.04f\n",
                GlobalState.Clock.CurrTime,
                GlobalState.Clock.LastFrameDuration * 1000.0);
        }
        */
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
