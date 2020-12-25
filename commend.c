/*
 * @Description: 
 * @Author: Emise
 * @LastEditors: Emise
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"
#include "directory.h"
#include "process.h"
#include "inode.h"
#include "commend.h"
void mkdir(char *argv[], int argc)
{
    if (argc == 2)
    {
        char name[NAME_SIZE];
        memset(name, '\0', NAME_SIZE);

        char *argv_path = (char *)argv[1];

        int count = split(argv_path, name);

        struct inode root;
        int root_index = 0;
        read_inode(&root, root_index);

        int index = 0;
        for (int i = 0; i < count - 1; i++)
        {
            if (!find_dir_item(&root, directories[i], &index, Dir))
            {
                add_inode(&index, Dir);
                insert_dir_item(&root, directories[i], Dir, root_index, index);
            }
            read_inode(&root, index);
            root_index = index;
        }

        if (!find_dir_item(&root, name, &index, Dir))
        {
            add_inode(&index, Dir);
            insert_dir_item(&root, name, Dir, root_index, index);
            printf("make dir %s\n", argv[1]);
        }
        else
        {
            printf("%s already existed.\n", name);
        }
        return;
    }
    printf("Wrong arguments!\n");
    return;
}

void touch(char *argv[], int argc)
{
    if (argc == 2)
    {
        char name[NAME_SIZE];
        memset(name, '\0', NAME_SIZE);

        char *argv_path = (char *)argv[1];

        int count = split(argv_path, name);

        struct inode root;
        int root_index = 0;
        read_inode(&root, root_index);

        int index = 0;
        for (int i = 0; i < count - 1; i++)
        {
            if (!find_dir_item(&root, directories[i], &index, Dir))
            {
                add_inode(&index, Dir);
                insert_dir_item(&root, directories[i], Dir, root_index, index);
            }
            read_inode(&root, index);
            root_index = index;
        }

        if (root.link >= 48)
        {
            printf("Too many files!\n");
            return;
        }

        if (!find_dir_item(&root, name, &index, File))
        {
            add_inode(&index, File);
            insert_dir_item(&root, name, File, root_index, index);
            printf("make file %s\n", argv[1]);
        }
        else
        {
            printf("%s already existed.\n", name);
        }
        return;
    }
    printf("Wrong arguments!\n");
    return;
}

void ls(char *argv[], int argc)
{
    if (argc == 2)
    {
        char *argv_path = (char *)argv[1];
        char name[NAME_SIZE];
        memset(name, '\0', NAME_SIZE);
        int count = split(argv_path, name);

        struct inode root;
        int root_index = 0;
        read_inode(&root, root_index);
        int index = 0;
        for (int i = 0; i < count; i++)
        {
            if (!find_dir_item(&root, directories[i], &index, Dir))
            {
                add_inode(&index, Dir);
                insert_dir_item(&root, directories[i], Dir, root_index, index);
            }
            read_inode(&root, index);
            root_index = index;
        }
        print_dir_item(&root);
    }
    else if (argc > 2)
    {
        printf("Too many arguments!\n");
    }
    else
    {
        struct inode root;
        read_inode(&root, 0);
        print_dir_item(&root);
    }
    return;
}

void cp(char *argv[], int argc)
{
    if (argc == 3)
    {
        // 存放文件名称
        char name[NAME_SIZE];
        memset(name, '\0', NAME_SIZE);

        // 源文件路径
        char *argv_path1 = (char *)argv[1];

        // 源路径中的目录个数
        int count1 = split(argv_path1, name);

        // 读取根目录
        struct inode root1;
        int root1_index = 0;
        read_inode(&root1, root1_index);

        // 找到源文件上一级目录
        int index1 = 0;
        for (int i = 0; i < count1 - 1; i++)
        {
            if (!find_dir_item(&root1, directories[i], &index1, Dir))
            {
                printf("Path %s did not exist.\n", directories[i]);
            }
            read_inode(&root1, index1);
            root1_index = index1;
        }

        // 判断源文件是否存在
        if (!find_dir_item(&root1, name, &index1, File))
        {
            printf("File %s did not exist.\n", name);
            return;
        }
        read_inode(&root1, index1);

        memset(name, '\0', NAME_SIZE);

        // 目标路径
        char *argv_path2 = (char *)argv[2];

        // 目标路径中的目录个数
        int count2 = split(argv_path2, name);

        // 读取根目录
        struct inode root2;
        int root2_index = 0;
        read_inode(&root2, root2_index);

        // 找到目标文件上一级目录
        int index2 = 0;
        for (int i = 0; i < count2 - 1; i++)
        {
            if (!find_dir_item(&root2, directories[i], &index2, Dir))
            {
                add_inode(&index2, Dir);
                insert_dir_item(&root2, directories[i], Dir, root2_index, index2);
            }
            read_inode(&root2, index2);
            root2_index = index2;
        }

        // 判断目标文件是否存在
        if (!find_dir_item(&root2, name, &index2, File))
        {
            printf("%s did not exist.\nFormat a new one.\n", name);
            add_inode(&index2, File);
            insert_dir_item(&root2, name, File, root2_index, index2);
        }
        read_inode(&root2, root2_index);

        root2.link = root1.link;
        root2.size = root2.size;
        for (int i = 0; i < root1.link; i++)
        {
            root2.data_block_point[i] = root1.data_block_point[i];
        }
        return;
    }
    printf("Wrong arguments!\n");
    return;
}

void help()
{
    printf("command:\n");
    printf("ls <dir path>\n");
    printf("touch <file path>\n");
    printf("cp <dst path> <src path>\n");
    printf("mkdir <dir path>\n");
    printf("shutdown\n");
    printf("help\n");
}

void shutdown()
{
    if (close_system())
    {
        printf("Exit EXT2 file system\n");
    }
    exit(0);
}