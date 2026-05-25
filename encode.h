#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>
#include "types.h"

/* Structure to store encoding information */
typedef struct
{
    // Source Image
    char *src_image_fname;
    FILE *fptr_src_image;

    // Secret File
    char *secret_fname;
    FILE *fptr_secret;

    // Stego Image
    char *stego_image_fname;
    FILE *fptr_stego_image;

    unsigned int image_capacity;
    unsigned int secret_file_size;

} EncodeInfo;

Status open_files(EncodeInfo *encInfo);

unsigned int get_image_capacity(FILE *fptr_image);

unsigned int get_secret_file_size(FILE *fptr_secret);

Status check_capacity(EncodeInfo *encInfo);

Status copy_bmp_header(FILE *fptr_src, FILE *fptr_stego);

void encode_byte_to_lsb(char data, char *image_buffer);

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

Status encode_int_to_lsb(unsigned int data, EncodeInfo *encInfo);

Status encode_secret_file_data(EncodeInfo *encInfo);

Status copy_remaining_image_data(FILE *fptr_src, FILE *fptr_stego);

void close_encode_files(EncodeInfo *encInfo);


#endif
