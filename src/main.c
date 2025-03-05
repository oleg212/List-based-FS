#include "types/filesystem/filesystem.h"
#include <stdio.h>

int main() {
    filesystem_t fs;

    fs_init(&fs);
    fs_print_free_space(&fs);

    block_index root = fs_create_dir(&fs, 0, "root");
    block_index home = fs_create_dir(&fs, root, "dir");
    fs_create_file(&fs, home, "readme.txt");
    fs_create_file(&fs, home, "file2.txt");

    printf("\n");
    fs_list_dir(&fs, home);

    printf("\n");
    fs_print_free_space(&fs);

    return 0;
}
