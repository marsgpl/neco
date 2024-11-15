#include "file.h"

void file_write(const char *path, const char *content, size_t size) {
    if (path == NULL) {
        error("path is NULL");
    }

    if (strlen(path) == 0) {
        error("path is empty");
    }

    struct stat st;

    if (stat(path, &st) == 0) {
        error("path already exists: %s", path);
    }

    FILE *fp = fopen(path, "w");

    if (fp == NULL) {
        error_errno("fopen failed; path: %s", path);
    }

    if (size > 0) {
        size_t written = fwrite(content, size, 1, fp);

        if (written != 1) {
            error_errno("fwrite failed; size: %lu; path: %s", size, path);
        }
    }

    if (fclose(fp) != 0) {
        error_errno("fclose failed; path: %s", path);
    }
}

char *file_read(const char *path, size_t *size) {
    if (path == NULL) {
        error("path is NULL");
    }

    if (strlen(path) == 0) {
        error("path is empty");
    }

    struct stat st;

    if (stat(path, &st) != 0) {
        error_errno("stat failed; path: %s", path);
    }

    if (!S_ISREG(st.st_mode)) {
        error("not a regular file; path: %s", path);
    }

    *size = st.st_size;

    if (*size <= 0) {
        return NULL;
    }

    char *content = malloc(*size);

    if (content == NULL) {
        error_errno("malloc failed; size: %lu", *size);
    }

    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        error_errno("fopen failed; path: %s", path);
    }

    size_t read_n = fread(content, *size, 1, fp);

    if (read_n != 1) {
        error_errno("fread failed; size: %lu; path: %s", *size, path);
    }

    if (fclose(fp) != 0) {
        error_errno("fclose failed; path: %s", path);
    }

    return content;
}
