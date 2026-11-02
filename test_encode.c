#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo = {0};

    if (argc < 2)
    {
        printf("Usage: ./lsb_steg -e <image.bmp> <secret.txt> [output.bmp]\n");
        return 1;
    }

    OperationType op_type = check_operation_type(argv[1][1]);

    if (op_type == e_encode)
    {
        if (read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding successful!\n");
            }
            else
            {
                printf("Encoding failed!\n");
                return 1;
            }
        }
        else
        {
            printf("Invalid encoding arguments!\n");
            return 1;
        }
    }
    else if (op_type == e_decode)
    {
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding completed successfully\n");
            }
            else
            {
                printf("Decoding failed\n");
            }
        }
        else
        {
            printf("Invalid decoding arguments\n");
        }

        if (decInfo.fptr_stego_image != NULL)
        {
            fclose(decInfo.fptr_stego_image);
        }

        if (decInfo.fptr_output != NULL)
        {
            fclose(decInfo.fptr_output);
        }
    }
    else
    {
        printf("Unsupported operation!\n");
        return 1;
    }

    return 0;
}

OperationType check_operation_type(char opt)
{
    if(opt == 'e')
    {
        return e_encode;
    }
    else if (opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
