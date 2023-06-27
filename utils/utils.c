#include "utils.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Gets the size of a binary file pointed to by the provided
// pointer
size_t file_size(FILE* file_ptr) {
    fseek(file_ptr, 0, SEEK_END);
    int size = ftell(file_ptr);
    rewind(file_ptr);
    return size;
}
