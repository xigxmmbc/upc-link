#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER_SIZE 4096
#pragma comment(lib, "wininet.lib")

// 判断字符是否是合法的URL字符
int is_url_safe(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '-' || c == '_' || c == '.' || c == '~';
}

// 对字符串进行URI编码
char *uri_encode(const char *str)
{
    const char *src = str;
    char *encoded = (char *)malloc(strlen(str) * 3 + 1); // 分配足够大的内存

    char *dest = encoded;
    while (*src)
    {
        if (is_url_safe(*src))
        {
            *dest++ = *src;
        }
        else
        {
            sprintf(dest, "%%%02X", (unsigned char)*src);
            dest += 3;
        }
        src++;
    }
    *dest = '\0'; // 添加字符串结尾标志
    return encoded;
}

char *getip()
{
    char szBuffer[MAX_BUFFER_SIZE];
    DWORD dwSize;
    BOOL bRet;
    HINTERNET hInternet = InternetOpen(" Mozilla / 5.0(compatible) ", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        // printf("InternetOpen failed\n");
        return NULL;
    }

    HINTERNET hConnect = InternetConnect(hInternet, "121.251.249.55", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        // printf("InternetConnect failed\n");
        InternetCloseHandle(hInternet);
        return NULL;
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "GET", "/", NULL, NULL, NULL, INTERNET_FLAG_NO_AUTO_REDIRECT, 0);
    if (hRequest == NULL)
    {
        // printf("HttpOpenRequest failed\n");
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    // 添加请求头
    const char *headers = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7";
    if (!HttpAddRequestHeaders(hRequest, headers, strlen(headers), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD))
    {
        // printf("HttpAddRequestHeaders failed\n");
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    // 发送请求
    if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
    {
        // printf("HttpSendRequest failed\n");
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    // 读取响应头
    dwSize = sizeof(szBuffer);
    bRet = HttpQueryInfo(hRequest, HTTP_QUERY_LOCATION, szBuffer, &dwSize, NULL);
    if (!bRet)
    {
        // printf("Failed to query LOCATION.\n");
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }
    // printf("Response Headers:\n%s\n", szBuffer);

    char *question_mark_position = strstr(szBuffer, "?");
    if (question_mark_position != NULL)
    {
        // 计算问号之后的字符串的长度
        size_t len = strlen(question_mark_position + 1);

        // 分配足够的空间来存储问号之后的字符串
        char result[len + 1];

        // 使用 sprintf 将问号之后的字符串存储到 result 中
        sprintf(result, "%s", question_mark_position + 1);

        // printf("问号之后的字符串为:\n%s\n", result);

        char *encoded_str = uri_encode(result);
        char *encoded_str2 = uri_encode(encoded_str);

        // printf("Doubly URI encoded:\n%s\n", encoded_str2);
        free(encoded_str);

        // 关闭句柄
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return encoded_str2;
    }
    else
    {
        // 关闭句柄
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        // printf("未找到问号\n");
        return NULL;
    }
}
