#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <time.h>
// HINTERNET hInternet;
int bd;
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    // InternetHandles *p1 = (InternetHandles *)lpParam;
    // 这里执行 InternetOpenUrlA 的操作
    HINTERNET hUrl = InternetOpenUrl(*(HINTERNET *)lpParam, "http://www.baidu.com", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL)
    {
        // printf("InternetOpenUrl failed: %d\n", GetLastError());
        InternetCloseHandle(lpParam);
        bd = 1;
        return 1;
    }
    // 读取并输出响应内容
    char buffer[1024];
    DWORD bytesRead;
    InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead);
    char *baidu = strstr(buffer, "baidu");
    if (baidu == NULL)
    {
        printf("百度连接失败\n");
        InternetCloseHandle(lpParam);
        bd = 1;
        return 1;
    }
    bd = 0;
    return 0;
}
int getbaidu()
{
    DWORD dwTimeout = 2000;
    // 初始化 WinINet 库
    HINTERNET hInternet = InternetOpen("Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        // printf("InternetOpen failed: %d\n", GetLastError());
        return 1;
    }

    // // 打开一个URL
    // HINTERNET hUrl = InternetOpenUrl(hInternet, "http://www.baidu.com", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    // if (hUrl == NULL)
    // {
    //     //printf("InternetOpenUrl failed: %d\n", GetLastError());
    //     InternetCloseHandle(hInternet);
    //     return 1;
    // }

    // 创建一个线程来执行操作
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, &hInternet, 0, &dwThreadId);
    if (hThread == NULL)
    {
        // printf("CreateThread failed!\n");
        return 1;
    }

    clock_t start, end;
    double cpu_time_used;
    // 等待线程结束或超时
    start = clock();
    if (WaitForSingleObject(hThread, 1000) == WAIT_TIMEOUT)
    {
        // printf("TIMEout\n");
        // 关闭线程句柄
        CloseHandle(hThread);
        // 关闭 URL 和 WinINet 库
        InternetCloseHandle(hInternet);
        return 1;
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    // printf("程序运行时间：%f 秒\n", cpu_time_used);
    if (bd == 1)
    {
        CloseHandle(hThread);
        // 关闭 URL 和 WinINet 库
        InternetCloseHandle(hInternet);
        return 1;
    }
    // 关闭线程句柄
    CloseHandle(hThread);
    // 关闭 URL 和 WinINet 库
    InternetCloseHandle(hInternet);

    return 0;
}

// int main()
// {
//     getbaidu();
//     return 0;
// }