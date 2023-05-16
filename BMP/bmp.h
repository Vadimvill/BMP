#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#pragma pack(push, 1)
typedef struct {
    char signature[2];  // Сигнатура файла (BM)
    int size;           // Размер файла в байтах
    int reserved;       // Зарезервированное значение
    int dataOffset;    // Смещение данных относительно начала файла
    int headerSize;    // Размер заголовка
    int width;          // Ширина изображения в пикселях
    int height;         // Высота изображения в пикселях
    short planes;       // Количество плоскостей
    short bitsPerPixel;  // Бит на пиксель
    int compression;    // Сжатие данных
    int imageSize;     // Размер данных изображения
    int xPixelsPerMeter;  // Разрешение по горизонтали
    int yPixelsPerMeter;  // Разрешение по вертикали
    int numColors;     // Количество цветов в палитре
    int importantColors;   // Количество важных цветов
} BMPHeader;
#pragma pack(pop)
#pragma pack(push, 1)
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}Pixel;
#pragma pack(pop)
float SetValue(float a);
char isBMP(const char* path);
char is24BitBmp(BMPHeader* header);
void gammaCorrection(BMPHeader* header, const char* path, Pixel* pixel, double gamma);
void insertionSort(unsigned char arr[], int n);
Pixel** createCopy(Pixel** pixel, int col, int row);
void MediumFilter(BMPHeader* header, const char* path, Pixel* pixel, int size);
BMPHeader* readBMPHeader(const char* path);
Pixel* createStructurePixel(BMPHeader* header, const char* path);
void BmpToNegative(BMPHeader* header, const char* path, Pixel* pixel);
void SaveBmp(BMPHeader* header, const char* path, Pixel* pixel);
void BmpToWhitBlack(BMPHeader* header, const char* path, Pixel* pixel);