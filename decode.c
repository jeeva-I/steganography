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
        printf("Invalid number of arguments\n");
        return e_failure;
    }

    /* Validate the .bmp input image */
    if (argv[2] != NULL && strstr(argv[2], ".") != NULL &&
        strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Enter a valid input .bmp file\n");
        return e_failure;
    }

    /* Check whether user provided output filename */
    if (argc == 4)
    {
        if (argv[3] != NULL && strstr(argv[3], ".") != NULL &&
            strcmp(strstr(argv[3], "."), ".txt") == 0)
        {
            decInfo->out_secret_txt = argv[3];
        }
        else
        {
            printf("Enter a valid output text file\n");
            return e_failure;
        }
    }
    else
    {
        /* Default output file */
        decInfo->out_secret_txt = "out_sec.txt";
    }

    return e_success;
}
