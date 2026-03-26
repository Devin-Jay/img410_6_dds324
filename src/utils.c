#include "utils.h"

void skip_comments(FILE *fh)
{
    int c;
    do
    {
        c = fgetc(fh);
        if (c == '#')
        {
            // skip entire comment line
            while ((c = fgetc(fh)) != '\n' && c != EOF);
        }
    } 
    while (c == '\n' || c == ' ' || c == '\t');

    ungetc(c, fh);
}

// reads input ppm file and returns PPMImage struct with data
PPMImage* readInputPPMFile(char *filename)
{
    FILE *fh = fopen(filename, "r");

    if (fh == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s for reading\n", filename);
        return NULL;
    }

    char magic[3];
    fscanf(fh, "%2s", magic);

    if (strcmp(magic, "P3") != 0)
    {
        fprintf(stderr, "Error: Unsupported PPM format %s. Only P3 format is supported.\n", magic);
        fclose(fh);
        return NULL;
    }

    skip_comments(fh);

    // read width, height, max color value
    PPMImage *img = (PPMImage*)malloc(sizeof(PPMImage));

    int maxval;
    fscanf(fh, "%d %d", &img->width, &img->height);
    fscanf(fh, "%d", &maxval);

    img->pixels = (uint8_t*)malloc(3 * img->width * img->height);

    if (img->pixels == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for PPMImage pixels\n");
        fclose(fh);
        free(img);
        return NULL;
    }

    int pixel_count = 3 * img->width * img->height;

    // loop through pixel data
    // Read RGB values, ensure no invalid numbers
    for (size_t i = 0; i < pixel_count; i++)
    {
        skip_comments(fh);
        int val;
        if (fscanf(fh, "%d", &val) != 1) {
            fprintf(stderr, "Error: Unexpected end of file or invalid pixel at index %zu\n", i);
            free(img->pixels);
            fclose(fh);
            free(img);
            return NULL;
        }
        if (val < 0 || val > maxval) {
            fprintf(stderr, "Error: Pixel value %d out of range 0-%d at index %zu\n", val, maxval, i);
            free(img->pixels);
            fclose(fh);
            free(img);
            return NULL;
        }
        img->pixels[i] = (uint8_t)val;
    }

    fclose(fh);

    return img;
}

// function to write PPMImage struct to output ppm file
void writeOutputPPMFile(char *filename, PPMImage* img)
{
    // implementation for writing PPM file
    printf("Writing PPM file: %s\n", filename);
    FILE *fh = fopen(filename, "w");

    // write header
    fprintf(fh, "P3\n");
    fprintf(fh, "%d %d\n", img->width, img->height);
    fprintf(fh, "255\n");

    int pixel_count = 3 * img->width * img->height;

    // write pixel data
    for (size_t i = 0; i < pixel_count; i++)
    {
        fprintf(fh, "%d ", img->pixels[i]);
        if ((i + 1) % (img->width * 3) == 0)
        {
            fprintf(fh, "\n");
        }
    }

    fclose(fh);
}

