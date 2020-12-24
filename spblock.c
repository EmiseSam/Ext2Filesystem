#include <stdlib.h>
#include <stdio.h>
#include "disk.h"
#include "spblock.h"

int init_super_block()
{
    sp_block.magic_num = MAGICNUM;
    sp_block.free_data_block_count = DATA_BLOCK_AMOUNT;
    sp_block.free_inode_count = INODE_AMOUNT;
    sp_block.dir_inode_count = 0;
    for (int i = 0; i < DATA_BLOCK_MAP; i++)
    {
        sp_block.data_block_map[i] = 0;
    }
    for (int i = 0; i < INODE_MAP; i++)
    {
        sp_block.inode_map[i] = 0;
    }
}

int write_super_block()
{
    char buf[DEVICE_BLOCK_SIZE * 2];
    // 将超级块信息存入缓冲区
    char *p = (char *)&sp_block;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        buf[i] = p[i];
    }
    // 将缓冲区内容写入磁盘
    if (disk_write_block(0, &buf[0]) < 0)
    {
        return 0;
    }
    if (disk_write_block(1, &buf[DEVICE_BLOCK_SIZE]) < 0)
    {
        return 0;
    }
    return 1;
}

int read_super_block()
{
    // 从磁盘读取超级块信息到缓冲区
    char buf[DEVICE_BLOCK_SIZE * 2];
    if (disk_read_block(0, &buf[0]) < 0)
    {
        printf("Fail to read from disk.\n");
        return 0;
    }
    if (disk_read_block(1, &buf[DEVICE_BLOCK_SIZE]) < 0)
    {
        printf("Fail to read from disk.\n");
        return 0;
    }

    // 从缓冲区中读取超级块信息
    char *p = (char *)&sp_block;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        p[i] = buf[i];
    }
    return 1;
}

int alloc_block()
{
    // 没有空闲数据块
    if (sp_block.free_data_block_count == 0)
        return -1;

    for (int i = 0; i < DATA_BLOCK_MAP; i++)
    {
        uint32_t data_block = sp_block.data_block_map[i];
        for (int j = 0; j < 32; j++)
        {
            // 按位与找到空闲的数据块
            if ((data_block >> j) & 1)
                continue;
            else
            {
                sp_block.free_data_block_count--;
                sp_block.data_block_map[i] = sp_block.data_block_map[i] | (1 << j);
                write_sp_block();
                return (i * 32 + j) + 66;
            }
        }
    }
    return -1;
}

int alloc_inode()
{
    // 没有空闲inode结点
    if (sp_block.free_inode_count == 0)
        return -1;

    for (int i = 0; i < INODE_MAP; i++)
    {
        uint32_t inode = sp_block.inode_map[i];
        for (int j = 0; j < 32; j++)
        {
            // 按位与找到空闲的inode结点
            if ((inode >> j) & 1)
                continue;
            else
            {
                sp_block.free_inode_count--;
                sp_block.inode_map[i] = sp_block.inode_map[i] | (1 << j);
                write_sp_block();
                return i * 32 + j;
            }
        }
    }
    return -1;
}