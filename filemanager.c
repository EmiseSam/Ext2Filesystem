/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#include <stdio.h>
#include <string.h>
#include "inode.h"
#include "spblock.h"
#include "filesystem.h"
#include "directory.h"
#include "filemanager.h"

int add_file(struct inode *dir_inode, char *name, int dir_inode_index, int *index)
{
    if (!add_inode(index, File))
    {
        printf("Failed to add inode.\n");
        return 0;
    }
    if (!insert_dir_item(dir_inode, name, File, dir_inode_index, *index))
    {
        printf("Failed to insert dir_item.\n");
        return 0;
    }
    return 1;
}