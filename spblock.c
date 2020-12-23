#include "spblock.h"
#include "disk.h"
#include <stdlib.h>
#include <stdio.h>

int init_super_block()
{
    sp_block.magic_num = MAGICNUM;
    sp_block.free_block_count = BLOCK_AMOUNT;
    sp_block.free_inode_count = INODE_AMOUNT;
    sp_block.dir_inode_count = 0;
    for (int i = 0; i < BLOCK_MAP; i++)
    {
        sp_block.block_map[i] = 0;
    }
    for (int i = 0; i < INODE_MAP; i++)
    {
        sp_block.inode_map[i] = 0;
    }
}

int write_super_block()
{
    char buf[DEVICE_BLOCK_SIZE * 2];
    char *p = (char *)&sp_block;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        buf[i] = p[i];
    }
    if (disk_write_block(0, &buf[0]) < 0)
        return 0;
    if (disk_write_block(1, &buf[DEVICE_BLOCK_SIZE]) < 0)
        return 0;
    return 1;
}

int read_super_block()
{
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

    char *p = (char *)&sp_block;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        p[i] = buf[i];
    }
    return 1;
}

int alloc_block()
{
    if (sp_block.free_block_count == 0)
        return -1;

    for (int i = 0; i < BLOCK_MAP; i++)
    {
        uint32_t block = sp_block.block_map[i];
        for (int j = 0; j < 32; j++)
        {
            // 按位与找到空闲的数据块
            if ((block >> j) & 1)
                continue;
            else
            {
                sp_block.free_block_count--;
                sp_block.block_map[i] = sp_block.block_map[i] | 1 << j;
                write_sp_block();
                return (i * 32 + j) + 66;
            }
        }
    }
    return -1;
}

int alloc_inode()
{
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
                sp_block.inode_map[i] = sp_block.inode_map[i] | 1 << j;
                write_sp_block();
                return i * 32 + j;
            }
        }
    }
    return -1;
}