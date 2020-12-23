#include "directory.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>
#include "inode.h"
#include "filesystem.h"

int init_dir_item(struct dir_item *ditem, int inode_id, int valid, int type, char *name)
{
    ditem->inode_id = inode_id;
    ditem->valid = valid;
    ditem->type = type;

    // 初始化目录项名称为空
    for (int i = 0; i < NAME_SIZE; i++)
    {
        ditem->name[i] = '\0';
    }

    // 赋值名称
    for (int i = 0; name[i] != '\0'; i++)
    {
        ditem->name[i] = name[i];
    }
    return 1;
}

int write_dir_items(struct dir_item ditems[], int data_block_index)
{
    char buf[DEVICE_BLOCK_SIZE * 2];

    // 将目录项一次存入缓冲区
    char *item_pointer = (char *)ditems;
    for (int i = 0; i < DEVICE_BLOCK_SIZE * 2; i++)
    {
        buf[i] = item_pointer[i];
    }

    // 将缓存中的数据存入对应的物理磁盘块
    if (disk_write_block(data_block_index * 2, &buf[0]) < 0)
    {
        return 0;
    }
    if (disk_write_block(data_block_index * 2 + 1, &buf[DEVICE_BLOCK_SIZE]) < 0)
    {
        return 0;
    }
    return 1;
}

int read_dir_items(struct dir_item ditems[], int data_block_index)
{
    char buf[DEVICE_BLOCK_SIZE * 2];

    // 读取对应的物理磁盘块到缓冲区
    if (disk_read_block(data_block_index * 2, &buf[0]) < 0)
    {
        return 0;
    }
    if (disk_read_block(data_block_index * 2 + 1, &buf[DEVICE_BLOCK_SIZE]) < 0)
    {
        return 0;
    }

    // 从缓冲区中读取目录项
    char *item_pointer = (char *)ditems;
    for (int i = 0; i < DEVICE_BLOCK_SIZE * 2; i++)
    {
        item_pointer[i] = buf[i];
    }
    return 1;
}

int insert_dir_item(struct inode *dir, char *name, int type, int dir_inode_index, int index)
{
    // 按已链接数据块查找空的目录项
    for (int i = 0; i < dir->link; i++)
    {
        struct dir_item items[ITEM_PER_BLOCK * 2];
        read_dir_items(dir->data_block_point[i], items);

        // 在数据块中的目录项查找空的目录项
        for (int j = 0; j < ITEM_PER_BLOCK * 2; j++)
        {
            if (!items[j].valid)
            {
                items[j].inode_id = index;
                items[j].type = type;
                items[j].valid = 1;
                strcpy(items[j].name, name);
                dir->size += ITEM_SIZE;

                // 将目录项数组写回数据块
                if (!write_dir_items(items, dir->data_block_point[i]))
                {
                    return 0;
                }

                // 将inode信息写回数据块
                if (!write_inode(dir, dir_inode_index))
                {
                    return 0;
                }
                return 1;
            }
        }
    }

    // 新链接一个空的数据块来存目录项
    int data_block_index = alloc_block();
    dir->data_block_point[dir->link] = data_block_index;
    dir->link++;

    struct dir_item items[ITEM_PER_BLOCK * 2];
    for (int i = 0; i < ITEM_PER_BLOCK * 2; i++)
    {
        if (!init_dir_item(&items[i], 0, 0, Dir, ""))
        {
            printf("Failed to initial directory item.\n");
            return 0;
        }
    }

    items[0].inode_id = index;
    items[0].type = type;
    items[0].valid = 1;
    strcpy(items[0].name, name);
    dir->size += ITEM_SIZE;

    // 将目录项数组写回数据块
    if (!write_dir_items(items, data_block_index))
    {
        return 0;
    }

    // 将inode信息写回数据块 
    if (!write_inode(dir, dir_inode_index))
    {
        return 0;
    }
    return 1;
}

int init_dir(int *index)
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
    struct inode *node = &inode[inode_index];
    if (!init_inode(node, 0, Dir, 1))
    {
        printf("Failed to initial inode.\n");
        return 0;
    }

    // 分配一个空的数据块，用于装载目录项
    int data_block_index = alloc_block();
    if (data_block_index < 0)
    {
        printf("alloc block failed.\n");
        return 0;
    }

    // 编辑对应的索引结点
    node->data_block_point[0] = data_block_index;
    node->link = 1;
    node->size += ITEM_SIZE;

    struct dir_item ditems[ITEM_PER_BLOCK * 2];
    for (int i = 0; i < ITEM_PER_BLOCK * 2; i++)
    {
        // 按照目录类型初始化目录项
        if (!init_dir_item(&ditems[i], inode_index, 0, Dir, ""))
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
