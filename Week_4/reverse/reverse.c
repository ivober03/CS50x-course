#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    WAVHEADER header;

    // read WAVHEADER bytes from the input file into the header variable
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    if (check_format(header) == 0)
    {
        printf("Not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // Creates an array to store each block of the WAV file we read.
    BYTE buffer[block_size];

    // Before reading the block uses fseek to know were to start
    if (fseek(input, block_size, SEEK_END))
    {
        return 1;
    }
    // Reads the blocks of the WAV file backwards and then write them on the output file until it reaches the header.
    while (ftell(input) - block_size > sizeof(header))
    {
        // Moves the pointer two blocks back in every iteration.
        if (fseek(input, - 2 * block_size, SEEK_CUR)){
            return 1;
        }
        fread(buffer, block_size, 1, input);
        fwrite(buffer, block_size, 1, output);
    }

    fclose(output);
    fclose(input);
}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A'
        && header.format[2] == 'V' && header.format[3] == 'E'
            && header.audioFormat == 1)
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    return header.numChannels * header.bitsPerSample / 8;
}