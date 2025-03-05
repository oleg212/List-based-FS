#include "types/filesystem/filesystem.h"
#include <stdio.h>

int main() {
    filesystem_t fs;

    fs_init(&fs);
    fs_print_free_space(&fs);

    block_index root = fs_create_entry(&fs, 0, "root", E_DIR);
    block_index home = fs_create_entry(&fs, root, "dir", E_DIR);
    fs_create_entry(&fs, home, "readme.txt", E_FILE);
    fs_create_entry(&fs, home, "file2.txt", E_FILE);

    printf("\n");
    fs_list_dir(&fs, home);

    printf("\n");
    fs_print_free_space(&fs);

    return 0;
}
