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
    int length = strlen(argv);
    /*
        -> check the argv[2] must be having ".bmp" extn 
            if yes, edInfo -> srd_image-fname = argv[2]
                . open the file in read mode argv[2] 
                encInfo -> fptr_src_image = fopen()
                if the file not present , return e_failure
            if not, return e_failure


        -> Check the argv[3] is a valid file
            encInfo -> secret_fname = argv[3]
            is the file is present or not
             If present, encInfo -> fptr_secret = fopen()
             if not, return e_failure
             
        -> check the argv[4] is NULL 
        if not null, encInfo -> stego_image_fname = argv[4]
        if null, encInfo -> stego_image_fname = "default.bmp"

        Check the file exxtn is only of ".bmp".
        if not, return e_failure

        open the file in  write mode, and store the ile pointer to structure member.
    */
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
        -> check_capacity (endInfo) == e_failure
           print error msg, return e_failure
        -> copy_bmp_header(endInfo -> fptr_src_image, endInfo -> fptr_stego_image) ==
           print error msg, return e_failure
        -> encode_magic_string(MAGIC_STRING,endInfo) == e_failre
            print error msg, return e_failure

        -> char *dot = strchr(argv[3], '.')
        -> strcpy(extn_secret_file, dot)
        -> encode


    */
}
Status check_capacity(EncodeInfo *endInfo)
{
    /*
        get_image_size_for_bmp(encInfo -> fptr_src_image) > ((get_file_size(encInfo -> fptr_secret) + 14) * 8)
         ==> return e_sucess    
    */
}

uint get_file_size(FILE *fptr)
{

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
        ->write buf to dest_image
    -> retunr 
    */
}

Status