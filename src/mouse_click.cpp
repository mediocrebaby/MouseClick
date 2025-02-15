#include "mouse_click.h"
#include <thread>
#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#elif defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#include <termios.h>
#include <unistd.h>
#else // Linux
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <termios.h>
#include <unistd.h>
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
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliSeconds));
    }

    if (durationSeconds != -1)
    {
        std::cout << "运行时间到达，程序退出\n";
    }
}