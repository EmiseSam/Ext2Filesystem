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

int write_inode(struct inode *node, int inode_index)
{
    // 物理磁盘块号
    int block_index = 2 + inode_index / INODES_PER_BLOCK;
    // inode对应的首地址
    int inode_head = INODE_SIZE * (inode_index % INODES_PER_BLOCK);

    char buf[DEVICE_BLOCK_SIZE];

    // 取出对应的物理磁盘块
    if (disk_read_block(block_index, &buf[0]) < 0)
    {
        return 0;
    }

    // 将inode写入缓冲区对应的位置
    char *p = (char *)node;
    for (int i = 0; i < INODE_SIZE; i++)
    {
        buf[inode_head + i] = p[i];
    }

    // 将缓冲区内容写入物理磁盘块
    if (disk_write_block(block_index, &buf[0]) < 0)
    {
        return 0;
    }
    return 1;
}

int read_inode(struct inode *node, int inode_index)
{
    // 物理磁盘块号
    int block_index = 2 + inode_index / INODES_PER_BLOCK;
    // inode对应的首地址
    int inode_head = INODE_SIZE * (inode_index % INODES_PER_BLOCK);

    char buf[DEVICE_BLOCK_SIZE];

    // 读取对应的物理磁盘块到buf
    if (disk_read_block(block_index, buf))
    {
        return 0;
    }
    // 从缓冲区中读取inode信息
    char *p = (char *)node;
    for (int i = 0; i < INODE_SIZE; i++)
    {
        p[i] = buf[inode_head + i];
    }
    return 1;
}