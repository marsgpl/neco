#ifndef NECO_DEBUG_H
#define NECO_DEBUG_H

#include "file.h"
#include "wav.h"
#include "error.h"
#include "alias.h"

#define NEED_ARGC 3
#define USAGE "./debug cmd path"

#define eq(s1, s2) (strcmp(s1, s2) == 0)

static void create_empty_wav(const char *path);
static void print_wav_header(const char *path);
static void print_wav_points(const char *path);
static void print_wav_aliases(const char *path);
static void print_wav_deltas(const char *path);

#endif
