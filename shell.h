#ifndef SHELL_H
#define SHELL_H

#define MAXARGS 10
#define MAXWORD 30
#define BUF_SIZE 128

// 无效字符
char whitespace[] = " \t\r\n\v";
// 分解命令存放在二维数组中
char args[MAXARGS][MAXWORD];
// 暂存命令
char buf[BUF_SIZE];

#endif