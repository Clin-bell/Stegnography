#include <stdio.h>
#include <string.h>
#include "types.h"
#include "encode.h"
#include "decode.h"

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }

    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

int main(int argc,  char *argv[])
{
    
    OperationType op_type;

    if(argc < 2)
    {
        printf("Usage :\n");
        printf("./steg -e <src_image.bmp> <secret.txt> <output.bmp>\n");
        return 1;
    }

    op_type = check_operation_type(argv);

    if(op_type == e_encode)
    {
        EncodeInfo encInfo;

        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        encInfo.stego_image_fname = argv[4];

        if (open_files(&encInfo) == e_failure)
        {
            printf("ERROR: File opening failed\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Files opened successfully\n");

        encInfo.image_capacity = get_image_capacity(encInfo.fptr_src_image);

        printf("Image capacity: %u bytes\n", encInfo.image_capacity);

        encInfo.secret_file_size = get_secret_file_size(encInfo.fptr_secret);

        printf("Secret file size: %u bytes\n", encInfo.secret_file_size);

        if (check_capacity(&encInfo) == e_failure)
        {
            printf("ERROR: Image does not have enough capacity\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Image has sufficient capacity to encode secret\n");

        if (copy_bmp_header(encInfo.fptr_src_image,encInfo.fptr_stego_image) == e_failure)
        {
            printf("ERROR: Failed to copy BMP header\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("BMP header copied successfully\n");

        if (encode_magic_string("#*", &encInfo) == e_failure)
        {
            printf("ERROR: Failed to encode magic string\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Magic string encoded successfully\n");

        if (encode_int_to_lsb(encInfo.secret_file_size, &encInfo) == e_failure)
        {
            printf("ERROR: Failed to encode secret file size\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Secret file size encoded successfully\n");

        if (encode_secret_file_data(&encInfo) == e_failure)
        {
            printf("ERROR: Failed to encode secret file data\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Secret file data encoded successfully\n");

        if (copy_remaining_image_data(encInfo.fptr_src_image,encInfo.fptr_stego_image) == e_failure)
        {
            printf("ERROR: Failed to copy remaining image data\n");
            close_encode_files(&encInfo);
            return 1;
        }

        printf("Remaining image data copied successfully\n");
        close_encode_files(&encInfo);

    }
    else if (op_type == e_decode)
    {
        DecodeInfo decInfo;

        decInfo.stego_image_fname = argv[2];
        decInfo.output_fname = argv[3];

        if (open_decode_files(&decInfo) == e_failure)
        {
            printf("ERROR: Failed to open decode files\n");
            close_decode_files(&decInfo);
            return 1;
        }

        printf("Decode files opened successfully\n");

        skip_bmp_header(decInfo.fptr_stego_image);

        if (decode_magic_string("#*", &decInfo) == e_failure)
        {
            printf("ERROR: Magic string not found. Invalid stego image\n");
            close_decode_files(&decInfo);
            return 1;
        }

        printf("Magic string decoded successfully\n");

        decInfo.secret_file_size = decode_int_from_lsb(decInfo.fptr_stego_image);

        printf("Decoded secret file size: %u bytes\n", decInfo.secret_file_size);

        if (decode_secret_file_data(&decInfo) == e_failure)
        {
            printf("ERROR: Failed to decode secret file data\n");
            close_decode_files(&decInfo);
            return 1;
        }

        printf("Secret file decoded successfully\n");
        close_decode_files(&decInfo);

    }

    else
    {
        printf("Unsupported Operation.\n");
        return 1;
    }

    return 0;
}