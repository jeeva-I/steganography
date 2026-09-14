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
