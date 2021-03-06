#include <stdio.h>
#include <string.h>
#include "spblock.h"
#include "inode.h"
#include "disk.h"
#include "filesystem.h"
#include "directory.h"

int init_dir_item(struct dir_item *dir_item, int inode_id, int valid, int type, char *name)
{
    dir_item->inode_id = inode_id;
    dir_item->valid = valid;
    dir_item->type = type;

    // 初始化目录项名称为空
    for (int i = 0; i < NAME_SIZE; i++)
    {
        dir_item->name[i] = '\0';
    }

    // 赋值名称
    for (int i = 0; name[i] != '\0'; i++)
    {
        dir_item->name[i] = name[i];
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

int read_dir_items(int data_block_index, struct dir_item ditems[])
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

int insert_dir_item(struct inode *dir_inode, char *name, int type, int dir_inode_index, int index)
{
    // 按已链接数据块查找空的目录项
    for (int i = 0; i < 6; i++)
    {
        struct dir_item ditems[ITEM_PER_BLOCK * 2];
        if (!dir_inode->data_block_point[i])
        {
            continue;
        }
        read_dir_items(dir_inode->data_block_point[i], ditems);

        // 在数据块中的目录项查找空的目录项
        for (int j = 0; j < ITEM_PER_BLOCK * 2; j++)
        {
            if (!ditems[j].valid)
            {
                // 编辑新的目录项
                ditems[j].inode_id = index;
                ditems[j].type = type;
                ditems[j].valid = 1;
                strcpy(ditems[j].name, name);
                dir_inode->size += ITEM_SIZE;
                dir_inode->link++;

                // 将目录项数组写回数据块
                if (!write_dir_items(ditems, dir_inode->data_block_point[i]))
                {
                    return 0;
                }

                // 将inode信息写回数据块
                if (!write_inode(dir_inode, dir_inode_index))
                {
                    return 0;
                }
                return 1;
            }
        }
    }

    // 新链接一个空的数据块来存目录项
    int data_block_index = alloc_block();
    int k = 0;
    for (k = 0; dir_inode->data_block_point[k] && k < 6; k++)
        ;
    if (k == 6)
    {
        printf("Too many dir_item.\n");
        return 0;
    }
    dir_inode->data_block_point[k] = data_block_index;
    dir_inode->link++;

    struct dir_item ditems[ITEM_PER_BLOCK * 2];
    for (int i = 0; i < ITEM_PER_BLOCK * 2; i++)
    {
        if (!init_dir_item(&ditems[i], 0, 0, Dir, ""))
        {
            printf("Failed to initial directory item.\n");
            return 0;
        }
    }

    // 编辑新的目录项
    ditems[0].inode_id = index;
    ditems[0].type = type;
    ditems[0].valid = 1;
    strcpy(ditems[0].name, name);
    dir_inode->size += ITEM_SIZE;
    dir_inode->link++;

    // 将目录项数组写回数据块
    if (!write_dir_items(ditems, data_block_index))
    {
        return 0;
    }

    // 将inode信息写回数据块
    if (!write_inode(dir_inode, dir_inode_index))
    {
        return 0;
    }
    return 1;
}

int find_dir_item(struct inode *dir_inode, char *name, int *index, int type)
{
    for (int i = 0; i < 6; i++)
    {
        if (dir_inode->data_block_point[i] < 0)
        {
            continue;
        }
        struct dir_item ditems[ITEM_PER_BLOCK * 2];
        read_dir_items(dir_inode->data_block_point[i], ditems);

        // 逐个搜索目录项
        for (int j = 0; j < ITEM_PER_BLOCK * 2; j++)
        {
            if (ditems[j].valid && !strcmp(name, ditems[j].name) && (ditems[j].type == type))
            {
                *index = ditems[j].inode_id;
                return 1;
            }
        }
    }
    return 0;
}

void print_dir_item(struct inode *dir_inode)
{
    for (int i = 0; i < 6; i++)
    {
        if (!dir_inode->data_block_point[i])
        {
            continue;
        }
        struct dir_item items[ITEM_PER_BLOCK * 2];

        read_dir_items(dir_inode->data_block_point[i], items);

        for (int j = 0; j < ITEM_PER_BLOCK * 2; j++)
        {
            if (items[j].valid)
            {
                printf("(%s ", items[j].name);
                if (items[j].type == Dir)
                {
                    printf("dictionary)\t");
                }
                else
                {
                    printf("file)\t");
                }
                printf("\n");
            }
        }
    }
    return;
}