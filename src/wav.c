#include "wav.h"

static void check_id(
    const char *field_name,
    const char *actual,
    const char *expected
) {
    if (memcmp(actual, expected, 4) != 0) {
        error("unexpected wav header field %s value: " WAV_ID_ERROR_FMT
            "; expected: \"%s\" " WAV_ID_ERROR_FMT,
            field_name,
            actual[0], actual[1], actual[2], actual[3],
            expected,
            expected[0], expected[1], expected[2], expected[3]);
    }
}

static void check_uint(
    const char *field_name,
    size_t actual,
    size_t expected
) {
    if (actual != expected) {
        error("unexpected wav header field %s value: %lu; expected: %lu",
            field_name, actual, expected);
    }
}

void wav_fill_header(wav_header_t *header, size_t file_size) {
    memcpy(header->file_type_block_id, WAV_NEED_FILE_TYPE_BLOCK_ID, 4);
    memcpy(header->file_format_id, WAV_NEED_FILE_FORMAT_ID, 4);
    memcpy(header->format_block_id, WAV_NEED_FORMAT_BLOCK_ID, 4);
    memcpy(header->data_block_id, WAV_NEED_DATA_BLOCK_ID, 4);

    header->file_size = file_size - 8;
    header->block_size = WAV_NEED_BLOCK_SIZE;
    header->audio_format = WAV_NEED_AUDIO_FORMAT;
    header->channels_n = WAV_NEED_CHANNELS_N;
    header->frequency = WAV_NEED_FREQUENCY;
    header->bytes_per_sec = WAV_NEED_BYTES_PER_SEC;
    header->bytes_per_block = WAV_NEED_BYTES_PER_BLOCK;
    header->bits_per_sample = WAV_NEED_BITS_PER_SAMPLE;
    header->data_size = file_size - sizeof(wav_header_t);
}

void wav_validate_header(wav_header_t *header, size_t file_size) {
    check_id("file_type_block_id",
        header->file_type_block_id,
        WAV_NEED_FILE_TYPE_BLOCK_ID);

    check_id("file_format_id",
        header->file_format_id,
        WAV_NEED_FILE_FORMAT_ID);

    check_id("format_block_id",
        header->format_block_id,
        WAV_NEED_FORMAT_BLOCK_ID);

    check_id("data_block_id",
        header->data_block_id,
        WAV_NEED_DATA_BLOCK_ID);

    check_uint("file_size",
        header->file_size + 8,
        file_size);

    check_uint("block_size",
        header->block_size,
        WAV_NEED_BLOCK_SIZE);

    check_uint("audio_format",
        header->audio_format,
        WAV_NEED_AUDIO_FORMAT);

    check_uint("channels_n",
        header->channels_n,
        WAV_NEED_CHANNELS_N);

    check_uint("frequency",
        header->frequency,
        WAV_NEED_FREQUENCY);

    check_uint("bytes_per_sec",
        header->bytes_per_sec,
        WAV_NEED_BYTES_PER_SEC);

    check_uint("bytes_per_block",
        header->bytes_per_block,
        WAV_NEED_BYTES_PER_BLOCK);

    check_uint("bits_per_sample",
        header->bits_per_sample,
        WAV_NEED_BITS_PER_SAMPLE);

    check_uint("bytes_per_sec calculated",
        header->frequency * header->bytes_per_block,
        header->bytes_per_sec);

    check_uint("bytes_per_block calculated",
        header->channels_n * header->bits_per_sample / 8,
        header->bytes_per_block);

    check_uint("data_size",
        header->data_size,
        file_size - sizeof(wav_header_t));

    if (header->data_size % header->bytes_per_block != 0) {
        error("wav header: data_size is not divisible by bytes_per_block"
            "; data_size: %u; bytes_per_block: %u",
            header->data_size,
            header->bytes_per_block);
    }
}

void wav_print_header(wav_header_t *header) {
    printf("[Master RIFF chunk]\n");
    printf("  file_type_block_id: \"%.*s\"\n", 4, header->file_type_block_id);
    printf("  file_size: %u\n", header->file_size);
    printf("  file_format_id: \"%.*s\"\n", 4, header->file_format_id);
    printf("[Chunk describing the data format]\n");
    printf("  format_block_id: \"%.*s\"\n", 4, header->format_block_id);
    printf("  block_size: %u\n", header->block_size);
    printf("  audio_format: %u\n", header->audio_format);
    printf("  channels_n: %u\n", header->channels_n);
    printf("  frequency: %u\n", header->frequency);
    printf("  bytes_per_sec: %u\n", header->bytes_per_sec);
    printf("  bytes_per_block: %u\n", header->bytes_per_block);
    printf("  bits_per_sample: %u\n", header->bits_per_sample);
    printf("[Chunk containing the sampled data]\n");
    printf("  data_block_id: \"%.*s\"\n", 4, header->data_block_id);
    printf("  data_size: %u\n", header->data_size);
}
