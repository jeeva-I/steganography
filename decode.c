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
    if (argv[2] != NULL && strrchr(argv[2], '.') != NULL &&
        strcmp(strrchr(argv[2], '.'), ".bmp") == 0)
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
        if (argv[3] != NULL && strrchr(argv[3], '.') != NULL &&
            strcmp(strrchr(argv[3], '.'), ".txt") == 0)
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
    decInfo -> fptr_stego = fopen(decInfo -> stego_image_fname, "rb");

    //Do error handling
    if(decInfo -> fptr_stego == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> stego_image_fname); //Displaying error message
        return e_failure; //return error
    }

    //Output text file opening in write mode
    decInfo -> fptr_out = fopen(decInfo -> out_secret_txt, "wb");

    //Do error handling
    if(decInfo -> fptr_out == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> out_secret_txt); //Displaying error message

        //Close input file if output file opening fails
        fclose(decInfo -> fptr_stego);

        return e_failure; //return error
    }

    return e_success; //If all files opened successfully then return success
}

/* Decode one byte from image */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    //Declaration
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

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    //Declaration
    char image_buffer[32];
    unsigned int decoded_size = 0;

    //Reading and validation of 32 bytes from input image
    if((fread(image_buffer, sizeof(char), 32, decInfo -> fptr_stego)) == 32)
    {
        //Loop to extract the LSB
        for(int i = 0; i < 32; i++)
        {
            decoded_size = decoded_size | ((unsigned int)(image_buffer[i] & 1) << (31 - i));//Extract,shift and merge the lsb
        }

        //Validate the decoded extension size
        if(decoded_size >= MAX_FILE_SUFFIX)
        {
            printf("Invalid secret file extension size\n");
            return e_failure;
        }

        decInfo -> secret_file_extn_size = decoded_size;
        //storing the size to the stucture member

        return e_success; //returns success
    }
    else
    {
        return e_failure; //returns error
    }
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    //Declaration
    int index = 0;

    //Loop to decode the extension characters
    while(index < decInfo->secret_file_extn_size)
    {
        //Reading 8 bytes from input image
        if(fread(decInfo->image_data, sizeof(char), 8,
                 decInfo->fptr_stego) == 8)
        {
            //Decode one character from the 8 image bytes
            decode_byte_from_lsb(&decInfo->secret_file_extn[index],
                                 decInfo->image_data); //Fucntion call()

            //Move to the next extension character
            index++;
        }
        else
        {
            return e_failure; //Returns error
        }
    }

    //Adding null character at the end of extension
    decInfo->secret_file_extn[index] = '\0';

    //Return success
    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    //Declaration
    char image_buffer[32];
    unsigned int decoded_size = 0;

    //Reading and validation of 32 bytes from input image
    if(fread(image_buffer, sizeof(char), 32,
             decInfo->fptr_stego) == 32)
    {
        //Loop to extract and merge the LSB
        for(int i = 0; i < 32; i++)
        {
            //Extract, shift and merge the LSB
            decoded_size = decoded_size |
                           ((unsigned int)(image_buffer[i] & 1) << (31 - i));
        }

        //Store the decoded size in the structure member
        decInfo->secret_file_size = decoded_size;

        //Return success
        return e_success;
    }
    else
    {
        return e_failure; //Returns error
    }
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //Declaration
    char decoded_data;

    //Loop to decode secret file data
    for(int i = 0; i < decInfo->secret_file_size; i++)
    {
        //Reading and validating 8 bytes from input image
        if(fread(decInfo->image_data, sizeof(char), 8,
                 decInfo->fptr_stego) == 8)
        {
            //Decode one character from the 8 image bytes
            decode_byte_from_lsb(&decoded_data,
                                 decInfo->image_data);

            //Writing and validating one decoded character to output file
            if(fwrite(&decoded_data, sizeof(char), 1,
                      decInfo->fptr_out) == 1)
            {
                //Character written successfully
            }
            else
            {
                return e_failure; //Returns error if writing fails
            }
        }
        else
        {
            return e_failure; //Returns error if reading fails
        }
    }

    //Return success after decoding and writing all data
    return e_success;
}

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    //Declaration
    int index = 0;
    int magic_length = strlen(magic_string);
    char temp[magic_length + 1];

    //Using loop to extract the magic string
    while(index < magic_length)
    {
        //Reading 8 bytes from input image file and validating
        if(fread(decInfo -> image_data, sizeof(char), 8,
                 decInfo -> fptr_stego) == 8)
        {
            //Calling function to decode the content of 8 bytes
            decode_byte_from_lsb(&temp[index],
                                 decInfo -> image_data);

            index++; //Incrementing the index
        }
        else
        {
            return e_failure; //returns error
        }
    }

    temp[index] = '\0'; //Adding null

    //Comparing the decoded magic string
    if(strcmp(temp, magic_string) != 0)
    {
        printf("Decoded magic string: %s\n", temp);
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
        if(fseek(decInfo -> fptr_stego, 54, SEEK_SET) != 0)
        {
            printf("Failed to skip BMP header\n");

            fclose(decInfo -> fptr_stego);
            fclose(decInfo -> fptr_out);

            return e_failure;
        }

        //validate wheater the magic_strings are decoded or not
        if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("Magic String decoded successfully\n"); //Displays the success message to the user

            //Validate the secret file extension size decoded or not
            if((decode_secret_file_extn_size(decInfo)) == e_success)
            {
                printf("Successfully decoded the secret file extension size\n"); //Displaying success message to the user

                //Validate the secret file extension is decoded or not
                if((decode_secret_file_extn(decInfo) == e_success))
                {
                    printf("Successfully decoded the secret file extension\n"); //Displaying success message to the user

                    //Validate the secret file size is decoded or not
                    if((decode_secret_file_size(decInfo)) == e_success)
                    {
                        printf("Successfully decoded the secret file size\n"); //Displaying success message to the user

                        //Validate the secret file data decoded or not
                        if((decode_secret_file_data(decInfo)) == e_success)
                        {
                            printf("Successfully decoded the secret file data\n"); //Displaying success message to the user

                            // Close the opened files
                            fclose(decInfo->fptr_stego);
                            fclose(decInfo->fptr_out);
                        }
                        else
                        {
                            printf("Failed to decode the secret file data\n"); //Displays error message

                            //Close the opened files
                            fclose(decInfo->fptr_stego);
                            fclose(decInfo->fptr_out);

                            return e_failure; // return error
                        }
                    }
                    else
                    {
                        printf("Failed to decode the secret file size\n"); //Displays error message

                        //Close the opened files
                        fclose(decInfo->fptr_stego);
                        fclose(decInfo->fptr_out);

                        return e_failure; // return error
                    }
                }
                else
                {
                    printf("Failed to decode the secret file extension\n"); //Displays error message

                    //Close the opened files
                    fclose(decInfo->fptr_stego);
                    fclose(decInfo->fptr_out);

                    return e_failure; //return error
                }
            }
            else
            {
                printf("Failed to decode the secret file extension size\n"); //Displays error to the user

                //Close the opened files
                fclose(decInfo->fptr_stego);
                fclose(decInfo->fptr_out);

                return e_failure; //return error
            }
        }
        else
        {
            printf("Failed to decode the magic string\n"); //Displays error to the user

            //Close the opened files
            fclose(decInfo->fptr_stego);
            fclose(decInfo->fptr_out);

            return e_failure; //returns error
        }
    }
    else
    {
        printf("Failed to open the files\n"); //Displays error to the user
        return e_failure; //return error
    }

    return e_success; // After all process completed it return sucess to the main
}
