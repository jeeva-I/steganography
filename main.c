//Header Inclusion
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include"decode.h"
#include "types.h"


//Main program starts here()
int main(int argc,char *argv[])
{
    //check the arguments count condition
    if(argc <= 2)
    {
        printf("Enter the CLA correctly\n");
         printf("**************************Usage**********************\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding: ./a.out -d stego.bmp\n");
        printf("*******************************************************\n");
        return 1;
    }
    //check the operation_type wheather it is encoding or decoding
    if(check_operation_type(argv) == e_encode)
    {
        EncodeInfo encInfo; //declaring structure name to access members 
        printf("Selected Encoding\n"); //printing the encoding selected to the user
        //validate wheather the .bmp and .txt file is present or not
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and Validate encode arguments is Success\n"); //printing the arguments validation output 
            //check wheather the encoding completed or not 
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding Completed\n"); //printing the output to the user
            }
            else
            {
                printf("Failed to encode the data\n");
            }
        }
        else
        {
            printf("Failed to validate the input arguments\n");//printing the error message
        }
    }
    //check the decoding part
    else if(check_operation_type(argv) == e_decode)
    {
        DecodeInfo decInfo; //declaring structure name to access members 
        printf("Selected Decoding\n"); //printing the decoding selected to the user

        //validate wheather the .bmp inuput image file is present or not
       if(read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            printf("Read and Validate decode arguments is Success\n"); //printing the arguments validation output 
            //check wheather the decoding completed or not 
            // if(do_decoding(&decInfo) == e_success)
            // {
            //     printf("Decoding Completed\n"); //printing the output to the user
            // }
            // else
            // {
            //     printf("Failed to Decode the data\n");
            // }
        }
        else
        {
            printf("Failed to validate the input arguments\n");//printing the error message
        }
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
