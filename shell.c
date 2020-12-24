#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "filesystem.h"
#include "commend.h"

int getcmd(char *buf, int nbuf)
{
    // 使用“@”来作为命令行提示符
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    fgets(buf, nbuf, stdin);
    if (buf[0] == 0) // EOF
        return 0;
    return 1;
}

void setargs(char *cmd, char *argv[], int *argc)
{
    // argv[i]和args[i][0]同时指向命令分解后各个参数的存储位置的首地址
    for (int i = 0; i < MAXARGS; i++)
    {
        argv[i] = &args[i][0];
    }

    int i = 0; // 输入的命令中第i个word
    int j = 0;
    for (; cmd[j] != '\n' && cmd[j] != '\0'; j++)
    {
        // 无视无效字符
        while (strchr(whitespace, cmd[j]))
        {
            j++;
        }
        // argv[i]分别指向输入的命令中的一个word的开头
        argv[i++] = cmd + j;

        // 找到每个word之后的第一个无效字符
        while (strchr(whitespace, cmd[j]) == 0)
        {
            j++;
        }
        cmd[j] = '\0';
    }
    // 其中argv[0]指向命令字;
    // 最后一个参数argv[size-1]必须为0
    argv[i] = 0;
    // *argc存放参数个数
    *argc = i;
}

// 运行命令
void runcmd(char *argv[], int argc)
{
    char* cmd = argv;
    if (!strcmp(cmd, "ls"))
        ls(argv, argc);
    else if (!strcmp(cmd, "mkdir"))
        mkdir(argv, argc);
    else if (!strcmp(cmd, "touch"))
        touch(argv, argc);
    else if (!strcmp(cmd, "cp"))
        cp(argv, argc);
    else if (!strcmp(cmd, "shutdown"))
        shutdown();
    else if (!strcmp(cmd, "help"))
        help();
    else
        printf("'%s': command not found.\nSee \'help\'\n", cmd);
}

int main()
{
    printf("---FileSystem shell---\n");
    if (!open_system())
    {
        printf("Failed to open FileSystem.\n");
        shutdown();
    }

    // 读取命令
    while (getcmd(buf, sizeof(buf)))
    {
        // 处理命令参数
        setargs(buf, argv, &argc);
        // 运行命令
        runcmd(argv, argc);
    }
    return 0;
}