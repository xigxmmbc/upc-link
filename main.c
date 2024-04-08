#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <shellapi.h>

int ZhuangTai();
int TuiDeng();
int link(int sernum);
int Zhuan(int sernum);
int getbaidu();
int mima();
extern char *userId;
extern char *XueHao;
extern char *password;

int a = 0;
HANDLE hThread;
bool Flag1 = true;
bool Flag2 = true;
int c = 0;

HANDLE event1;
HWND g_hStaticText;
NOTIFYICONDATA nid;
HMENU hMenu;
WINDOWPLACEMENT g_wpPrev = {sizeof(g_wpPrev)};

void XianShi()
{
    if (ZhuangTai() == 1)
    {
        SetWindowText(g_hStaticText, L"当前状态为中国移动");
    }
    else if (ZhuangTai() == 2)
    {
        SetWindowText(g_hStaticText, L"当前状态为校园网");
    }
    else
    {
        SetWindowText(g_hStaticText, L"当前已掉线");
    }
}

void XunHuanLianJie(int ser)
{
    while (Flag1)
    {
        if (getbaidu())
        {
            link(ser);
            XianShi();
            Sleep(2000);
        }
        else
        {
            Sleep(2000);
        }
    }
}

DWORD WINAPI ThreadFunc1()
{
    mima();
    link(1);
    XianShi();
    while (Flag2)
    {
        Flag1 = true;
        switch (a)
        {
        case 1:
            link(1);
            XianShi();
            XunHuanLianJie(1);
            break;
        case 2:
            link(2);
            XianShi();
            XunHuanLianJie(2);
            break;
        case 3:
            Zhuan(1);
            XianShi();
            XunHuanLianJie(1);
            break;
        case 4:
            Zhuan(2);
            XianShi();
            XunHuanLianJie(2);
            break;
        case 5:
            TuiDeng();
            XianShi();
            WaitForSingleObject(event1, INFINITE);
            break;
        default:
            XunHuanLianJie(1);
            break;
        }
    }
    return 0;
}

LRESULT CALLBACK WindowProcW(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // 初始化托盘图标
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hwnd;
        nid.uID = 1;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER + 1;
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        lstrcpy(nid.szTip, L"登校网");
        Shell_NotifyIcon(NIM_ADD, &nid);

        // 创建菜单
        hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, 1, L"中国移动");
        AppendMenu(hMenu, MF_STRING, 2, L"校园网");
        AppendMenu(hMenu, MF_STRING, 3, L"转为中国移动");
        AppendMenu(hMenu, MF_STRING, 4, L"转为校园网");
        AppendMenu(hMenu, MF_STRING, 5, L"退出登录");
        break;
    case WM_USER + 1:
        // 处理托盘消息
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
            // 左击恢复主窗口
            ShowWindow(hwnd, SW_NORMAL);
            break;
        case WM_RBUTTONDOWN:
            // 右键点击显示菜单
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
            break;
        }
        break;
    case WM_DESTROY:
        // 移除托盘图标
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            a = 1;
            Flag1 = false;
            SetEvent(event1);
            break;
        case 2:
            a = 2;
            Flag1 = false;
            SetEvent(event1);
            break;
        case 3:
            a = 3;
            Flag1 = false;
            SetEvent(event1);
            break;
        case 4:
            a = 4;
            Flag1 = false;
            SetEvent(event1);
            break;
        case 5:
            a = 5;
            Flag1 = false;
            break;
        case 6:
            XianShi();
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    case WM_SYSCOMMAND:
        // 最小化到托盘
        if (LOWORD(wParam) == SC_MINIMIZE)
        {
            ShowWindow(hwnd, SW_HIDE);
            break;
        }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProcW;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, L"MyWindowClass", L"登校网", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 60, 60, 450, 550, NULL, NULL, hInstance, NULL);
    HWND button1 = CreateWindowW(L"BUTTON", L"中国移动", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 50, 100, 50, hwnd, (HMENU)1, hInstance, NULL);
    HWND button2 = CreateWindowW(L"BUTTON", L"校园网", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 120, 100, 50, hwnd, (HMENU)2, hInstance, NULL);
    HWND button3 = CreateWindowW(L"BUTTON", L"转为中国移动", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 190, 100, 50, hwnd, (HMENU)3, hInstance, NULL);
    HWND button4 = CreateWindowW(L"BUTTON", L"转为校园网", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 260, 100, 50, hwnd, (HMENU)4, hInstance, NULL);
    HWND button5 = CreateWindowW(L"BUTTON", L"退出登录", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 400, 100, 50, hwnd, (HMENU)5, hInstance, NULL);
    HWND button6 = CreateWindowW(L"BUTTON", L"查询状态", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 330, 100, 50, hwnd, (HMENU)6, hInstance, NULL);
    g_hStaticText = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 200, 190, 200, 50, hwnd, NULL, NULL, NULL);

    event1 = CreateEventW(NULL, FALSE, FALSE, NULL);
    MSG msg;
    hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, NULL);

    ShowWindow(hwnd, nCmdShow);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Flag1 = false;
    Flag2 = false;
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    free(userId);
    free(XueHao);
    free(password);
    return msg.wParam;
}
