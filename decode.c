#include <stdio.h>
#include "decode.h"

/* Open stego image and output secret file */
Status open_decode_files(DecodeInfo *decInfo)
{
    /* Open stego image file */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open stego image file %s\n",
                decInfo->stego_image_fname);
        return e_failure;
    }

    /* Open output secret file */
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to create output file %s\n",
                decInfo->output_fname);
        return e_failure;
    }

    return e_success;
}

/* Skip BMP header (54 bytes) */
void skip_bmp_header(FILE *fptr_stego)
{
    fseek(fptr_stego, 54, SEEK_SET);
}

/* Decode one byte from 8 image bytes using LSB */
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;

    for (int i = 0; i < 8; i++)
    {
        data = data << 1;
        data |= (image_buffer[i] & 1);
    }

    return data;
}

/* Decode magic string from stego image */
Status decode_magic_string(const char *expected_magic, DecodeInfo *decInfo)
{
    char image_buffer[8];
    char decoded_char;

    for (int i = 0; expected_magic[i] != '\0'; i++)
    {
        fread(image_buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        decoded_char = decode_byte_from_lsb(image_buffer);

        if (decoded_char != expected_magic[i])
        {
            return e_failure;
        }
    }

    return e_success;
}

/* Decode 32-bit integer from LSB */
unsigned int decode_int_from_lsb(FILE *fptr_stego)
{
    char image_buffer[32];
    unsigned int size = 0;

    fread(image_buffer, sizeof(char), 32, fptr_stego);

    for (int i = 0; i < 32; i++)
    {
        size = size << 1;
        size |= (image_buffer[i] & 1);
    }

    return size;
}

/* Decode secret file data and write to output file */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char decoded_char;

    for (unsigned int i = 0; i < decInfo->secret_file_size; i++)
    {
        fread(image_buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        decoded_char = decode_byte_from_lsb(image_buffer);

        fwrite(&decoded_char, sizeof(char), 1, decInfo->fptr_output);
    }

    return e_success;
}

/* Close all decoder file pointers */
void close_decode_files(DecodeInfo *decInfo)
{
    if (decInfo->fptr_stego_image)
        fclose(decInfo->fptr_stego_image);

    if (decInfo->fptr_output)
        fclose(decInfo->fptr_output);
}


