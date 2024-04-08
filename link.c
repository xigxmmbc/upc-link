#include <windows.h>
#include <wininet.h>
#include <stdio.h>
char *getip();

char *userId = NULL;
extern char *XueHao;
extern char *password;

int link(int sernum)
{

    char *queryString = getip();
    // char XueHao[] = "S2304";
    // char password[] = "4b1ae1f0cebd476f3bd3fe2a642b97639475a31ace409936086f2d391ddf";
    char *service;
    if (sernum == 2)
    {
        service = (char *)malloc(strlen("default") + 1); // 分配足够的内存
        if (service == NULL)
        {
            perror("Memory allocation failed");
            return 1;
        }
        strcpy(service, "default"); // 复制字符串到分配的内存中
    }
    else
    {
        service = (char *)malloc(strlen("cmcc") + 1); // 分配足够的内存
        if (service == NULL)
        {
            perror("Memory allocation failed");
            return 1;
        }
        strcpy(service, "cmcc"); // 复制字符串到分配的内存中
    }

    // char queryString[] = "wlanuserip%252F62%253A1473.0";
    char operatorPwd[] = "";
    char operatorUserId[] = "";
    char validcode[] = "";
    char passwordEncrypt[] = "true";

    HINTERNET hInternet, hConnect, hRequest;
    char *szHeaders = "Accept: */*\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n";
    char *szContentTypeHeader = "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
    char szPostData[600];
    // cb9met6utorUserId=&validcode=&passwordEncrypt=true";
    char *szHostName = "lan.upc.edu.cn";
    char *szUrlPath = "/eportal/InterFace.do?method=login";
    if (queryString == NULL)
    {
        char queryString[] = "wlanuseri53A1473.0";
        sprintf(szPostData, "userId=%s&password=%s&service=%s&queryString=%s&operatorPwd=%s&operatorUserId=%s&validcode=%s&passwordEncrypt=%s",
                XueHao, password, service, queryString, operatorPwd, operatorUserId, validcode, passwordEncrypt);
        // printf("合并后的字符串为: %s\n", szPostData);
    }
    else
    {
        sprintf(szPostData, "userId=%s&password=%s&service=%s&queryString=%s&operatorPwd=%s&operatorUserId=%s&validcode=%s&passwordEncrypt=%s",
                XueHao, password, service, queryString, operatorPwd, operatorUserId, validcode, passwordEncrypt);
        printf("合并后的字符串为: %s\n", szPostData);
    }
    free(service);
    free(queryString);

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
    InternetReadFile(hRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead);
    szBuffer[dwRead] = '\0';
    printf("%s\n", szBuffer);

    char *userIndexPtr = strstr(szBuffer, ":");
    if (userIndexPtr != NULL)
    {
        char *colonPtr = strchr(userIndexPtr, '\"');
        if (colonPtr != NULL)
        {
            char *valueStartPtr = colonPtr + 1;
            char *commaPtr = strchr(valueStartPtr, '\"');
            if (commaPtr != NULL)
            {
                // 计算值的长度
                size_t valueLength = commaPtr - valueStartPtr;
                // 分配内存并拷贝值
                char userIndex[valueLength + 1];
                strncpy(userIndex, valueStartPtr, valueLength);
                userIndex[valueLength] = '\0';
                userId = strdup(userIndex);
                // printf("%s\n", userIndex);

                // 关闭句柄
                InternetCloseHandle(hRequest);
                InternetCloseHandle(hConnect);
                InternetCloseHandle(hInternet);
                return 0;
            }
        }
    }

    // 关闭句柄
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}
// char *getip();
// int main()
// {
//     char *queryString = getip();
//     link(1, queryString);
//     return 0;
// }
