/*
———————————————————————————————————————————————————————————————————————
Project: BMP Steganography Encoder/Decoder
Author : JEEVA I
Date   : 14 SEP 2026
Language: C

Description:
  This C program allows users to hide (encode) a secret text file within a BMP image file
  and to extract (decode) the hidden text file from a stego BMP image.
  It uses LSB (Least Significant Bit) steganography to embed and retrieve data.

Features:
  - Encode a .txt secret file into a .bmp image, producing a new stego image.
  - Decode and extract the hidden secret file from a stego .bmp image.
  - Validates file types and checks if the image has enough capacity for the secret.
  - Provides error messages for invalid usage or unsupported operations.

Sample User Inputs:
  ./a.out -e input.bmp secret.txt "#*"
    → Encodes secret.txt into input.bmp using magic string "#*", and saves result as output_image.bmp (default output file)

  ./a.out -e input.bmp secret.txt output.bmp "#*"
    → Encodes secret.txt into input.bmp using magic string "#*", and saves result as output.bmp

  ./a.out -d stego.bmp "#*"
    → Decodes the hidden file from stego.bmp using magic string "#*", and saves it as output_text.txt (default output name)

  ./a.out -d stego.bmp output "#*"
    → Decodes the hidden file from stego.bmp using magic string "#*", and saves it as output.txt

File Info:
  - Only supports 24-bit BMP images for encoding/decoding.
  - Secret file must be a .txt file.
  - Output file for decoding should not include an extension; it is restored automatically.

———————————————————————————————————————————————————————————————————————
*/




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
        if(do_decoding(&decInfo) == e_success)
            {
                printf("Decoding Completed\n"); //printing the output to the user
            }
        else
            {
                printf("Failed to Decode the data\n"); //Displaying the error message
            }
        }
        else
        {
            printf("Failed to validate the input arguments\n");//printing the error message
        }
    }
    else //Displaying error message and comment line prompt to the user
    {
        printf("Invalid option\n***************************Usage**********************\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding: ./a.out -d stego.bmp\n");
        printf("*******************************************************\n");
    }
    return 0;
}
