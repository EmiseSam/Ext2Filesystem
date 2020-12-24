/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#ifndef COMMEND_H
#define COMMEND_H

/**
 * @description:  在目标路径下创建一个文件夹
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void mkdir(char *argv[], int argc);

/**
 * @description:  在目标路径下创建一个文件夹
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void touch(char *argv[], int argc);

/**
 * @description:  显示目录下的内容
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void ls(char *argv[], int argc);

/**
 * @description:  复制文件
 * @param argv 命令参数指针数组
 * @param argc 命令参数个数
 */
void cp(char *argv[], int argc);

/**
 * @description:  有关命令的帮助
 */
void help();

/**
 * @description:  关闭shell
 */
void shutdown();


#endif