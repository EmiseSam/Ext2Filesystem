/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <inttypes.h>

#define ITEM_SIZE 128           // 一个目录项占用128个字节
#define ITEM_PER_BLOCK 4        // 一个物理磁盘块可容纳4个目录项
#define NAME_SIZE 121           // 文件名最大为121个字节


struct dir_item {               // 目录项一个更常见的叫法是 dirent(directory entry)
    uint32_t inode_id;          // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;             // 当前目录项是否有效 
    uint8_t type;               // 当前目录项类型（文件/目录）
    char name[121];             // 目录项表示的文件/目录的文件名/目录名
};

/**
 * @description:  初始化一个目录项
 * @param dir_item 初始化的目录项指针
 * @param inode_id 当前目录项表示的文件/目录的对应inode
 * @param valid 当前目录项是否有效
 * @param type  当前目录项类型（文件/目录）
 * @param name  目录项表示的文件/目录的文件名/目录名
 * @return {return 1}
 */
int init_dir_item(struct dir_item *dir_item, int inode_id, int valid, int type, char *name);

/**
 * @description:  将一个数据块中的目录项写入对应的磁盘块
 * @param ditems[] 一个数据块中的目录项
 * @param data_block_index 对应的磁盘块号
 * @return {returns 1 on success, 0 otherwise}
 */
int write_dir_items(struct dir_item ditems[], int data_block_index);

/**
 * @description:  从磁盘块中读取对应的数据块中的目录项
 * @param ditems[] 一个数据块中的目录项
 * @param data_block_index 对应的磁盘块号
 * @return {returns 1 on success, 0 otherwise}
 */
int read_dir_items(struct dir_item ditems[], int data_block_index);

/**
 * @description: 插入一个目录项
 * @param dir_inode 目录项的上一级目录的索引结点
 * @param name 目录项的名称
 * @param type 目录项指向的索引结点类型（文件\目录）
 * @param dir_inode_index 目录项的上一级目录的索引结点号
 * @param index 目录项指向的索引结点号
 * @return {returns 1 on success, 0 otherwise}
 */
int insert_dir_item(struct inode *dir_inode, char *name, int type, int dir_inode_index, int index);

/**
 * @description:  
 * @param dir_inode 需要搜索的目录项的上一级索引结点
 * @param name 需要搜索的目录项的名称
 * @param index 需要搜索的目录项指向的索引结点号
 * @param type 需要搜索的目录项的类型（文件\目录）
 * @return {returns 1 on success, 0 otherwise}
 */
int find_dir_item(struct inode *dir_inode, char *name, int *index, int type);
#endif