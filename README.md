## MouseClick

基于C++开发的跨平台鼠标点击模拟工具，支持Windows/Linux/MacOS。适用于需要重复点击的场景。

### 主要特性
- 🖱️ 支持鼠标左键点击模拟
- ⏱️ 灵活的时间控制参数
- 🖥️ 跨平台支持（Windows/Linux/MacOS）
- ⚡ 支持ESC键立即退出
- ⚙️ 命令行参数配置
- 📊 实时运行状态显示

### 项目结构

- `src` 源码目录
- `CMakeLists.txt` CMake配置文件
- `README.md` 项目说明
- `.gitignore` git忽略文件

### 功能支持

- 支持鼠标左键点击
- 支持用户设置点击次数或点击时长（默认一直点击）
- 支持用户设置每次点击的间隔时间（默认1s）
- 支持用户设置等待点击开始的延迟时间（默认10s）
- 支持运行时按下ESC键立即退出程序
- 程序通过命令行配置参数并启动

### 安装与使用
#### 从源码构建
```bash
# 1. 克隆仓库
git clone https://github.com/mediocrebaby/MouseClick.git
cd MouseClick

# 2. 创建构建目录
mkdir build && cd build

# 3. 生成构建系统
cmake ..

# 4. 编译项目
cmake --build .
```

#### 快速开始
```bash
# 默认参数运行（60秒，间隔1秒，延迟10秒）
./mouse_click

# 持续运行2小时（7200秒），间隔500毫秒
./mouse_click -t 7200 -i 500

# 无限运行模式，延迟5秒启动
./mouse_click -t -1 -d 5

# 注意：程序运行期间可随时按下ESC键退出
```

### 参数说明
| 选项 | 全称 | 参数 | 默认值 | 说明 |
|------|------|------|--------|------|
| `-t` | `--time` | 秒 | 60 | 运行时长（-1表示无限） |
| `-i` | `--interval` | 毫秒 | 1000 | 点击间隔时间 |
| `-d` | `--delay` | 秒 | 10 | 启动延迟时间 |
| `-h` | `--help` | - | - | 显示帮助信息 |

### 平台依赖
- **Windows**: 无需额外依赖
- **Linux**: 需要X11开发库
  ```bash
  sudo apt-get install libx11-dev libxtst-dev
  ```
- **MacOS**: 需要CoreGraphics框架

### 实现原理
通过各平台原生API实现鼠标控制：
- Windows: `SendInput` API
- Linux: X11库的`XTestFakeButtonEvent`
- MacOS: CoreGraphics的`CGEventCreateMouseEvent`
