// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // reserve 8-bit * HEADER_SIZE unsigned bytes in memory to read the header
    uint8_t header[HEADER_SIZE];

    // read HEADER_SIZE bytes from the input file into the header variable
    fread(header, sizeof(header), 1, input);

    // write the data readed before to the output file
    fwrite(header, sizeof(header), 1, output);

    // reserve 2 bytes to read each sample of the file
    int16_t buffer;

    // reads the samples, multiplies them by factor and writes the new multiplied sample to the output file
    while (fread(&buffer, sizeof(buffer), 1, input))
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(buffer), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
