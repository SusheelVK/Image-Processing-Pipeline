#include "imgproc.h"
//#include "data400.h"
//#include "data600.h"
#include "datalighthouse.h"
//#include "dataflower.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

extern int BAYER_ARRAY[];
extern const int ROWS;
extern const int COLS;
extern const int ROWS_DEMOSAIC;
extern const int COLS_DEMOSAIC;
extern const int SIZE;
extern const int SIZE_DEMOSAIC;

struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
}__attribute__((packed, aligned(1)));

struct DIBHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t totalColors;
    uint32_t importantColors;
}__attribute__((packed, aligned(1)));

void init(int arr[], int SIZE){
    for (int i = 0; i < SIZE; i++){
        arr[i] = i % 256;
    }
}

void writeBMP(const char* filename, RGB demosaicArray[], int width, int height) {
    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return;
    }
    // Calculate row SIZE including padding
    int rowSize = width * sizeof(RGB);
    int paddingSize = (4 - (rowSize % 4)) % 4;
    int paddedRowSize = rowSize + paddingSize;

    // Calculate image SIZE including padding
    int imageSize = paddedRowSize * height; 
    // Write BMP header
    bmpHeader.signature[0] = 'B';
    bmpHeader.signature[1] = 'M';
    bmpHeader.fileSize = sizeof(BMPHeader) + sizeof(DIBHeader) + imageSize;
    bmpHeader.reserved1= 0;
    bmpHeader.reserved2= 0;
    bmpHeader.dataOffset = sizeof(BMPHeader) + sizeof(DIBHeader);

    file.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));

    // Write DIB header
    dibHeader.headerSize = sizeof(DIBHeader);
    dibHeader.width = width;
    dibHeader.height = height;
    dibHeader.planes = 1;
    dibHeader.bitsPerPixel = 24;
    dibHeader.compression = 0;
    dibHeader.xPixelsPerMeter = 0;
    dibHeader.yPixelsPerMeter = 0;
    dibHeader.imageSize = height * width * sizeof(RGB);
    dibHeader.importantColors = 0;
    dibHeader.totalColors = 0;

    file.write(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));

    // Write image data with padding
    unsigned char paddingBuffer[4] = { 0 };
    int col = COLS_DEMOSAIC;
    // Write image data
    for (int i = height - 1; i > -1; --i) {
        for (int j = 0; j < width; ++j) {
            RGB pixel = demosaicArray[i * col + j];
            std::swap(pixel.red, pixel.blue);
            file.write(reinterpret_cast<char*>(&pixel), sizeof(RGB));
        }
        if(paddingSize > 0){
            file.write(reinterpret_cast<char*>(paddingBuffer), paddingSize);
        }
    }
    file.close();
}


int main(){
    
    RGB image[SIZE_DEMOSAIC];
    YCoCg new_image[SIZE_DEMOSAIC];
    RGB old_image[SIZE_DEMOSAIC];

    const int diameter = 5;
    const float sigma_spatial = 40.0f;
    const float sigma_range = 40.0f;

    //init(bayerArray, SIZE);
    ahdDemosaic_RGGB(image);
    whiteBalance(image);
    conv_RGB_to_YCoCg(image, new_image);
    //autoContrast(new_image);
    //bilateralFilterRGB(image, diameter, sigma_spatial, sigma_range);
    //bilateralFilter(new_image, diameter, sigma_spatial, sigma_range);
    bilateralFilterYCoCg(new_image, diameter, sigma_spatial, sigma_range);
    //conv_YCoCg_to_RGB(old_image, new_image);
    
    for (int i = 0; i < SIZE_DEMOSAIC; i ++){
		old_image[i].red = new_image[i].Y;
		old_image[i].green = new_image[i].Y;
		old_image[i].blue = new_image[i].Y;
	}
	
    writeBMP("driver_output_lighthouse_filter_Y.bmp", old_image, COLS_DEMOSAIC ,ROWS_DEMOSAIC );
	/*
    std::ofstream fout("RGB_DemosaicBuilding400_3.txt");
    for (int i = 0; i < SIZE_DEMOSAIC; i++){
        fout << "{ ";
        fout << (int)image[i].red;
        fout << ", ";
        fout << (int)image[i].green;
        fout << ", ";
        fout << (int)image[i].blue;
        fout << " }, \n";
    }
	*/
    return 0;
}
