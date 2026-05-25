# Stegnography
Steganography Documentation

Name  : D Clinton
Mail  : bellclintooon@gmail.com

Introduction

Steganography is the technique of hiding secret information inside another medium such that the existence of the information is concealed. Unlike cryptography, which only encrypts the content, steganography hides the very presence of the data.

In this project, Least Significant Bit (LSB) Image Steganography is implemented using the C programming language, where a text file is hidden inside a 24-bit BMP image. The image looks visually unchanged, but it internally contains hidden data.


Objective of the Project

The main objectives of this project are:

To understand binary file formats (BMP)

To implement bit-level data manipulation

To design a complete encode–decode system

To hide and retrieve secret data securely from an image


Why BMP Image?

BMP images are ideal for steganography because:

They are uncompressed

Pixel data is stored in a simple format

Each pixel uses 3 bytes (RGB)

Modifying the least significant bit (LSB) does not visually affect the image


Concept Used: LSB (Least Significant Bit)

Each byte of pixel data has 8 bits:

Example byte: 10110110
LSB (last bit): 0

To hide data:

Only the LSB is modified

One image byte stores one bit of secret data

Changes are visually imperceptible


Overall Working of the Project

Encoding Flow:

Open source BMP image
Open secret text file
Create output stego image
Check if image has enough capacity
Copy BMP header (54 bytes)
Encode magic string (#*)
Encode secret file size (32 bits)
Encode secret file data (character by character)
Copy remaining image data

Decoding Flow:

Open stego image
Create output secret file
Skip BMP header
Decode and verify magic string
Decode secret file size
Decode secret file data
Write decoded data to output file


File Structure of the Project

main.c        → Program entry point
types.h       → Common data types and enums
common.h      → Project-wide constants
encode.h      → Encoder declarations
encode.c      → Encoder implementation
decode.h      → Decoder declarations
decode.c      → Decoder implementation
beautiful.bmp → Source image
secret.txt    → Input secret file
stego.bmp     → Output encoded image
output.txt    → Decoded secret file


Data Structures Used

EncodeInfo Structure:

Stores all information required during encoding.

typedef struct
{
    char *src_image_fname;
    FILE *fptr_src_image;

    char *secret_fname;
    FILE *fptr_secret;

    char *stego_image_fname;
    FILE *fptr_stego_image;

    unsigned int image_capacity;
    unsigned int secret_file_size;

} EncodeInfo;


DecodeInfo Structure:

Stores information required during decoding.

typedef struct
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *output_fname;
    FILE *fptr_output;

    unsigned int secret_file_size;

} DecodeInfo;



Important Functions – Encoding Side


open_encode_files()

Opens:
Source image (read-binary)
Secret file (read)
Stego image (write-binary)
Ensures safe file handling.


get_bmp_image_capacity()

Reads image width and height from BMP header and calculates:
capacity = width × height × 3


get_secret_file_size()

Finds the size of the secret file using fseek() and ftell().


check_capacity()

Verifies whether the image can store:
Magic string
Secret file size
Secret data
Prevents image corruption.


copy_bmp_header()

Copies the first 54 bytes of BMP header unchanged to the stego image.


encode_byte_to_lsb()

Encodes 1 character (8 bits) into 8 image bytes using LSB.



encode_magic_string()

Encodes a predefined string (#*) to mark the image as a valid stego image.



encode_uint32_to_lsb()

Encodes the secret file size (32-bit integer) into 32 image bytes.


encode_secret_file_data()

Encodes the entire secret file data into the image byte-by-byte.



copy_remaining_image_bytes()

Copies the remaining image data to complete the stego image.




Important Functions – Decoding Side


open_decode_files()

Opens:

Stego image (read-binary)
Output secret file (write)


skip_bmp_header()

Skips first 54 bytes of BMP header.



decode_byte_from_lsb()

Extracts a character by reading LSBs from 8 image bytes.



decode_magic_string()

Verifies whether the encoded image contains the correct magic string.



decode_uint32_from_lsb()

Decodes the 32-bit secret file size from the image.



decode_secret_file_data()

Decodes secret data and writes it into the output file.


Error Handling and Safety

Capacity check prevents overflow
Magic string validation avoids invalid decoding
Unified cleanup ensures no file descriptor leaks
All files are safely opened and closed


Advantages of the Project

Simple and effective steganography
No visual change in image
Bit-level control
Completely reversible
Written fully in C (low-level understanding)


Limitations

Works only with 24-bit BMP images
Not encrypted (can be enhanced)
Supports text files only (currently)



Applications

Secure data hiding
Digital watermarking
Confidential communication
Educational use (file formats & bit manipulation)



Conclusion

This project successfully demonstrates LSB-based image steganography by hiding and retrieving secret data from BMP images. The encoder and decoder are designed symmetrically and handle data safely at the bit level. Building the project from scratch helped in understanding low-level file handling, binary operations, and system-level programming concepts in C.
