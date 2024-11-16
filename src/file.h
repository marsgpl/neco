#ifndef NECO_FILE_H
#define NECO_FILE_H

#include <sys/stat.h>
#include "error.h"

void write_file(const char *path, const char *content, size_t size);
char *read_file(const char *path, size_t *size);
off_t validate_file_path(const char *path);
void validate_dir_path(const char *path);

#endif
