/*
    main.c
    主程序
*/

// 引入头文件
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <windows.h>

// 宏定义
#define UNICODE // 使用Unicode字符集
#define _UNICODE // 使用Unicode字符集

#define TIMER_ID 0x1 // 计时器ID
#define TIMER_ELAPSE 50 // 计时器时间
#define PARAM_BASIC 100 // 基本放大参数
#define PARAM_OFFSET 25 // 放大参数偏移
#define PARAM_OFFSET_RATE 0.005 // 放大参数偏移速率
#define SAMPLE_RATE 0.005 // 图形采样速率

// 函数声明
/*
    窗口消息处理
    \param 窗口句柄
    \param 消息ID
    \param 消息附加信息
    \param 消息附加信息
    \return 消息处理结果
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/*
    图形绘制
    \param 窗口句柄
*/
void Draw(HWND hWnd);

// 函数定义

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR pCmdLine, int nCmdShow)
{
    // 定义窗口类
    WNDCLASSEX wndCls = {0};
    wndCls.cbSize = sizeof(WNDCLASSEX);
    wndCls.style = CS_HREDRAW | CS_VREDRAW;
    wndCls.lpfnWndProc = WndProc;
    // wndCls.cbClsExtra = 0;
    // wndCls.cbWndExtra = 0;
    wndCls.hInstance = hInstance;
    // wndCls.hIcon = NULL;
    wndCls.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    wndCls.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    // wndCls.lpszMenuName = NULL;
    wndCls.lpszClassName = TEXT("wndCls");
    // wndCls.hIconSm = NULL;

    // 注册窗口类
    if (!RegisterClassEx(&wndCls))
    {
        MessageBoxEx(NULL, TEXT("Failed to register window class!"), TEXT("ERROR"), MB_OK | MB_ICONERROR, 0);
        return 0;
    }

    // 创建窗口
    HWND hWnd = CreateWindowEx(WS_EX_LEFT, TEXT("wndCls"), TEXT("GDILove"), WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, hInstance, NULL);

    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 消息循环
    MSG msg; // 消息
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // 设置定时器
            SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
            break;
        case WM_PAINT:
            Draw(hWnd);
            break;
        case WM_TIMER:
            if (wParam == TIMER_ID)
            {
                // 设定重绘矩形
                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                InvalidateRect(hWnd, &clientRect, TRUE);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void Draw(HWND hWnd)
{
    // 获取客户端矩形中心
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int clientCenterX = clientRect.right / 2;
    int clientCenterY = clientRect.bottom / 2;

    // 准备绘制
    PAINTSTRUCT paintStruct;
    HDC hDc = BeginPaint(hWnd, &paintStruct);

    // 计算放大参数
    int param = PARAM_BASIC + PARAM_OFFSET * sin(PARAM_OFFSET_RATE * GetTickCount());
    // 绘制上半部分
    for (double i = 0.0; i <= 4.0; i += SAMPLE_RATE)
    {
        double x = -2.0 + i;
        double y = sqrt(2.0 * sqrt(pow(x, 2.0)) - pow(x, 2.0));
        SetPixel(hDc, clientCenterX + param * x, clientCenterY - param * y, RGB(255, 0, 0));
    }
    // 绘制下半部分
    for (double i = 0.0; i <= 4.0; i += SAMPLE_RATE)
    {
        double x = -2.0 + i;
        double y = -2.14 * sqrt(sqrt(2.0) - sqrt(fabs(x)));
        SetPixel(hDc, clientCenterX + param * x, clientCenterY - param * y, RGB(255, 0, 0));
    }

    // 结束绘制
    EndPaint(hWnd, &paintStruct);
}
