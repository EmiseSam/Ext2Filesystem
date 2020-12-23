#ifndef INODE_H
#define INODE_H

#include <inttypes.h>

#define File 1              // 文件类型为2
#define Dir 2               // 目录类型为2
#define INODE_SIZE 32       // 一个inode占用32个字节
#define INODES_PER_BLOCK 16 // 一个物理磁盘块可容纳16个inode

struct inode
{
    uint32_t size;           // 文件大小
    uint16_t file_type;      // 文件类型（文件/文件夹）
    uint16_t link;           // 连接数
    uint32_t block_point[6]; // 数据块指针
};

/**
 * @description:  初始化一个inode
 * @param {*}
 * @return {returns 1 on success, 0 otherwise.}
 */
int init_inode(struct inode *node, int size, int type, int link);

/**
 * @description:  修改一个inode
 * @param {*}
 * @return {returns 1 on success, 0 otherwise.}
 */
int write_inode(struct inode *node, int block);

/**
 * @description:  读取一个inode
 * @param {*}
 * @return {returns 1 on success, 0 otherwise.}
 */
int read_inode(struct inode *node, int index);

#endif