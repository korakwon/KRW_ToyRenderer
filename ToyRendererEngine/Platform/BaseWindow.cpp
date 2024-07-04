#include "BaseWindow.h"
#include "Application/Application.h"

using Platform::BaseWindow;

HINSTANCE Platform::G_HINSTANCE = nullptr;
HWND Platform::G_HWND = nullptr;

BaseWindow::BaseWindow(std::wstring PlatformTitle, const UINT32& PlatformWidth, const UINT32& PlatformHeight)
    : Title(PlatformTitle)
      , Width(PlatformWidth)
      , Height(PlatformHeight)
{
}
BaseWindow::~BaseWindow()
{
    Destroy();
}

void BaseWindow::Show() const
{
    ShowWindow(G_HWND, SW_SHOW);
    SetForegroundWindow(G_HWND);
    SetFocus(G_HWND);
    ShowCursor(true);
}
void BaseWindow::Hide() const
{
    ShowWindow(G_HWND, SW_HIDE);
}

void BaseWindow::Initialize()
{
    ASSERT_NOT_NULL(Platform::G_HINSTANCE);
    
    WNDCLASSEX WindowClass = {};
    WindowClass.cbSize        = sizeof(WNDCLASSEX);
    WindowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    WindowClass.lpfnWndProc   = App::WndProc;
    WindowClass.hInstance     = Platform::G_HINSTANCE;
    WindowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    WindowClass.lpszClassName = Title.c_str();
    RegisterClassEx(&WindowClass);

    G_HWND = CreateWindow(
        Title.c_str(),
        Title.c_str(),
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        Platform::G_HINSTANCE,
        nullptr
    );
    ASSERT_NOT_NULL(G_HWND);

    RECT WindowRect = {0, 0, static_cast<LONG>(Width), static_cast<LONG>(Height)};
    UINT32 CenterX = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
    UINT32 CenterY = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
    
    AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);
    MoveWindow(
        G_HWND
        , static_cast<int>(CenterX), static_cast<int>(CenterY)
        , WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top
        , TRUE
    );

    Show();
}
void BaseWindow::Update(const float& DeltaTime)
{
    MSG Message = {};
    if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
}
void BaseWindow::Destroy()
{
    if (G_HWND != nullptr)
    {
        DestroyWindow(G_HWND);
        UnregisterClass(Title.c_str(), Platform::G_HINSTANCE);
        
        G_HWND = nullptr;
    }
    
    Platform::G_HINSTANCE = nullptr;
}

void BaseWindow::OnResize(const UINT32& Width, const UINT32& Height)
{
    this->Width = Width;
    this->Height = Height;
}
