#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "inode.h"
#include "spblock.h"

struct inode inode[INODE_AMOUNT];

/**
 * @description:  初始化文件系统
 * @param {*}
 * @return {returns 1 on success, 0 otherwise}
 */
int init_system();

/**
 * @description:  打开文件系统
 * @param {*}
 * @return {returns 1 on success, 0 otherwise}
 */
int open_system();

/**
 * @description:  关闭文件系统
 * @param {*}
 * @return {returns 1 on success, 0 otherwise}
 */
int close_system();

#endif