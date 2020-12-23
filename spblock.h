#ifndef SPBLOCK_H
#define SPBLOCK_H

#include <inttypes.h>

#define INODE_AMOUNT 1024           // inode共1024个
#define DATA_BLOCK_AMOUNT 4096           // 数据块共1024个

#define DATA_BLOCK_MAP 128               // 数据块位图占128个uint32_t
#define INODE_MAP 32                // inode位图占32个uint32_t

#define MAGICNUM 0000   //幻数

typedef struct super_block {
    int32_t magic_num;                      // 幻数
    int32_t free_data_block_count;               // 空闲数据块数
    int32_t free_inode_count;               // 空闲inode数
    int32_t dir_inode_count;                // 目录inode数
    uint32_t data_block_map[DATA_BLOCK_MAP];          // 数据块占用位图
    uint32_t inode_map[INODE_MAP];          // inode占用位图
};

struct super_block sp_block;

/**
 * @description: 初始化超级块
 * @return {returns 1 on success, 0 otherwise}
 */
int init_super_block();

/**
 * @description: 写超级块
 * @return {returns 1 on success, 0 otherwise}
 */
int write_super_block();

/**
 * @description: 读超级块
 * @return {returns 1 on success, 0 otherwise}
 */
int read_super_block();

/**
 * @description: 申请空闲数据块
 * @return {returns data_block_index on success, -1 otherwise.}
 */
int alloc_block();

/**
 * @description: 申请空闲inode结点
 * @return {returns inode_index on success, -1 otherwise.}
 */
int alloc_inode();

#endif