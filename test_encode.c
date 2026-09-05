#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    //check the operation_type wheather it is encoding or decoding
    if(check_operation_type(argv) == e_encode)
    {
        printf("Selected Encoding\n");
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Selected Decoding\n");
    }
    else
    {
        printf("Invalid option\n***************************Usage**********************\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding: ./a.out -d stego.bmp\n");
        printf("*******************************************************\n");
    }
    return 0;
}

//Function definiton
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }   
    else if(strcmp(argv[1],"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}