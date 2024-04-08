#include <windows.h>
#include <wininet.h>
#include <stdio.h>
extern char *userId;

int Zhuan(int sernum)
{

    char *serviceName;
    if (sernum == 2)
    {
        serviceName = (char *)malloc(strlen("default") + 1); // 分配足够的内存
        if (serviceName == NULL)
        {
            perror("Memory allocation failed");
            return 1;
        }
        strcpy(serviceName, "default"); // 复制字符串到分配的内存中
    }
    else
    {
        serviceName = (char *)malloc(strlen("cmcc") + 1); // 分配足够的内存
        if (serviceName == NULL)
        {
            perror("Memory allocation failed");
            return 1;
        }
        strcpy(serviceName, "cmcc"); // 复制字符串到分配的内存中
    }

    HINTERNET hInternet, hConnect, hRequest;
    char *szHeaders = "Accept: */*\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n";
    char *szContentTypeHeader = "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
    char szPostData[200];
    char *szHostName = "lan.upc.edu.cn";
    char *szUrlPath = "/eportal/InterFace.do?method=switchService";
    if (userId == NULL)
    {
        char userId[] = "33303636323362613462363233303430303235";
        sprintf(szPostData, "userId=%s&serviceName=%s",
                userId, serviceName);
        // printf("合并后的字符串为: %s\n", szPostData);
    }
    else
    {
        sprintf(szPostData, "userId=%s&serviceName=%s",
                userId, serviceName);
        // printf("合并后的字符串为: %s\n", szPostData);
    }
    free(serviceName);
    // free(userId);

    // 初始化WinINet
    hInternet = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        // printf("InternetOpen failed: %d\n", GetLastError());
        return 1;
    }

    // 连接到服务器
    hConnect = InternetConnect(hInternet, szHostName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        // printf("InternetConnect failed: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 创建请求
    hRequest = HttpOpenRequest(hConnect, "POST", szUrlPath, NULL, NULL, NULL, 0, 0);
    if (!hRequest)
    {
        // printf("HttpOpenRequest failed: %d\n", GetLastError());
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 添加请求头
    if (!HttpAddRequestHeaders(hRequest, szHeaders, strlen(szHeaders), HTTP_ADDREQ_FLAG_ADD))
    {
        // printf("HttpAddRequestHeaders failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 添加Content-Type请求头
    if (!HttpAddRequestHeaders(hRequest, szContentTypeHeader, strlen(szContentTypeHeader), HTTP_ADDREQ_FLAG_ADD))
    {
        // printf("HttpAddRequestHeaders failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 发送请求
    if (!HttpSendRequest(hRequest, NULL, 0, szPostData, strlen(szPostData)))
    {
        // printf("HttpSendRequest failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 获取响应
    char szBuffer[1024];
    DWORD dwRead;
    while (InternetReadFile(hRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead > 0)
    {
        szBuffer[dwRead] = '\0';
        // printf("%s", szBuffer);
    }

    // 关闭句柄
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}
// char *getip();
// char *uid(int sernum, char *queryString);
// int main()
// {
//     char *queryString = getip();
//     char *userIndex = uid(1, queryString);
//     Zhuan(2, userIndex);
//     return 0;
// }
