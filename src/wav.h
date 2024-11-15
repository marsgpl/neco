#ifndef NECO_WAV_H
#define NECO_WAV_H

#include "error.h"

#define WAV_NEED_FILE_TYPE_BLOCK_ID "RIFF"
#define WAV_NEED_FILE_FORMAT_ID "WAVE"
#define WAV_NEED_FORMAT_BLOCK_ID "fmt "
#define WAV_NEED_BLOCK_SIZE 16
#define WAV_NEED_AUDIO_FORMAT 1
#define WAV_NEED_CHANNELS_N 1
#define WAV_NEED_FREQUENCY 19531
#define WAV_NEED_BYTES_PER_SEC 39062
#define WAV_NEED_BYTES_PER_BLOCK 2
#define WAV_NEED_BITS_PER_SAMPLE 16
#define WAV_NEED_DATA_BLOCK_ID "data"

#define WAV_ID_ERROR_FMT "{ 0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX }"

// https://en.wikipedia.org/wiki/WAV

typedef struct {
    // [Master RIFF chunk]
    char file_type_block_id[4]; // "RIFF" (0x52, 0x49, 0x46, 0x46)
    uint32_t file_size; // Overall file size minus 8 bytes
    char file_format_id[4]; // "WAVE" (0x57, 0x41, 0x56, 0x45)

    // [Chunk describing the data format]
    char format_block_id[4]; // "fmt " (0x66, 0x6D, 0x74, 0x20)
    uint32_t block_size; // Chunk size minus 8 bytes. 16 bytes here (0x10)
    uint16_t audio_format; // 1: PCM integer, 3: IEEE 754 float
    uint16_t channels_n;
    uint32_t frequency; // Sample rate (in hertz)
    uint32_t bytes_per_sec; // frequency * bytes_per_block
    uint16_t bytes_per_block; // channels_n * bits_per_sample / 8
    uint16_t bits_per_sample;

    // [Chunk containing the sampled data]
    char data_block_id[4]; // "data" (0x64, 0x61, 0x74, 0x61)
    uint32_t data_size;
} __attribute__((packed)) wav_header_t;

void wav_validate_header(wav_header_t *header, size_t file_size);
void wav_fill_header(wav_header_t *header, size_t file_size);
void wav_print_header(wav_header_t *header);

#endif
