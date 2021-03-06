#include <stdio.h>

int main(int argc, char* argv[]) {

    // read in input file
    FILE *inFile;
    inFile = fopen(argv[1], "r");

    // create output file at specified path
    FILE *outFile;
    outFile = fopen(argv[2], "w");

    // store message in buffer
    char *secretMessage = argv[3];

    // bitmap parsing
    typedef struct header {
        unsigned int offset;
        int height;
        unsigned int width;
    }BITMAPFILEHEADER;

    struct header fileHeader;

    fseek(inFile, 10, SEEK_SET);
    fread(&fileHeader.offset, 4, 1, inFile);
    printf("Offset is %i\n", fileHeader.offset);

    fseek(inFile, 4, SEEK_CUR);
    fread(&fileHeader.width, 4, 1, inFile);
    printf("Width is %i\n", fileHeader.width);
    fread(&fileHeader.height, 4, 1, inFile);
    if (fileHeader.height < 0) {
        fileHeader.height *= -1;
    }
    printf("Height is %i\n", fileHeader.height);

    int numPixels = fileHeader.height * fileHeader.width;

    // copy headers (everything before offset) to outFile
    fseek(inFile, 0, SEEK_SET);
    char headerBytes[fileHeader.offset];
    fread(&headerBytes, fileHeader.offset, 1, inFile);
    fwrite(&headerBytes, fileHeader.offset, 1, outFile);

    // encode message in RGB bytes
    fseek(inFile, 54, SEEK_SET);

    int messageIndex = 0;
    int pixelByteCounter = 0;
    unsigned char currentByte;

    while(secretMessage[messageIndex] != '\0'){
        printf("Current byte: %c\n", secretMessage[messageIndex]);

        // print char binary:
        char charBits[8];
        for (int i=0; i < 8; ++i) {
            charBits[i] = secretMessage[messageIndex] >> i & 1;
        }
        // end of printing char binary

        for (int currentBit = 0; currentBit < 8; ) {
            fread(&currentByte, 1, 1, inFile);
            printf("Current pixel byte value: %i\n", currentByte);

            if (pixelByteCounter % 4 != 3) {
                // modify LSB
                unsigned char outputByte;
                outputByte = currentByte >> 1;
                outputByte = outputByte << 1;
                // modify currentByte with LSB mask
                outputByte = outputByte | charBits[currentBit];
                printf(" New encoded byte: %i\n", outputByte);
                currentByte = outputByte;
                currentBit++;
            }
            // output currentByte to outFile
            fwrite(&currentByte, 1, 1, outFile);
            pixelByteCounter++;
        }
        ++messageIndex;
    }
    printf("\n");

    // encode null byte after message has been encoded
    unsigned char nextByte;

    printf("encoding null byte!\n");
    for (int currentBit = 0; currentBit < 8; ) {
        fread(&nextByte, 1, 1, inFile);
        printf("Current pixel byte value: %i\n", nextByte);

        if (pixelByteCounter % 4 != 3) {
            unsigned char outputByte;
            outputByte = nextByte >> 1;
            outputByte = outputByte << 1;
            printf(" Null encoded byte: %i\n", outputByte);
            currentBit++;
        }

        // output currentByte to outFile
        fwrite(&currentByte, 1, 1, outFile);
        pixelByteCounter++;
    }

    // copy unmodified pixels to outFile
    char remainingPixelBytes[(numPixels*4) - pixelByteCounter];
    fread(&remainingPixelBytes, (numPixels*4) - pixelByteCounter, 1, inFile);
    fwrite(&remainingPixelBytes, (numPixels*4) - pixelByteCounter, 1, outFile);

    // close files
    fclose(inFile);
    fclose(outFile);
    printf("Done!\n");

    return 0;
}
