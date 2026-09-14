#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"
#include "common.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
    /* Input Image Info */
    char *stego_image_fname;
    FILE *fptr_stego;

    /* Output Secret File Info */
    char *out_secret_txt;
    FILE *fptr_out;

    /* Decoded Information */
    int secret_file_extn_size;
    char secret_file_extn[MAX_FILE_SUFFIX];
    int secret_file_size;

    /* Image Data Buffer */
    char image_data[MAX_IMAGE_BUF_SIZE];

} DecodeInfo;

/* Function Prototypes */

/* Check and validate decoding arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open input and output files */
Status open_files_dec(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode one byte from image */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Perform complete decoding */
Status do_decoding(DecodeInfo *decInfo);

#endif