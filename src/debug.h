#ifndef NECO_DEBUG_H
#define NECO_DEBUG_H

#include <dirent.h>
#include "file.h"
#include "wav.h"
#include "error.h"
#include "alias.h"

#define NEED_ARGC 3
#define USAGE "./debug cmd path"

#define PRINT_MAX_BPP 7 // max bytes per point: "-32768,"
#define AGGR_PATH_MAX_LEN 2048UL
#define AGGR_CONTENT_MAX_LEN 200000UL

#define eq(s1, s2) (strcmp(s1, s2) == 0)

typedef struct {
    size_t files_n;
    size_t total_size;
    size_t total_points_n;
    char path[AGGR_PATH_MAX_LEN]; // tmp buf for file path
    char content[AGGR_CONTENT_MAX_LEN]; // tmp buf for file contents
    char uniqs[65536];
} aggr_ctx;

static void create_empty_wav(const char *path);
static void print_wav_header(const char *path);

static void aggr(const char *path);
static void aggr_make_path(aggr_ctx *ctx, const char *root, const char *path);
static void aggr_process_file(aggr_ctx *ctx);
static void aggr_print_stats(aggr_ctx *ctx);

#endif
