#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "filesystem.h"

int getcmd(char *buf, int nbuf)
{
    // 使用“@”来作为命令行提示符
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    fgets(buf, nbuf, stdin);
    if (buf[0] == 0) // EOF
        return -1;
    return 0;
}

// 处理命令的每一个参数
void setargs(char *cmd, char *argv[], int *argc)
{
    // argv作为每一个word的首地址
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
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            // “ | ”即pipe，说明后面至少还有一个命令要执行
            execPipe(argv, argc);
        }
    }

    // 仅处理一个命令：查看是否有重定向符
    for (int i = 1; i < argc; i++)
    {
        // “ > ” ，执行输出重定向
        if (!strcmp(argv[i], ">"))
        {
            // 关闭stdout
            close(1);
            // 打开重定向输出文件
            open(argv[i + 1], O_CREATE | O_WRONLY);
            argv[i] = 0;
        }
        // “ < ” ，执行输入重定向
        if (!strcmp(argv[i], "<"))
        {
            // 关闭stdin
            close(0);
            // 打开重定向输入文件
            open(argv[i + 1], O_RDONLY);
            argv[i] = 0;
        }
    }
    exec(argv[0], argv);
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
    while (getcmd(buf, sizeof(buf)) >= 0)
    {
        char *argv[MAXARGS];
        int argc = -1;
        // 处理命令参数
        setargs(buf, argv, &argc);
        // 运行命令
        runcmd(argv, argc);
    }
}