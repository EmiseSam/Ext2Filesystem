#include "disk.h"
#include "inode.h"
#include <stdio.h>

int init_inode(struct inode *node, int size, int type, int link)
{
    node->size = size;
    node->link = link;
    node->file_type = type;
    for (int i = 0; i < 6; i++)
    {
        node->block_point[i] = 0;
    }
    return 1;
}

int write_inode(struct inode *node, int index)
{
    int block_index = 2 + index / INODES_PER_BLOCK;
    int inode_head = INODE_SIZE * (index % INODES_PER_BLOCK);

    char buf[DEVICE_BLOCK_SIZE];

    // 取出对应的inode块
    if (disk_read_block(block_index, &buf[0]) < 0)
    {
        return 0;
    }

    // 从buf中修改对应的inode块
    char *p = (char *)node;
    for (int i = 0; i < INODE_SIZE; i++)
    {
        buf[inode_head + i] = p[i];
    }
    if (disk_write_block(block_index, &buf[0]) < 0)
    {
        return 0;
    }

    return 1;
}

int read_inode(struct inode *node, int index)
{
    int block_index = 2 + index / INODES_PER_BLOCK;
    int inode_head = INODE_SIZE * (index % INODES_PER_BLOCK);

    char buf[DEVICE_BLOCK_SIZE];

    // 取出对应的inode块
    if (disk_read_block(block_index, buf))
    {
        return 0;
    }
    // 读取对应的inode块到buf
    char *p = (char *)node;
    for (int i = 0; i < INODE_SIZE; i++)
    {
        p[i] = buf[inode_head + i];
    }
    return 1;
}