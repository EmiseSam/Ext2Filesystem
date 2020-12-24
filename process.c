/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h"
#include "process.h"

int split(char *agrv_path, char *name)
{
    // 需要解析的路径
    char *path_point = (char *)agrv_path;
    memset(directories, '\0', sizeof(directories));
    int count = 0;

    // 忽略.
    if (path_point[0] == '.')
    {
        path_point++;
    }

    if (path_point[0] != '/')
    {
        int i = 0;
        while (path_point[i] != '/' && path_point[i] != '\0')
        {
            directories[count][i] = path_point[i];
            i++;
        }
        directories[count][i + 1] = '\0';
        path_point += i;
        count++;
    }

    for (int i = 0; path_point[i] != '\0'; i++)
    {
        if (path_point[i] == '/')
        {
            int j = 1;
            while (path_point[i + j] != '/' && path_point[i + j] != '\0')
            {
                directories[count][j - 1] = path_point[i + j];
                j++;
            }
            directories[count][j - 1] = '\0';
            count++;
            i = i + j - 1;
        }
    }

    for (int i = 0; directories[count - 1][i] != '\0'; i++)
    {
        name[i] = directories[count - 1][i];
    }
    return count;
}