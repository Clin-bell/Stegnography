#include <stdio.h>
#include "encode.h"

/* Open source image, secret file, and stego image */
Status open_files(EncodeInfo *encInfo)
{
    /* Open source image file */
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->src_image_fname);
        return e_failure;
    }

    /* Open secret file */
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->secret_fname);
        return e_failure;
    }

    /* Open stego image file */
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

/* Get image capacity for 24-bit BMP image */
unsigned int get_image_capacity(FILE *fptr_image)
{
    unsigned int width, height;

    /* Seek to width (offset 18) */
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);

    /* Read height (offset 22) */
    fread(&height, sizeof(int), 1, fptr_image);

    return width * height * 3;
}

/* Get size of secret file */
unsigned int get_secret_file_size(FILE *fptr_secret)
{
    unsigned int size;

    fseek(fptr_secret, 0, SEEK_END);
    size = ftell(fptr_secret);
    rewind(fptr_secret);

    return size;
}

/* Check if image can store secret data */
Status check_capacity(EncodeInfo *encInfo)
{
    unsigned int required_bits;

    required_bits = (2 * 8)                 // magic string
                  + 32                      // secret file size
                  + (encInfo->secret_file_size * 8); // secret data

    if (encInfo->image_capacity >= required_bits)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/* Copy BMP header from source to stego image */
Status copy_bmp_header(FILE *fptr_src, FILE *fptr_stego)
{
    char header[54];

    rewind(fptr_src);

    fread(header, sizeof(char), 54, fptr_src);
    fwrite(header, sizeof(char), 54, fptr_stego);

    return e_success;
}

/* Encode one byte into 8 image bytes using LSB */
void encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
}

/* Encode magic string into image */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char image_buffer[8];

    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        fread(image_buffer, sizeof(char), 8, encInfo->fptr_src_image);

        encode_byte_to_lsb(magic_string[i], image_buffer);

        fwrite(image_buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Encode integer value into image using LSB */
Status encode_int_to_lsb(unsigned int data, EncodeInfo *encInfo)
{
    char image_buffer[32];

    fread(image_buffer, sizeof(char), 32, encInfo->fptr_src_image);

    for (int i = 0; i < 32; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) | ((data >> (31 - i)) & 1);
    }

    fwrite(image_buffer, sizeof(char), 32, encInfo->fptr_stego_image);

    return e_success;
}

/* Encode secret file data into image */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char secret_char;
    char image_buffer[8];

    for (unsigned int i = 0; i < encInfo->secret_file_size; i++)
    {
        fread(&secret_char, sizeof(char), 1, encInfo->fptr_secret);
        fread(image_buffer, sizeof(char), 8, encInfo->fptr_src_image);

        encode_byte_to_lsb(secret_char, image_buffer);

        fwrite(image_buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Copy remaining image data after encoding */
Status copy_remaining_image_data(FILE *fptr_src, FILE *fptr_stego)
{
    char ch;

    while (fread(&ch, sizeof(char), 1, fptr_src) == 1)
    {
        fwrite(&ch, sizeof(char), 1, fptr_stego);
    }

    return e_success;
}

/* Close all encoder file pointers */
void close_encode_files(EncodeInfo *encInfo)
{
    if (encInfo->fptr_src_image)
        fclose(encInfo->fptr_src_image);

    if (encInfo->fptr_secret)
        fclose(encInfo->fptr_secret);

    if (encInfo->fptr_stego_image)
        fclose(encInfo->fptr_stego_image);
}

