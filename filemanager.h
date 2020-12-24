/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/**
 * @description: 新增一个文件 
 * @param dir_inode 需要新增文件的上一级目录的索引结点
 * @param name 文件名
 * @param dir_inode_index 
 * @param index 
 * @return returns 1 on success, 0 otherwise.
 */
int add_file(struct inode *dir_inode, char *name, int dir_inode_index, int *index);

#endif