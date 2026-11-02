#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include <string.h>
#include<stdio.h>

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    // stego image information
    FILE *fptr_stego_image;
    FILE *fptr_output;

    //output file information
    char *stego_image_fname;
    char output_fname[100];
    char extn_secret_file[10];

    //secret file information
    uint size_secret_file;
    uint size_secret_file_extn;

} DecodeInfo;


// /* Encoding function prototype */
/* Check operation type */
OperationType check_operation_type(char opt);


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo);



/* Decode one byte from image LSBs */
char decode_byte_from_lsb(char *image_buffer);

/* Decode integer size from image LSBs */
uint decode_size_from_lsb(char *image_buffer);

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Append extension to output filename */
Status append_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
