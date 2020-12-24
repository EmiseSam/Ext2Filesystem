#include "disk.h"
#include "inode.h"
#include "filesystem.h"
#include "directory.h"
#include <stdio.h>

int init_inode(struct inode *node, int size, int type, int link)
{
    node->size = size;
    node->link = link;
    node->file_type = type;
    for (int i = 0; i < 6; i++)
    {
        node->data_block_point[i] = 0;
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

int add_inode(int *index, int type)
{
    // 获取一个空的inode结点
    int inode_index = alloc_inode();
    if (inode_index < 0)
    {
        printf("Failed to alloc inode.\n");
        return 0;
    }
    *index = inode_index;

    // 按目录类型初始化搜寻到的空inode结点
    struct inode *node = &inodes[inode_index];
    if (!init_inode(node, 0, type, 1))
    {
        printf("Failed to initial inode.\n");
        return 0;
    }

    // 分配一个空的数据块
    int data_block_index = alloc_block();
    if (data_block_index < 0)
    {
        printf("alloc block failed.\n");
        return 0;
    }

    // 编辑对应的索引结点
    node->data_block_point[0] = data_block_index;
    node->link = 1;

    if (type = DIR)
    {
        struct dir_item ditems[ITEM_PER_BLOCK * 2];
        for (int i = 0; i < ITEM_PER_BLOCK * 2; i++)
        {
            // 按照目录类型初始化目录项
            if (!init_dir_item(&ditems[i], inode_index, 0, DIR, ""))
            {
                printf("Failed to initial directory item.\n");
                return 0;
            }
        }

        // 将初始化成功的目录项写入数据块
        if (!write_dir_items(ditems, data_block_index))
        {
            return 0;
        }
    }

    // 将本次创建的索引结点写入索引表
    if (!write_inode(node, inode_index))
    {
        return 0;
    }

    // 写回超级块
    if (!write_sp_block())
    {
        return 0;
    }

    return 1;
}