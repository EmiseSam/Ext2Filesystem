/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#ifndef SHELL_H
#define SHELL_H

#define MAXARGS 10
#define MAXWORD 30
#define BUF_SIZE 128

// 无效字符
char whitespace[] = " \t\r\n\v";
// 暂存命令
char buf[BUF_SIZE];
// 输入的命令参数数组
char *argv[MAXARGS];
// 命令参数个数
int argc = -1;
// 分解后的命令
char args[MAXARGS][MAXWORD];

/**
 * @description:  从输入缓冲区获取命令
 * @param buf 缓冲区指针
 * @param nbuf 缓冲区大小
 * @return returns 1 on success, 0 otherwise.
 */
int getcmd(char *buf, int nbuf);

/**
 * @description:  处理命令的每一个参数
 * @param cmd 命令头指针
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void setargs(char *cmd, char *argv[], int *argc);

/**
 * @description:  根据命令执行对应函数
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void runcmd(char *argv[], int argc);

/**
 * @description:  主函数
 * @return 0
 */
int main();
#endif