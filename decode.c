/* Header Inclusion */
#include<stdio.h>
#include<string.h>
#include"decode.h"
#include"common.h"

/*  Function defintion  */

/* Check and validate decoding arguments */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    /* Validate argument count */
    if (argc < 3 || argc > 4)
    {
        printf("Invalid number of arguments\n"); //Printing the error message
        return e_failure; //returns error to the main
    }

    /* Validate the .bmp input image */
    if (argv[2] != NULL && strstr(argv[2], ".") != NULL &&
        strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        //Fetch and store the image file(.bmp) in the stucture member (stego_image_fname)
        decInfo -> stego_image_fname = argv[2];
    }
    else
    {
        printf("Enter a valid input .bmp file\n"); //Displaying error message 
        return e_failure; //return error
    }

    /* Check whether user provided output filename */
    if (argc == 4)
    {
        if (argv[3] != NULL && strstr(argv[3], ".") != NULL &&
            strcmp(strstr(argv[3], "."), ".txt") == 0)
        {
            //Fetch and store the output text file into the stucture member (out_secret_txt)
            decInfo->out_secret_txt = argv[3];
        }
        else
        {
            printf("Enter a valid output text file\n"); //Displays the error message
            return e_failure; //returing error
        }
    }
    else
    {
        /* Default output file */
        decInfo->out_secret_txt = "out_sec.txt";
    }

    return e_success; //After all the process returns success to the main 
}

/* Open input and output files */
Status open_files_dec(DecodeInfo *decInfo)
{
    //Input image file opening in read mode
    decInfo -> fptr_stego = fopen(decInfo -> stego_image_fname, "r");

    //Do error handling
    if(decInfo -> fptr_stego == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> stego_image_fname); //Displaying error message
    	return e_failure; //return error
    }

    //Output text file opening in write mode
    decInfo -> fptr_out = fopen(decInfo -> out_secret_txt, "w");

    //Do error handling 
    if(decInfo -> fptr_out == NULL)
    {
       perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> out_secret_txt); //Displaying error message
    	return e_failure; //return error 
    }

    return e_success; //If all files opened successfully then return success
}

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    //
}

/* Decode one byte from image */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    //Declaration
    unsigned char mask = 1 << 7;
    unsigned char decoded_data = 0;

    //Loop for extracting and merging the LSB
    for(int i = 0; i < 8; i++)
    {
        //Extract the LSB from the image byte
        decoded_data = decoded_data | ((image_buffer[i] & 1) << (7 - i));
    }

    //Store the decoded byte
    *data = decoded_data;

    //Return success
    return e_success;
}

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
        //Declaration
        int index = 0;
        char temp[strlen(magic_string) + 1];

        //Using loop to extract the magic string 
        while(magic_string[index] != '\0')
        {
            //Reading 8 bytes from input image file and validating
            if(fread(decInfo -> image_data, 8, sizeof(char), decInfo -> fptr_stego) == 8)
            {
                
                //Calling function to decode the content of 8 bytes
                decode_byte_from_lsb(&temp[index] , decInfo -> image_data);
                index++; //Incrementing the index
            }
            else
            {
                return e_failure; //returns error
            }
            
        }
        temp[index] = '\0'; //Adding null

        //comparing the string
        if(strcmp(temp,magic_string) != 0)
        {
            return e_failure; //returning error
        }

        return e_success; //returing sucess

}


/* Perform complete decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    //Validates wheather all the files are opened successfully
    if(open_files_dec(decInfo) == e_success)
    {
        printf("Opened all files Successfully\n"); //Displays the success message to the user
        printf("Started Decoding...\n");

        //Skips the .bmp header (54 bytes)
        fseek(decInfo -> fptr_stego, 54, SEEK_SET);

        //validate wheater the magic_strings are decoded or not
        if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("Magic String decoded successfully\n");
        }

    }

    return e_success; // After all process completed it return sucess to the main
}