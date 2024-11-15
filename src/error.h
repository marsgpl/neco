#ifndef NECO_ERROR_H
#define NECO_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define LOG_ERROR_START "Error: "

#define error(msg, ...) { \
    fprintf(stderr, LOG_ERROR_START msg "\n" \
        __VA_OPT__(,) __VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

#define error_errno(msg, ...) { \
    fprintf(stderr, LOG_ERROR_START msg "; errno: %s (%d)\n" \
        __VA_OPT__(,) __VA_ARGS__, \
        strerror(errno), errno); \
    exit(EXIT_FAILURE); \
}

#endif
