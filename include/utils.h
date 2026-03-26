// header files
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int width;
    int height;
    uint8_t *pixels;
} PPMImage;

// function declarations
void writeOutputPPMFile(char *filename, PPMImage* img);
PPMImage* readInputPPMFile(char *filename);
void skip_comments(FILE *fh);