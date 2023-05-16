#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "bmp.h"
#include "math.h"
//P' = pow(P / 255.0, gamma) * 255.0;
#define ARRAYSIZE 10000
void gammaCorrection(BMPHeader* header, const char* path, Pixel* pixel, double gamma) {
    for (int i = 0;i < header->imageSize / 3;i++) {
        double pixelR = (double)pixel[i].red / 255.0;
        double pixelG = (double)pixel[i].green / 255.0;
        double pixelB = (double)pixel[i].blue / 255.0;
        pixel[i].red = (unsigned char)(pow(pixelR, 1.0 / gamma) * 255.0);
        pixel[i].green = (unsigned char)(pow(pixelG, 1.0 / gamma) * 255.0);
        pixel[i].blue = (unsigned char)(pow(pixelB, 1.0 / gamma) * 255.0);
    }
    SaveBmp(header, path, pixel);
}
float SetValue(float a) {
    while (scanf("%f", &a) != 1 || getchar() != '\n') {
        printf("Error\n");
        rewind(stdin);
    }
    return a;
}
BMPHeader* readBMPHeader(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }

    BMPHeader* header = malloc(sizeof(BMPHeader));
    fread(header, sizeof(BMPHeader), 1, file);

    header->imageSize = header->width * header->height * (header->bitsPerPixel / 8);
    fclose(file);
    return header;
}
void BmpToNegative(BMPHeader* header, const char* path, Pixel* pixel) {
    for (int i = 0;i < header->imageSize / 3;i++) {
        pixel[i].red = 255 - pixel[i].red;
        pixel[i].blue = 255 - pixel[i].blue;
        pixel[i].green = 255 - pixel[i].green;
    }
    SaveBmp(header, path, pixel);
}
void BmpToWhitBlack(BMPHeader* header, const char* path, Pixel* pixel) {
    for (int i = 0;i < header->imageSize / 3;i++) {
        unsigned char sum = (pixel[i].red + pixel[i].blue + pixel[i].green) / 3;
        pixel[i].red = sum;
        pixel[i].blue = sum;
        pixel[i].green = sum;
    }
    SaveBmp(header, path, pixel);
}
void SaveBmp(BMPHeader* header, const char* path, Pixel* pixel) {
    FILE* file = fopen(path, "wb");
    fwrite(header, sizeof(BMPHeader), 1, file);
    for (int i = 0;i < header->imageSize / 3;i++) {
        fwrite(&pixel[i].red, sizeof(pixel->red), 1, file);
        fwrite(&pixel[i].green, sizeof(pixel->green), 1, file);
        fwrite(&pixel[i].blue, sizeof(pixel->blue), 1, file);
    }
    fclose(file);
}
void MediumFilter(BMPHeader* header, const char* path, Pixel* pixel, int size) {
    Pixel** pixels = malloc(sizeof(Pixel*) * header->height);
    for (int i = 0;i < header->height;i++) {
        pixels[i] = malloc(sizeof(Pixel) * header->width);
    }
    long int m = 0;
    for (int i = 0;i < header->height;i++) {
        for (int j = 0;j < header->width;j++) {
            pixels[i][j] = pixel[m];
            m++;
        }
    }
    Pixel** newPixel = createCopy(pixels, header->width, header->height);
    for (int i = 0;i < header->height;i++) {
        for (int j = 0;j < header->width;j++) {
            unsigned char r[ARRAYSIZE];
            unsigned char g[ARRAYSIZE];
            unsigned char b[ARRAYSIZE];
            int count = 0;
            if (j - size < 0) continue;
            if (j + size > header->width) continue;
            if (i - size < 0) continue;
            if (i + size > header->height) continue;
            for (int l = i - size;l < i + size;l++) {
                for (int z = j - size;z < j + size;z++) {
                    r[count] = pixels[l][z].red;
                    g[count] = pixels[l][z].green;
                    b[count] = pixels[l][z].blue;
                    count++;
                }
            }
            insertionSort(r, count);
            insertionSort(g, count);
            insertionSort(b, count);
            int index = count / 2;
            newPixel[i][j].red = r[index];
            newPixel[i][j].green = g[index];
            newPixel[i][j].blue = b[index];
        }
    }
    m = 0;
    for (int i = 0;i < header->height;i++) {
        for (int j = 0;j < header->width;j++) {
            pixel[m] = newPixel[i][j];
            m++;
        }
    }
    SaveBmp(header, path, pixel);
    for (int i = header->height - 1;i != -1;i--) {
        free(pixels[i]);
        free(newPixel[i]);
    }
    free(pixels);
    free(newPixel);
}
char isBMP(const char* path) {
    int i = strlen(path);
    i--;
    if (path[i] == 'p' && path[i - 1] == 'm' && path[i - 2] == 'b' && path[i - 3] && path[i - 4] == '.') return 1;

    return 0;
}
char is24BitBmp(BMPHeader* header) {
    if (header->bitsPerPixel == 24) return 1;

    return 0;
}
void insertionSort(unsigned char arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
Pixel** createCopy(Pixel** pixel, int col, int row) {
    Pixel** newPix = malloc(sizeof(Pixel*) * row);
    for (int i = 0;i < row;i++) {
        newPix[i] = malloc(sizeof(Pixel) * col);
    }
    for (int i = 0;i < row;i++) {
        for (int j = 0;j < col;j++) {
            newPix[i][j] = pixel[i][j];
        }
    }
    return newPix;
}
Pixel* createStructurePixel(BMPHeader* header, const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Не удалось открыть файл\n");
        return NULL;
    }
    fseek(file, 54, SEEK_SET);

    Pixel* pixel = malloc(sizeof(pixel) * header->imageSize / 3);
    unsigned char red;
    unsigned char blue;
    unsigned char green;
    int j = 0;
    for (int i = 0;i < header->imageSize;i += 3) {
        fread(&red, sizeof(unsigned char), 1, file);
        fread(&green, sizeof(unsigned char), 1, file);
        fread(&blue, sizeof(unsigned char), 1, file);
        pixel[j].red = red;
        pixel[j].green = green;
        pixel[j].blue = blue;
        j++;
    }
    fclose(file);
    return pixel;
}