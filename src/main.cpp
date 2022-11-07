#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "world.h"
#include "camera.h"
#include "common.h"
#include "debug.h"
#include "input.h"
#include "physics.h"

#include <chrono>
#include <thread>

static const char* s_windowClassName = "PhysWindowClass";

static bool s_running = true;
static HDC s_renderDC;
static HWND s_window;
static int s_windowHeight;
static int s_windowWidth;
constexpr int INITIAL_WINDOW_HEIGHT = 640;
constexpr int INITIAL_WINDOW_WIDTH = 640;
constexpr int INITIAL_WINDOW_POS_X = 100;
constexpr int INITIAL_WINDOW_POS_Y = 100;
static Input s_input = { 0 };
static short s_lastMoveX = 0;
static short s_lastMoveY = 0;

//*****************************************************************************
// Public Interface
//*****************************************************************************
static void RenderThread()
{
    HGLRC glc = wglCreateContext(s_renderDC);
    assert(glc != NULL);
    bool success = wglMakeCurrent(s_renderDC, glc);
    assert(success == TRUE);

    while (s_running)
    {
        float ratio = (float)s_windowWidth * (1.0f / (float)s_windowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, s_windowWidth, s_windowHeight);
        gluPerspective(45.0f, ratio, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        Camera_Draw();
        World_Draw();
        Debug_Draw();

        SwapBuffers(s_renderDC);
    }
}
//*****************************************************************************
static void UpdateThread()
{
    auto frameEnd = std::chrono::high_resolution_clock::now();
    //auto fpsLimit = std::chrono::duration<double, std::milli>(1.l/60.l);
    constexpr float MAX_FPS_MS = (1000.f / 60.f);
    while (s_running)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta_ms = (frameStart - frameEnd);
        float dt = min(MAX_FPS_MS, (float)(delta_ms.count())) / 1000.f;

        // Consume Input
        Input input = s_input;
        s_input = { s_input.mouseInput, 0, 0, 0 }; // todo this isnt threadsafe
        Input_Update(input);
        
        // update state of world
        Physics_Update(dt);
        World_Update(dt);
        Camera_Update(dt);

        // sleep if necessary
        frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameTime = frameStart - frameEnd;
        if (frameTime.count() < MAX_FPS_MS)
        {
            std::chrono::duration<double, std::milli> delta_ms(MAX_FPS_MS - frameTime.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
    }
}
//*****************************************************************************
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            if (msg == WM_LBUTTONDOWN)
            {
                s_input.mouseInput |= LEFT_BUTTON;
            }
            else if (msg == WM_MBUTTONDOWN)
            {
                s_input.mouseInput |= MIDDLE_BUTTON;
            }
            else
            {
                s_input.mouseInput |= RIGHT_BUTTON;
            }
            s_lastMoveX = SHORT(LOWORD(lParam));
            s_lastMoveY = SHORT(HIWORD(lParam));
            SetCapture(hWnd);
        }
        break;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            if (msg == WM_LBUTTONUP)
            {
                s_input.mouseInput &= ~LEFT_BUTTON;
            }
            else if (msg == WM_MBUTTONUP)
            {
                s_input.mouseInput &= ~MIDDLE_BUTTON;
            }
            else
            {
                s_input.mouseInput &= ~RIGHT_BUTTON;
            }
            if (!s_input.mouseInput)
            {
                ReleaseCapture();
            }
        }
        break;

        case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            s_input.x += x - s_lastMoveX;
            s_input.y += y - s_lastMoveY;
            s_lastMoveX = x;
            s_lastMoveY = y;
        }
        break;

        case WM_CHAR:
        {
            // The numbers 32–126 correspond to characters and symbols
            // this is the ' ' char to the '~' char
            if (wParam >= 32 && wParam <= 126)
            {
                s_input.keyInput |= wParam;
            }
            break;
        }

        case WM_SIZE:
        {
            s_windowWidth = LOWORD(lParam);
            s_windowHeight = HIWORD(lParam);
        }
        break;

        case WM_COMMAND:
            // TODO: from accelerators
            break;

        case WM_CLOSE:
            s_running = false;
            PostQuitMessage(0);
            break;

        default:
            break;
    }
    return (DefWindowProc(hWnd, msg, wParam, lParam));
}
//*****************************************************************************
int main( int argc, char* argv[] )
{
    World_Init();
    Camera_Init();
    Input_Init();


    const char* className = "PhysicsClassName";

    RECT winrect;
    winrect.left = INITIAL_WINDOW_POS_X;
    winrect.top = INITIAL_WINDOW_POS_Y;
    winrect.right = winrect.left + INITIAL_WINDOW_WIDTH;
    winrect.bottom = winrect.top + INITIAL_WINDOW_HEIGHT;
    DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    AdjustWindowRect(&winrect, style, 1);

    HMODULE instance = GetModuleHandleA(NULL);

    WNDCLASS wc;
    wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL; //MAKEINTRESOURCE(IDR_MENU1);
    wc.lpszClassName = className;
    int handle = RegisterClass(&wc);
    assert(handle);

    HWND window = CreateWindowExA(
        0
        , className
        , "Window Name"
        , style
        , winrect.left// x
        , winrect.top // y
        , winrect.right - winrect.left // width
        , winrect.bottom - winrect.top // height
        , NULL // parent
        , NULL // menu
        , instance // instance
        , NULL
    );
    assert(window != NULL);

    ShowWindow(window, SW_SHOWNORMAL);
    s_renderDC = GetDC(window);
    assert(s_renderDC != NULL);

    PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
          1,				     // version number
          PFD_DRAW_TO_WINDOW |	     // support window
          PFD_SUPPORT_OPENGL |	     // support OpenGL
          PFD_DOUBLEBUFFER,		     // double buffered
          PFD_TYPE_RGBA,		     // RGBA type
          24, 			     // 24-bit color depth
          0, 0, 0, 0, 0, 0,		     // color bits ignored
          0,				     // no alpha buffer
          0,				     // shift bit ignored
          0,				     // no accumulation buffer
          0, 0, 0, 0, 		     // accum bits ignored
          32, 			     // 32-bit z-buffer
          0,				     // no stencil buffer
          0,				     // no auxiliary buffer
          PFD_MAIN_PLANE,		     // main layer
          0,				     // reserved
          0, 0, 0			     // layer masks ignored
    };
    // get the best available match of pixel format for the device context
    int iPixelFormat = ChoosePixelFormat(s_renderDC, &pfd);
    assert(iPixelFormat != 0);
    bool success = SetPixelFormat(s_renderDC, iPixelFormat, &pfd);
    assert(success);

    std::thread renderThread = std::thread(RenderThread);
    std::thread updateThread = std::thread(UpdateThread);
    
    MSG msg;
    while (GetMessage(&msg, window, 0, 0))
    {
        if (!TranslateAccelerator(window, NULL, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    s_running = false;
    updateThread.join();
    renderThread.join();
    DestroyWindow(window);

    Input_Deinit();
    Camera_Deinit();
    World_Deinit();

    return 0;
}
//*****************************************************************************
