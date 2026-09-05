#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    //check the operation_type wheather it is encoding or decoding
    if(check_operation_type(argv) == e_encode)
    {
        EncodeInfo encInfo; //declaring structure name to access members 
        printf("Selected Encoding\n"); //printing the encoding selected to the user
        //validate wheather the .bmp and .txt file is present or not
        if(read_and_validate_encode_args(argv,&encInfo) == e_success)
        {
            printf("Read and Validate encode arguments is Success\n"); //printing the arguments validatiion output 
        }
        else
        {
            printf("Failed to validate the input arguments\n");//printing the error message
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Selected Decoding\n"); //printing the decoding selected to the user
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