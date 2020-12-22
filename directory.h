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

#endif