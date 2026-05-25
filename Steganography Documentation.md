Steganography Documentation



Name  : D Clinton

Batch : 25031C

Mail  : bellclintooon@gmail.com



#### Introduction



Steganography is the technique of hiding secret information inside another medium such that the existence of the information is concealed. Unlike cryptography, which only encrypts the content, steganography hides the very presence of the data.



In this project, Least Significant Bit (LSB) Image Steganography is implemented using the C programming language, where a text file is hidden inside a 24-bit BMP image. The image looks visually unchanged, but it internally contains hidden data.





#### Objective of the Project



The main objectives of this project are:



* To understand binary file formats (BMP)



* To implement bit-level data manipulation



* To design a complete encode–decode system



* To hide and retrieve secret data securely from an image





#### Why BMP Image?



BMP images are ideal for steganography because:



* They are uncompressed



* Pixel data is stored in a simple format



* Each pixel uses 3 bytes (RGB)



* Modifying the least significant bit (LSB) does not visually affect the image





#### Concept Used: LSB (Least Significant Bit)



Each byte of pixel data has 8 bits:



Example byte: 10110110

LSB (last bit): 0



To hide data:



* Only the LSB is modified



* One image byte stores one bit of secret data



* Changes are visually imperceptible





#### Overall Working of the Project



##### Encoding Flow:



1. Open source BMP image
2. Open secret text file
3. Create output stego image
4. Check if image has enough capacity
5. Copy BMP header (54 bytes)
6. Encode magic string (#\*)
7. Encode secret file size (32 bits)
8. Encode secret file data (character by character)
9. Copy remaining image data



##### Decoding Flow:



1. Open stego image
2. Create output secret file
3. Skip BMP header
4. Decode and verify magic string
5. Decode secret file size
6. Decode secret file data
7. Write decoded data to output file





#### File Structure of the Project



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



#### 

#### Data Structures Used 



##### EncodeInfo Structure:



###### Stores all information required during encoding.



typedef struct

{

&nbsp;   char \*src\_image\_fname;

&nbsp;   FILE \*fptr\_src\_image;



&nbsp;   char \*secret\_fname;

&nbsp;   FILE \*fptr\_secret;



&nbsp;   char \*stego\_image\_fname;

&nbsp;   FILE \*fptr\_stego\_image;



&nbsp;   unsigned int image\_capacity;

&nbsp;   unsigned int secret\_file\_size;



} EncodeInfo;



##### 

##### DecodeInfo Structure:



###### Stores information required during decoding.



typedef struct

{

&nbsp;   char \*stego\_image\_fname;

&nbsp;   FILE \*fptr\_stego\_image;



&nbsp;   char \*output\_fname;

&nbsp;   FILE \*fptr\_output;



&nbsp;   unsigned int secret\_file\_size;



} DecodeInfo;







#### Important Functions – Encoding Side





###### open\_encode\_files()



Opens:

* Source image (read-binary)
* Secret file (read)
* Stego image (write-binary)
* Ensures safe file handling.





###### get\_bmp\_image\_capacity()



Reads image width and height from BMP header and calculates:

capacity = width × height × 3





###### get\_secret\_file\_size()



Finds the size of the secret file using fseek() and ftell().





###### check\_capacity()



Verifies whether the image can store:

* Magic string
* Secret file size
* Secret data

Prevents image corruption.





###### copy\_bmp\_header()



Copies the first 54 bytes of BMP header unchanged to the stego image.





###### encode\_byte\_to\_lsb()



Encodes 1 character (8 bits) into 8 image bytes using LSB.







###### encode\_magic\_string()



Encodes a predefined string (#\*) to mark the image as a valid stego image.







###### encode\_uint32\_to\_lsb()



Encodes the secret file size (32-bit integer) into 32 image bytes.





###### encode\_secret\_file\_data()



Encodes the entire secret file data into the image byte-by-byte.







###### copy\_remaining\_image\_bytes()



Copies the remaining image data to complete the stego image.









##### Important Functions – Decoding Side





###### open\_decode\_files()



Opens:



* Stego image (read-binary)
* Output secret file (write)





###### skip\_bmp\_header()



Skips first 54 bytes of BMP header.







###### decode\_byte\_from\_lsb()



Extracts a character by reading LSBs from 8 image bytes.







###### decode\_magic\_string()



Verifies whether the encoded image contains the correct magic string.







###### decode\_uint32\_from\_lsb()



Decodes the 32-bit secret file size from the image.







###### decode\_secret\_file\_data()



Decodes secret data and writes it into the output file.





#### Error Handling and Safety



* Capacity check prevents overflow
* Magic string validation avoids invalid decoding
* Unified cleanup ensures no file descriptor leaks
* All files are safely opened and closed





#### Advantages of the Project



* Simple and effective steganography
* No visual change in image
* Bit-level control
* Completely reversible
* Written fully in C (low-level understanding)



#### 

#### Limitations



* Works only with 24-bit BMP images
* Not encrypted (can be enhanced)
* Supports text files only (currently)







#### Applications



* Secure data hiding
* Digital watermarking
* Confidential communication
* Educational use (file formats \& bit manipulation)







#### Conclusion



This project successfully demonstrates LSB-based image steganography by hiding and retrieving secret data from BMP images. The encoder and decoder are designed symmetrically and handle data safely at the bit level. Building the project from scratch helped in understanding low-level file handling, binary operations, and system-level programming concepts in C.











































