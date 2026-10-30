#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    EncodeInfo decInfo;

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
        printf("Decoding will be implemented after discussing with your mentor.\n");
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
