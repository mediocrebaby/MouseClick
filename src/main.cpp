#include "mouse_click.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <string.h>

void printUsage()
{
    std::cout << "用法: mouse_click [选项]\n"
              << "选项:\n"
              << "  -t, --time <秒>     设置运行时长 (默认60秒，-1表示一直运行)\n"
              << "  -i, --interval <毫秒>  设置点击间隔 (默认1000毫秒)\n"
              << "  -d, --delay <秒>      设置开始延迟 (默认10秒)\n"
              << "  -h, --help            显示帮助信息\n";
}

bool parseArguments(int argc, char *argv[], int &durationSeconds, int &intervalMilliSeconds, int &delaySeconds)
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            printUsage();
            return false;
        }

        if (i + 1 >= argc)
        {
            std::cout << "错误: 参数 " << arg << " 需要一个值\n";
            return false;
        }

        try
        {
            if (arg == "-t" || arg == "--time")
            {
                durationSeconds = std::stoi(argv[++i]);
                if (durationSeconds < -1)
                {
                    std::cout << "错误: 运行时长不能小于-1\n";
                    return false;
                }
            }
            else if (arg == "-i" || arg == "--interval")
            {
                intervalMilliSeconds = std::stoi(argv[++i]);
                if (intervalMilliSeconds <= 0)
                {
                    std::cout << "错误: 间隔时间必须大于0\n";
                    return false;
                }
            }
            else if (arg == "-d" || arg == "--delay")
            {
                delaySeconds = std::stoi(argv[++i]);
                if (delaySeconds < 0)
                {
                    std::cout << "错误: 延迟时间不能小于0\n";
                    return false;
                }
            }
            else
            {
                std::cout << "错误: 未知参数 " << arg << "\n";
                printUsage();
                return false;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "错误: 参数值无效\n";
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    int durationSeconds = 60;        // 默认1分钟
    int intervalMilliSeconds = 1000; // 默认1000毫秒
    int delaySeconds = 10;

    if (!parseArguments(argc, argv, durationSeconds, intervalMilliSeconds, delaySeconds))
    {
        return 1;
    }

    MouseClick clicker;

    std::cout << "配置信息:\n"
              << "- 运行时长: " << (durationSeconds == -1 ? "无限" : std::to_string(durationSeconds) + "秒") << "\n"
              << "- 点击间隔: " << intervalMilliSeconds << "毫秒\n"
              << "- 开始延迟: " << delaySeconds << "秒\n\n";

    std::cout << "倒计时开始...\n";

    for (int i = delaySeconds; i > 0; i--)
    {
        std::cout << "\r程序将在 " << i << " 秒后开始运行..." << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    // 清除整行并显示新消息
    std::cout << "\r                                          \r程序开始运行!\n";

    clicker.start(durationSeconds, intervalMilliSeconds);

    return 0;
}