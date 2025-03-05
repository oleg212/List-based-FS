#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char* read_file(const char* filename, size_t* size) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[file_size] = '\0';
    *size = file_size;

    fclose(file);
    return buffer;
}

int write_file(const char* filename, const char* buffer, size_t size) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for writing: %s\n", filename, strerror(errno));
        return -1;
    }

    size_t bytes_written = fwrite(buffer, 1, size, file);
    if (bytes_written < size) {
        fprintf(stderr, "Error writing to file: %s\n", strerror(errno));
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
