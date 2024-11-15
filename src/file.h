#ifndef NECO_FILE_H
#define NECO_FILE_H

#include <sys/stat.h>
#include "error.h"

void file_write(const char *path, const char *content, size_t size);
char *file_read(const char *path, size_t *size);

#endif
