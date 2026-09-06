#include <stdio.h>
#include "encode.h"
#include "types.h"
#include"common.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

//Logic for get the size of .txt file
uint get_file_size(FILE *fptr_sec)
{
    fseek(fptr_sec, 0, SEEK_END); //Moving the cursor to last position
    return ftell(fptr_sec); //returning the last position
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

//Check the user choice encoding or decoding
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

//check wheather the user provided .bmp and .txt and also file the date to the structure members
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    //Validate the .bmp file
    if(argv[2] != NULL && strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
        //Fetch and store the filename (.bmp) in the structure member (src_image_fname)
        encInfo -> src_image_fname = argv[2];
    }
    else
    {
        return e_failure; //return error
    }
    //Validate the .txt file
    if(argv[3] != NULL && strcmp(strstr(argv[3],"."),".txt") == 0)
    {
        //Fetch and store the filename(.txt) in the structure member (secret_fname)
        encInfo -> secret_fname = argv[3];
    }
    else
    {
        return e_failure; //return error
    }
    //optional output_image_file to create in two methods
    if(argv[4] != NULL) //validate the .bmp file
    {
        if(strcmp(strstr(argv[4],"."),".bmp") == 0)
        {
            //Fetch and store the filename(.bmp) in the structure member (stego_image_fname)
            encInfo -> stego_image_fname = argv[4];
        }
        else
        {
            printf("Enter the valid output image file --stego.bmp--\n");
            return e_failure; //return failure
        }
    }
    else
    {
        //create by default if the user did not create any file
        encInfo -> stego_image_fname = "stego.bmp";
    }
    return e_success; //after all the process return success

}

//For checking the image capacity to encode the secret data
Status check_capacity(EncodeInfo *encInfo)
{
    //get the size of image file and stores in the structure member
    encInfo ->  image_capacity = get_image_size_for_bmp(encInfo ->  fptr_src_image);
    //get the size of secret file and stores in the structure member
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    //validating the size and returning the result
    if(encInfo -> image_capacity > (54 +((2 + 4 + 4 + 4 + encInfo -> size_secret_file) * 8)))
    {
        return e_success; //returning success message
    }
    else
    {
        return e_failure; //returning failure message
    }
}

//For copy the header to destination file
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //declaring character array
    char header[54];

    //Moving file position to start
    fseek(fptr_src_image,0,SEEK_SET);

    //Read the 54 byte Header data from source file
    fread(header,54,sizeof(char),fptr_src_image);

    //Write 54 bytes header data to stego image
    fwrite(header,54,sizeof(char),fptr_dest_image);
    
    return e_success;
}
//Encoding the character in the lsb of RGB
Status encode_byte_to_lsb(char data,  char *image_buffer)
{
    //Declaration
    unsigned char mask = 1 << 7; //initial mask 

    //Loop for  clearing, extracting and merging the bit
    for(int i  = 0;i < 8;i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & mask) >> (7 - i));
        mask = mask >> 1; //changing masking for next bit encoding
    }
    return e_success; //returns success

}
//For encoding character 
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo)
{
    //call the function encode byte to lsb to encode the data one after the other
    for(int i = 0;i < size;i++)
    {
        //read 8 bytes of RGB data from source image 
        fread(encInfo -> image_data, 8, sizeof(char), encInfo -> fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo -> image_data); //callin the function 
        fwrite(encInfo -> image_data, 8, sizeof(char), encInfo -> fptr_stego_image); //Write 8 bytes of encoded data to the output image
    }
}
//for encoding the magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{   
    //Every character encoding will have to call this function
    encode_data_to_image(magic_string,strlen(magic_string),encInfo -> fptr_src_image, encInfo->fptr_stego_image,encInfo);
    return e_success; //Returns success 

}

//rest of the encoding function is called here
Status do_encoding(EncodeInfo *encInfo)
{
    //Validate wheather all files are opened successfully
    if(open_files(encInfo) == e_success)
    {
        printf("Opened all files Successfully\n"); //printing the success message to the user
        printf("Started Encoding...\n");

        //validate wheater the image has enough space to encode
        if(check_capacity(encInfo) == e_success)
        {
            printf("Image has enough Capacity to encode\n"); //printing the success message to the user
            
            //Copy the header from input to output bmp file
            if(copy_bmp_header(encInfo -> fptr_src_image, encInfo->fptr_stego_image ) == e_success) 
            {
                printf("Succesfully copied the header\n");

                //Encode the magic string into output bmp file using the data of input bmp file
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("Magic String encoded successfully\n"); //Displaying success message to the user
                }
                else
                {
                    printf("Failed to encode the magic string\n"); //wError message for the user
                    return e_failure;
                }

            }
            else
            {
                printf("Failed to copy the header\n"); //error message
                return e_failure;    
            }
        }
        else
        {
            printf("Do not have enough RCG data to encode the secret message\n"); //Printing the error message to the user
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open the files\n"); //printing the error message to the user
        return e_failure;
    }
    return e_success;
}