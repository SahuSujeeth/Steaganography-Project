#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
    if (argc != 4)
    {
        return e_failure;
    }
    // ARGV ORIGINAL IMAGE EXTEN
    char *ext = strrchr(argv[2], '.');
    if(ext == NULL ||  strcmp(ext,".bmp") != 0)// validate the bmp filename
    {
        return e_failure;
    }
    decInfo -> stego_image_fname = argv[2]; // Store the input filename
    if (strlen(argv[3]) >= sizeof(decInfo->output_fname))
    {
        return e_failure;
    }
    strcpy(decInfo->output_fname, argv[3]); // Store the output filename
    
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    // Step 1: Skip BMP header
    if (fseek(decInfo->fptr_stego_image, 54, SEEK_SET) != 0)
    {
        return e_failure;
    }

    // Step 2: Decode magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
        return e_failure;

    // Step 3: Decode extension size
    if (decode_secret_file_extn_size(decInfo) == e_failure)
        return e_failure;

    // Step 4: Decode extension
    if (decode_secret_file_extn(decInfo) == e_failure)
        return e_failure;

    // Step 5: Append extension and open output file
    if (append_extn(decInfo) == e_failure)
        return e_failure;

    // Step 6: Decode secret file size
    if (decode_secret_file_size(decInfo) == e_failure)
        return e_failure;

    // Step 7: Decode secret file data
    if (decode_secret_file_data(decInfo) == e_failure)
        return e_failure;

    return e_success;
}

// uint get_file_size(FILE *fptr)
// {
//     fseek(fptr, 0, SEEK_END);
//     uint size = ftell(fptr);
//     rewind(fptr);
//     return size;
// }

// Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
// {
//     char buffer[54];
//     rewind(fptr_src_image);

//     if(fread(buffer, 1, 54, fptr_src_image) != 54)
//     {
//         return e_failure;
//     }
//     if(fwrite(buffer, 1, 54, fptr_dest_image) != 54)
//     {
//         return e_failure;
//     }
//     return e_success;
// }

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
   char image_buffer[8];
   char decoded_magic[3];

   // Decode first character 
    if(fread(image_buffer, 1, 8, decInfo->fptr_stego_image) != 8)
    {
        return e_failure;
    }
    decoded_magic[0] = decode_byte_from_lsb(image_buffer);

    // decoding 2nd character
    if(fread(image_buffer, 1, 8, decInfo->fptr_stego_image) != 8)
    {
        return e_failure;
    }
    decoded_magic[1] = decode_byte_from_lsb(image_buffer);

    decoded_magic[2] = '\0';

    //comparing decoded magic string
    if(strcmp(decoded_magic, magic_string) == 0)
    {
        return e_success;
    }

    return e_failure;
    
}

char decode_byte_from_lsb(char *image_buffer)
{
    unsigned char data = 0;

    for(int i = 0; i < 8; i++)
    {
        data = data | ((image_buffer[i] & 1) << i);
    }

    return (char)data;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{

    char buffer[32];
    if(fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }
    
    decInfo -> size_secret_file_extn = decode_size_from_lsb(buffer);
    return e_success;
}

uint decode_size_from_lsb(char *image_buffer)
{
    uint data = 0;

    for (int i = 0; i < 32; i++)
    {
        data = data | ((uint)(image_buffer[i] & 1) << i);
    }

    return data;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];

    int len = decInfo->size_secret_file_extn;

    if (len >= sizeof(decInfo->extn_secret_file))
    {
        return e_failure;
    }

    for(int i = 0; i < len; i++)
    {
        if(fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        decInfo->extn_secret_file[i] = decode_byte_from_lsb(buffer);
    }

    decInfo->extn_secret_file[len] = '\0';

    return e_success;
}

Status append_extn(DecodeInfo *decInfo)
{
    strcat(decInfo->output_fname, decInfo->extn_secret_file);
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if (decInfo->fptr_output == NULL)
    {
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    decInfo->size_secret_file = decode_size_from_lsb(buffer);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char data;

    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        if (fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        data = decode_byte_from_lsb(buffer);

        if (fwrite(&data, 1, 1, decInfo->fptr_output) != 1)
        {
            return e_failure;
        }
    }

    return e_success;
}


