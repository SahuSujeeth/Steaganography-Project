#include <stdio.h>
#include "encode.h"
#include "types.h"

int main()
{
    EncodeInfo encInfo;
    //(Call check_operation_type (arv[1][1]) == e_encode)
      /*
        read_and_validate == e_sucesss
        {
            // do_encoding(&endcInfo)
        }
        else
        {
            stop
        }
      */
    

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
