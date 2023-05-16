#include <stdio.h>
#include "stdlib.h"
#include "bmp.h"
#define PATHSIZE 2048
void setPath(char* path) {
    printf("Set outPath\n");
    scanf("%s", path);
}
int main() {
    printf("Set Path to File\n");
    char* path = malloc(sizeof(char) * PATHSIZE);
    scanf("%s", path);
    BMPHeader* header = readBMPHeader(path);
    if (header == NULL) {
        printf("error");
        free(path);
        return 1;
    }
    if (!is24BitBmp(header)) return 0;
    int bool = 1;
    Pixel* pixel = createStructurePixel(header, path);
    rewind(stdin);
    while (bool) {
        Pixel* pixel = createStructurePixel(header, path);
        printf("Set fucntion\n");
        printf("1 - negativ, 2 - White and Black, 3 - Median filter, 4 - Gamma cor, 5 - exit\n");

        int func;
        func = (int)SetValue(0);
        char* outPath = malloc(sizeof(char) * PATHSIZE);

        printf("Wait...\n");
        double gamma = 0;
        switch (func) {
        case 1:
            setPath(outPath);
            BmpToNegative(header, outPath, pixel);
            break;
        case 2:
            setPath(outPath);
            BmpToWhitBlack(header, outPath, pixel);
            break;
        case 3:
            setPath(outPath);
            printf("Set size from 0 to 100\n");
            func = (int)SetValue(0);
            if (func > 100 || func < 0) {
                printf("Error\n");
                continue;
            }
            MediumFilter(header, outPath, pixel, func);
            break;
        case 4:
            setPath(outPath);
            printf("Set gamma\n");
            gamma = SetValue(gamma);
            gammaCorrection(header, outPath, pixel, gamma);
            break;
        case 5:
            bool = 0;
            break;
        default:
            printf("error\n");
            free(outPath);
            break;
        }
        printf("Ready\n");
        rewind(stdin);
        free(pixel);
    }
    free(header);
}
