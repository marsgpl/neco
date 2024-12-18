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
    } else if (eq(cmd, "chart")) {
        chart(path);
    } else if (eq(cmd, "aggr")) {
        aggr(path);
    } else {
        error("unknown command: %s", cmd);
    }
}

static void create_empty_wav(const char *path) {
    size_t file_size = sizeof(wav_header_t);
    wav_header_t header;

    wav_fill_header(&header, file_size);
    write_file(path, (const char *)&header, file_size);
}

static void print_wav_header(const char *path) {
    size_t size;
    const char *content = read_file(path, &size);
    wav_header_t *header = (wav_header_t *)content;

    if (size < sizeof(wav_header_t)) {
        error("file is too small: %lu; min: %lu (wav header)",
            size, sizeof(wav_header_t));
    }

    wav_print_header(header);
    wav_validate_header(header, size);

    free((void *)content);
}

static void aggr(const char *path) {
    validate_dir_path(path);

    DIR *dir = opendir(path);

    if (dir == NULL) {
        error_errno("opendir failed; path: %s", path);
    }

    aggr_ctx ctx = {0};

    while (1) {
        errno = 0;
        struct dirent *entry = readdir(dir);

        if (entry == NULL) {
            if (errno != 0) {
                error_errno("readdir failed; path: %s", path);
            }

            break;
        }

        if (entry->d_type != DT_REG) {
            continue;
        }

        aggr_make_path(&ctx, path, entry->d_name);
        aggr_process_file(&ctx);
    }

    if (closedir(dir) != 0) {
        error_errno("closedir failed; path: %s", path);
    }

    aggr_print_stats(&ctx);
}

static void aggr_make_path(aggr_ctx *ctx, const char *root, const char *path) {
    size_t path_len = strlen(root) + 1 + strlen(path); // +1 for /

    if (path_len >= AGGR_PATH_MAX_LEN) { // >= because snprintf puts +1 nul
        error("file path is too long: %lu; max: %lu (can be increased)",
            path_len, AGGR_PATH_MAX_LEN);
    }

    snprintf(ctx->path, AGGR_PATH_MAX_LEN, "%s/%s", root, path);
}

static void aggr_process_file(aggr_ctx *ctx) {
    const char *path = ctx->path;
    char *content = ctx->content;
    struct stat st;

    if (stat(path, &st) != 0) {
        error_errno("stat failed; path: %s", path);
    }

    if (!S_ISREG(st.st_mode)) {
        return; // not a regular file
    }

    size_t size = st.st_size;

    if (size < sizeof(wav_header_t)) {
        error("file is too small: %lu; min: %lu (wav header)",
            size, sizeof(wav_header_t));
    } else if (size > AGGR_CONTENT_MAX_LEN) {
        error("file size is too big: %lu; max: %lu (can be increased)",
            size, AGGR_CONTENT_MAX_LEN);
    }

    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        error_errno("fopen failed; path: %s", path);
    }

    size_t read_n = fread(content, size, 1, fp);

    if (read_n != 1) {
        error_errno("fread failed; size: %lu; path: %s", size, path);
    }

    if (fclose(fp) != 0) {
        error_errno("fclose failed; path: %s", path);
    }

    wav_header_t *header = (wav_header_t *)content;
    size_t data_size = header->data_size;
    size_t bpb = header->bytes_per_block;
    const char *points = content + sizeof(wav_header_t);

    wav_validate_header(header, size);

    ctx->files_n++;
    ctx->total_size += size;
    ctx->total_points_n += data_size / bpb;

    int16_t prev_point = 0;

    for (size_t offset = 0; offset < data_size; offset += bpb) {
        int16_t point = (points[offset]) | (points[offset + 1] << 8);

        int uniq = (int)point + 32768;
        int delta = (int)point - (int)prev_point + 65536;

        ctx->uniqs[uniq] = 1;

        if (offset > 0) {
            ctx->deltas[delta] = 1;
        }

        prev_point = point;
    }
}

static void aggr_print_stats(aggr_ctx *ctx) {
    size_t n = 0;

    printf("files: %lu\n",
        ctx->files_n);

    printf("total size: %lu\n",
        ctx->total_size);

    printf("avg size: %lu\n",
        ctx->files_n ? ctx->total_size / ctx->files_n : 0);

    printf("total points: %lu\n",
        ctx->total_points_n);

    printf("avg points: %lu\n",
        ctx->files_n ? ctx->total_points_n / ctx->files_n : 0);

    for (int i = 0; i < 65536; ++i) {
        if (ctx->uniqs[i]) n++;
    }

    printf("uniq points: %lu\n", n);

    for (int i = 0; i < 131072; ++i) {
        if (ctx->deltas[i]) n++;
    }

    printf("uniq deltas: %lu\n", n);
}

static void chart(const char *path) {
    size_t size;
    const char *content = read_file(path, &size);
    wav_header_t *header = (wav_header_t *)content;
    size_t data_size = header->data_size;
    size_t bpb = header->bytes_per_block;
    const char *points = content + sizeof(wav_header_t);

    if (size < sizeof(wav_header_t)) {
        error("file is too small: %lu; min: %lu (wav header)",
            size, sizeof(wav_header_t));
    }

    wav_validate_header(header, size);

    // "M 0 0000.00 l" + " 000 -0000.00"
    size_t lines_size = 13 + 13 * (data_size / bpb - 1) + 1; // + nil
    size_t lines_filled = 0;
    char *lines = malloc(lines_size);

    if (lines == NULL) {
        error_errno("malloc failed; size: %lu", lines_size);
    }

    float prev_alias = 0;

    for (size_t offset = 0; offset < data_size; offset += bpb) {
        int16_t point = (points[offset]) | (points[offset + 1] << 8);
        float alias = ((float)point + 32768) / 64;

        if (offset == 4000) {
            break;
        }

        if (offset == 0) {
            lines_filled += snprintf(lines + lines_filled,
                lines_size - lines_filled,
                "M 0 %.2f l", alias);
        } else {
            lines_filled += snprintf(lines + lines_filled,
                lines_size - lines_filled,
                " 5 %.2f", alias - prev_alias);
        }

        prev_alias = alias;
    }

    printf("<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "style=\"background: #292c32\">\n"
        "<path d=\"%s\" fill=\"none\" stroke=\"#52b5d9\" stroke-width=\"1\"/>\n"
        "</svg>\n",
        lines);

    free(lines);
    free((void *)content);
}
