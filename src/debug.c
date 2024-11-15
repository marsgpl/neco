#include "debug.h"

int main(int argc, const char **argv) {
    if (argc != NEED_ARGC) {
        error("Usage: %s", USAGE);
    }

    const char *cmd = argv[1];
    const char *path = argv[2];

    if (eq(cmd, "empty")) {
        create_empty_wav(path);
    } else if (eq(cmd, "header")) {
        print_wav_header(path);
    } else if (eq(cmd, "points")) {
        print_wav_points(path);
    } else if (eq(cmd, "aliases")) {
        print_wav_aliases(path);
    } else if (eq(cmd, "aliases")) {
        print_wav_deltas(path);
    } else {
        error("unknown command: %s", cmd);
    }
}

static void create_empty_wav(const char *path) {
    size_t file_size = sizeof(wav_header_t);
    wav_header_t header;

    wav_fill_header(&header, file_size);
    file_write(path, (const char *)&header, file_size);
}

static void print_wav_header(const char *path) {
    size_t size;
    const char *content = file_read(path, &size);

    if (size < sizeof(wav_header_t)) {
        error("file is too small: %lu; min: %lu", size, sizeof(wav_header_t));
    }

    wav_print_header((wav_header_t *)content);
    free((void *)content);
}

static void print_wav_points(const char *path) {
    (void)path;
}

static void print_wav_aliases(const char *path) {
    (void)path;
}

static void print_wav_deltas(const char *path) {
    (void)path;
}
