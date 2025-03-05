#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#include "../filesystem/filesystem.h"
#include "../helpers/file_io.h"

typedef struct image_handler
{
    FILE* file;
    char* filename;
    size_t size;
};

// int load_image()

#endif