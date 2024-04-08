#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 全局变量
char *XueHao = NULL;
char *password = NULL;

// // 函数声明
// void readLinesFromFile();

// int main()
// {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//     {
//         printf("当前工作目录：%s\n", cwd);
//     }
//     else
//     {
//         perror("getcwd() error");
//         return 1;
//     }
//     const char *filename = "C:\\Users\\admin\\Desktop\\lianjie\\mima.txt"; // 替换为你的文件名

//     readLinesFromFile(filename);

//     // 打印全局变量a和b的内容
//     printf("第一行：%s\n", a);
//     printf("第二行：%s\n", b);

//     // 释放内存
//     free(a);
//     free(b);

//     return 0;
// }

int mima()
{
    char cwd[100];      // 用于存储当前工作目录的缓冲区
    char filepath[100]; // 用于存储拼接后的文件路径的缓冲区
    getcwd(cwd, sizeof(cwd));
    // 打印当前工作目录
    printf("Current working directory: %s\n", cwd);
    // 构建文件路径
    snprintf(filepath, sizeof(filepath), "%s%s%s", cwd, "/", "mima.txt");
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("无法打开文件mima %s\n");
        return 0;
    }

    char line[1000]; // 假设每行不超过1000个字符

    // 读取第一行
    if (fgets(line, sizeof(line), file) != NULL)
    {
        // 分配足够的内存来存储第一行的内容
        XueHao = (char *)malloc(strlen(line) + 1);
        if (XueHao == NULL)
        {
            printf("内存分配失败\n");
            exit(1);
        }
        strncpy(XueHao, line, strlen(line) - 1);
        XueHao[strlen(line + 1)] = '\0';
    }

    // 读取第二行
    if (fgets(line, sizeof(line), file) != NULL)
    {
        // 分配足够的内存来存储第二行的内容
        password = (char *)malloc(strlen(line) + 1);
        if (password == NULL)
        {
            printf("内存分配失败\n");
            exit(1);
        }
        strcpy(password, line);
    }

    fclose(file);
}