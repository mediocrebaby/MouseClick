#include "mouse_click.h"
#include <thread>
#include <chrono>
#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <conio.h>

// 添加Windows全局热键检测函数
bool checkEscapeKey()
{
    return (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
}

#elif defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#include <termios.h>
#include <unistd.h>

// 添加用于MacOS的全局热键检测
bool checkEscapeKey()
{
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    if (!source)
        return false;

    bool escapePressed = CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, 53);

    CFRelease(source);
    return escapePressed;
}

#else // Linux
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <termios.h>
#include <unistd.h>

// 添加用于Linux的全局热键检测
bool checkEscapeKey()
{
    Display *display = XOpenDisplay(NULL);
    if (!display)
        return false;

    char keys[32];
    XQueryKeymap(display, keys);
    KeyCode escapeKeycode = XKeysymToKeycode(display, XK_Escape);
    bool pressed = !!(keys[escapeKeycode >> 3] & (1 << (escapeKeycode & 7)));

    XCloseDisplay(display);
    return pressed;
}
#endif

#if defined(__APPLE__) || defined(__linux__)
bool kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}
#endif

void MouseClick::click()
{
#ifdef _WIN32
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
#elif defined(__APPLE__)
    CGEventRef event = CGEventCreate(NULL);
    CGPoint currentPos = CGEventGetLocation(event);
    CFRelease(event);

    CGEventRef clickDown = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseDown,
        currentPos,
        kCGMouseButtonLeft);
    CGEventRef clickUp = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseUp,
        currentPos,
        kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, clickDown);
    CGEventPost(kCGHIDEventTap, clickUp);

    CFRelease(clickDown);
    CFRelease(clickUp);
#else // Linux
    Display *display = XOpenDisplay(NULL);
    XTestFakeButtonEvent(display, 1, True, 0);
    XTestFakeButtonEvent(display, 1, False, 0);
    XCloseDisplay(display);
#endif
}

void MouseClick::start(int durationSeconds, int intervalMilliSeconds)
{
    isRunning = true;
    auto startTime = std::chrono::steady_clock::now();

    std::cout << "程序已启动，按ESC键退出...\n";

    while (isRunning)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(
                                  currentTime - startTime)
                                  .count();

        if (durationSeconds != -1 && elapsedSeconds >= durationSeconds)
        {
            break;
        }

        click();

        // 将等待时间分成多个100ms的小段
        int remainingWait = intervalMilliSeconds;
        while (remainingWait > 0 && isRunning)
        {
            // 每次等待100ms或剩余的时间（如果小于100ms）
            int waitTime = std::min(100,remainingWait);
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
            remainingWait -= waitTime;

            // 检查ESC键是否被按下
            if (checkEscapeKey())
            {
                std::cout << "检测到ESC键，程序退出\n";
                isRunning = false;
                break;
            }
        }
    }

    if (durationSeconds != -1 && isRunning)
    {
        std::cout << "运行时间到达，程序退出\n";
    }
}