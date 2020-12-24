#include "filemanager.h"
#include "inode.h"
#include "spblock.h"
#include "filesystem.h"
#include "directory.h"
#include <stdio.h>
#include <string.h>

int add_file(struct inode *dir, char *name, int dir_inode_index, int *index)
{
    if (!add_inode(index, FILE))
    {
        printf("Failed to add inode.\n");
        return 0;
    }
    if (!insert_dir_item(dir, name, FILE, dir_inode_index, *index))
    {
        printf("Failed to insert dir_item.\n");
        return 0;
    }
    return 1;
}

int 