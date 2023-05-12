// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "util.h"

#include <stdlib.h>
#include <string.h>

char* append_path_and_filename(const char* path, const char* filename)
{
    const size_t path_size = strlen(path);
    const size_t filename_size = strlen(filename);
    const size_t size = path_size + filename_size;
    char *return_str = (char *) calloc(size + 1, sizeof(char));
    memcpy(return_str, path, path_size);
    memcpy(return_str + path_size, filename, filename_size);
    return_str[size] = 0;
    return return_str;
}
