/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#include <stdio.h>
#include "disk.h"
#include "spblock.h"
#include "inode.h"
#include "filesystem.h"

int init_system()
{
    // 初始化超级块
    init_super_block(&sp_block);

    // 将超级块写入磁盘块
    if (!write_super_block(&sp_block))
    {
        printf("Failed to initial super bolck.\n");
        return -1;
    }

    for (int i = 0; i < INODE_AMOUNT; i++)
    {
        // 初始化索引表
        if (!init_inode(&inodes[i], 0, File, 0))
        {
            printf("Failed to initial inode block.\n");
            return -1;
        }
        // 将索引表写入磁盘块
        if (!write_inode(&inodes[i], i))
        {
            printf("Failed to write inode block.\n");
            return -1;
        }
    }

    // 初始化根目录
    int index = 0;
    add_inode(&index, Dir);
    if (index != 0) {
        printf("initial root dir failed.\n");
        return 0;
    }
    printf("Initial super bolck.\n");
    return 1;
}

int open_system()
{
    if (open_disk() < 0)
    {
        printf("Fail to open disk.\n");
        return 0;
    }

    // 核对幻数
    if (read_super_block(&sp_block) && sp_block.magic_num == MAGICNUM)
    {
        // 接着读取上一次的索引表配置
        for (int i = 0; i < INODE_AMOUNT; i++)
        {
            if (!read_inode(&inodes[i], i))
            {
                printf("Failed to load inode block.\n");
                return -1;
            }
        }
        printf("Ext2 already exists. Enter shell\n");
    }
    else
    {
        // 没有配置过索引表，则进行初始化文件系统
        printf("File System Unkonwn or doesn't exist. Format disk and build a new file system.\n");
        init_system();
    }
    return 1;
}

int close_system()
{
    // 写回超级块
    if (!write_super_block(&sp_block))
    {
        printf("Failed to save super block.\n");
        return -1;
    }
    printf("Saved super block.\n");

    if (close_disk() < 0)
    {
        printf("Failed to close disk.\n");
        return -1;
    }
    printf("Closed disk.\n");
    return 1;
}