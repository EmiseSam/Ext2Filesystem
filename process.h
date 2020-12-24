/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#ifndef PROCESS_H
#define PROCESS_H

#include "directory.h"

char directories[128][NAME_SIZE];
/**
 * @description:  分解路径
 * @param agrv_path 命令参数中的路径
 * @param name 命令参数中的路径中的最后一个参数名称
 * @return 路径中的参数数量
 */
int split(char *agrv_path, char *name);

#endif