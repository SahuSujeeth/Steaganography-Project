#include <stdio.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int length = strlen(argv[2]);
    if (length >= 4 && strcmp(argv[2] + length - 4, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
        encInfo -> fptr_src_image = fopen(encInfo->src_image_fname,"rb");
        if(encInfo -> fptr_src_image == NULL)
        {
            return e_failure;
        }

        
    }
    else
    {
        printf("Error: Source image must have .bmp extension\n");
        return e_failure;
    }

    int length = strlen(argv[3]);
    if (length >= 4 && strcmp(argv[3] + length - 4, ".txt") == 0)
    {
        encInfo->secret_fname = argv[3];
        encInfo -> fptr_secret = fopen(encInfo->secret_fname,"rb");
        if(encInfo -> fptr_secret == NULL)
        {
            return e_failure;
        }

        
    }
    else
    {
        printf("Error: Secret file  must have .txt extension\n");
        return e_failure;
    }

    /* Select output image filename */
    if (argv[4] != NULL)
    {
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "default.bmp";
    }

    /* Validate output image extension */
    int output_length = strlen(encInfo->stego_image_fname);

    if (output_length < 4 ||
        strcmp(encInfo->stego_image_fname + output_length - 4, ".bmp") != 0)
    {
        printf("Error: Output image must have .bmp extension\n");
        return e_failure;
    }

    /* Open output image */
    encInfo->fptr_stego_image =
        fopen(encInfo->stego_image_fname, "wb");

    if (encInfo->fptr_stego_image == NULL)
    {
        printf("Error: Unable to open output image\n");
        return e_failure;
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
        -> check_capacity (endInfo) == e_failure
           print error msg, return e_failure
        -> copy_bmp_header(endInfo -> fptr_src_image, endInfo -> fptr_stego_image) == e_failure
           print error msg, return e_failure
        -> encode_magic_string(MAGIC_STRING,endInfo) == e_failre
            print error msg, return e_failure

        -> char *dot = strchr(argv[3], '.')
        -> strcpy(extn_secret_file, dot)
        -> encode_secret_file_extn_size(strlen(extn_secret_file),encInfo) == e_failure
           print error msg, return e_failure
        -> encode_secret_file_extn(extn_secret_file, encInfo) == e_failure
            print error msg, return e_failure
        -> encode_secret_file_size( encInfo -> size_secret_file, encInfo) == e_failure
            print error msg, return e_failure
        -> encode_secret_file_data(encInfo) == e_failure
            print error msg, return e_failure
        -> copy_remaining_img_data(fptr_src_image, fptr_stego_image) == efailure
            print error msg, return e_failure


    */
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
    int size = ftell(fptr);
    return size;


    /*
        -> fseek() to SEEK_END
        -> return ftell()
    */
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
     * rewind(), src and secret files -> fread and fwrite
     * Declare a buf of 54 bytes, char buff[54]
     * Read 54 bytes from drc image to buff
     * Write 54 bytes from buff to dest images
     */
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        -> Declare a buff of 8bytes

        => Run a loop for 2 times(or length of the magic string)
            ->Read 8bytes from src_image into buff
            ->encode_byte_to_lsb(magic_string[],buff);
            -> write 8 bytes of encoder data from buff to dest_image

        => return e_sucess
    */
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
    ==> Loop for 8 times
        * check the nth bit is set or not
            -> if set, set the LSB if image_buffer[]
            -> if clear, clear the LSB if image_buffer[]
    => return e_sucess
    */
}

Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{
    /*
        -> Declare a buff of 32 bytes

        -> REad 32 bytes of buf from src_image
        ->encode_size_to_lsb(size,buff)
        ->write buff to dest_image
    -> return e_sucess
    */
}

Status encode_size_to_lsb(uint data, char *image_buffer)
{
    /*
        ==> loop for 32 times
            *check the nth bit is set or not
                -> if set , set the LSB of image_buffer[]
                -> if clear, clear the LSB of image_buffer[]
        ==> return e_success
    */
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        -> declear a buff of 8 bytes
        => Run a loop for length of file_extn
            -> Read 8 bytes from src_image into buff
            -> encode_byte_to_lsb(file_extn[], buff);
            -> write 8 bytesif encided data from buff to dest_image

        ==> return e_success
    */
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        -> declare a buff of 32 bytes
        -> read 32 bytes of buff from src_image
        -> encode_size_to_lsb(size,buff)
        -> write buff to dest_image
    -> return e_success
    */
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        -> declare a buff of 8 bytes
        char buff[8] , data;
        => run a looop for Secret_file_size times
        -> read 8 byte from src_image into buff
        -> read a char from secret_file to data
        ->encode _byte_to_lsb(data, buff)
        -> write 8 bytes of encoded data from buff to dest_image
        => return e_success
    */
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        => Run a looop till EOF
         -> read a char from fptr_src
         -> write the char to fptr_dest
    */
}
