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

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if (decInfo->fptr_output == NULL)
    {
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    // Check capacity
    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: Image does not have enough capacity\n");
        return e_failure;
    }

    // Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image,
                        encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Failed to copy BMP header\n");
        return e_failure;
    }

    // Encode magic string
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode magic string\n");
        return e_failure;
    }

    // Get secret file extension
    char *dot = strchr(encInfo->secret_fname, '.');

    if (dot == NULL)
    {
        printf("ERROR: Secret file extension not found\n");
        return e_failure;
    }

    strcpy(encInfo->extn_secret_file, dot);

    // Encode extension size
    if (encode_secret_file_extn_size(
            strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode extension size\n");
        return e_failure;
    }

    // Encode secret file extension
    if (encode_secret_file_extn(
            encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode secret file extension\n");
        return e_failure;
    }

    // Encode secret file size
    if (encode_secret_file_size(
            encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode secret file size\n");
        return e_failure;
    }

    // Encode secret file data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode secret file data\n");
        return e_failure;
    }

    // Copy remaining image data
    if (copy_remaining_img_data(
            encInfo->fptr_src_image,
            encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Failed to copy remaining image data\n");
        return e_failure;
    }

    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    
        encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
        if(get_image_size_for_bmp(encInfo -> fptr_src_image) > ((encInfo -> size_secret_file) + 14) * 8)
        {
            return e_success;
        } 
        return e_failure;
    
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);

    if(fread(buffer, 1, 54, fptr_src_image) != 54)
    {
        return e_failure;
    }
    if(fwrite(buffer, 1, 54, fptr_dest_image) != 54)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    for(int i = 0; i < strlen(magic_string); i++)
    {
        if (fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }  
        encode_byte_to_lsb(magic_string[i], buffer);
        if(fwrite(buffer,1,8,encInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
    
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
        if(data  & (1 << i))
        {
            image_buffer[i] = image_buffer[i] | 1;
        }
        else
        {
            image_buffer[i] = image_buffer[i] & (~1);
        }
    }
    return e_success;

}

Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{

    char buffer[32];
    if(fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
    {
        return e_failure;
    }
    encode_size_to_lsb(size,buffer);
    if(fwrite(buffer,1,32,encInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_size_to_lsb(uint data, char *image_buffer)
{
    for(int i = 0; i < 32; i++)
    {
        if(data & (1u << i))
        {
            image_buffer[i] = image_buffer[i] | 1;
        }
        else
        {
            image_buffer[i] = image_buffer[i] & (~1);
        }
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    int len = strlen(file_extn);
    for(int i = 0; i < len; i++)
    {
        if(fread(buffer,1,8,encInfo -> fptr_src_image) != 8)
        {
            return e_failure;
        }
        encode_byte_to_lsb(file_extn[i],buffer);
        if(fwrite(buffer,1,8,encInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    if(fread(buffer,1,32,encInfo -> fptr_src_image) != 32)
    {
        return e_failure;
    }
    encode_size_to_lsb(file_size,buffer);
    if(fwrite(buffer,1,32,encInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8], data;
    for(int i = 0; i < encInfo -> size_secret_file; i++)
    {
        if(fread(buffer,1,8,encInfo ->fptr_src_image) != 8)
        {
            return e_failure;
        }
        if(fread(&data,1,1,encInfo -> fptr_secret) != 1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(data,buffer);
        if(fwrite(buffer,1,8,encInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while((ch = fgetc(fptr_src)) != EOF)
    {
        if(fputc(ch,fptr_dest) == EOF)
        {
            return e_failure;
        }
    }
    return e_success;
}
