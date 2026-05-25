#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

/* Structure to store decoding information */
typedef struct
{
    /* Stego image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output secret file info */
    char *output_fname;
    FILE *fptr_output;

    /* Secret file size */
    unsigned int secret_file_size;

} DecodeInfo;

Status open_decode_files(DecodeInfo *decInfo);

char decode_byte_from_lsb(char *image_buffer);

void skip_bmp_header(FILE *fptr_stego);

Status decode_magic_string(const char *expected_magic, DecodeInfo *decInfo);

unsigned int decode_int_from_lsb(FILE *fptr_stego);

Status decode_secret_file_data(DecodeInfo *decInfo);

void close_decode_files(DecodeInfo *decInfo);

#endif
