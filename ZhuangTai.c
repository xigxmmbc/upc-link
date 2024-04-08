#include <windows.h>
#include <wininet.h>
#include <stdio.h>
char *uid(int sernum);
extern char *userId;

int ZhuangTai()
{

    HINTERNET hInternet, hConnect, hRequest;
    char *szHeaders = "Accept: */*\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n";
    char *szContentTypeHeader = "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
    char szPostData[200];
    char *szHostName = "lan.upc.edu.cn";
    char *szUrlPath = "/eportal/InterFace.do?method=getOnlineUserInfo";
    if (userId == NULL)
    {
        char userId[] = "33303636333233303430303235";
        sprintf(szPostData, "userId=%s", userId);
        // printf("合并后的字符串为: %s\n", szPostData);
    }
    else
    {
        sprintf(szPostData, "userId=%s", userId);
        // printf("合并后的字符串为: %s\n", szPostData);
    }
    // free(userId);

    // 初始化WinINet
    hInternet = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        // printf("InternetOpen failed: %d\n", GetLastError());
        return 0;
    }

    // 连接到服务器
    hConnect = InternetConnect(hInternet, szHostName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        // printf("InternetConnect failed: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return 0;
    }

    // 创建请求
    hRequest = HttpOpenRequest(hConnect, "POST", szUrlPath, NULL, NULL, NULL, 0, 0);
    if (!hRequest)
    {
        // printf("HttpOpenRequest failed: %d\n", GetLastError());
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // 添加请求头
    if (!HttpAddRequestHeaders(hRequest, szHeaders, strlen(szHeaders), HTTP_ADDREQ_FLAG_ADD))
    {
        // printf("HttpAddRequestHeaders failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // 添加Content-Type请求头
    if (!HttpAddRequestHeaders(hRequest, szContentTypeHeader, strlen(szContentTypeHeader), HTTP_ADDREQ_FLAG_ADD))
    {
        // printf("HttpAddRequestHeaders failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // 发送请求
    if (!HttpSendRequest(hRequest, NULL, 0, szPostData, strlen(szPostData)))
    {
        // printf("HttpSendRequest failed: %d\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // 获取响应
    char szBuffer[1024];
    DWORD dwRead;
    InternetReadFile(hRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead);
    szBuffer[dwRead] = '\0';
    // printf("%s", szBuffer);

    char *userIndexPtr = strstr(szBuffer, "realService");
    if (userIndexPtr != NULL)
    {
        char *colonPtr = strchr(userIndexPtr, ':');
        if (colonPtr != NULL)
        {
            char *valueStartPtr = colonPtr + 2;
            char *commaPtr = strchr(valueStartPtr, '\"');
            if (commaPtr != NULL)
            {
                // 计算值的长度
                size_t valueLength = commaPtr - valueStartPtr;
                // 分配内存并拷贝值
                char userIndex[valueLength + 1];
                strncpy(userIndex, valueStartPtr, valueLength);
                userIndex[valueLength] = '\0';
                // printf("%s\n", userIndex);
                if (strcmp(userIndex, "default") == 0)
                { // 关闭句柄
                    InternetCloseHandle(hRequest);
                    InternetCloseHandle(hConnect);
                    InternetCloseHandle(hInternet);
                    return 2;
                }
                else if (strcmp(userIndex, "cmcc") == 0)
                {
                    // 关闭句柄
                    InternetCloseHandle(hRequest);
                    InternetCloseHandle(hConnect);
                    InternetCloseHandle(hInternet);
                    return 1;
                }
            }
        }
    }

    // 关闭句柄
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return 0;
}
int link(int sernum);
// int main()
// {
//     link(1);
//     ZhuangTai();
//     return 0;
// }
