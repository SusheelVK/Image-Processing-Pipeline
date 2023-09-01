/*
    White Balance Using fstream to read from and write to a BMP file
*/
#include <cstdio>
#include <fstream>
struct BMPFileHeader {
    char signature[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
}__attribute__((packed, aligned(1)));

struct BMPInfoHeader {
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


struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void whiteBalance(Pixel* pixel, int numPixels) {
    double totalRed = 0;
    double totalGreen = 0;
    double totalBlue = 0;

    // Calculate the total of each color channel
    for (int i = 0; i < numPixels; i ++) {
        totalRed += pixel[i].red;
        totalGreen += pixel[i].green;
        totalBlue += pixel[i].blue;
    }

    // Calculate the average color values
    double averageRed = (totalRed / numPixels);
    double averageGreen = (totalGreen / numPixels);
    double averageBlue = (totalBlue / numPixels);
    // Calculate scale to multiply with each pixel
    double scale = (averageRed + averageGreen + averageBlue) / 3.0;

    double RedScale = scale / averageRed;
    double BlueScale = scale / averageBlue;
    double GreenScale = scale / averageGreen;
    // Adjust each pixel based on the average color values
    for (int i = 0; i < numPixels; i ++) {
        if ((short int)pixel[i].red * RedScale > 255){
            pixel[i].red = 255;
        } 
        else{
            pixel[i].red = static_cast<unsigned char>(pixel[i].red * RedScale);
        }
        if ((short int)pixel[i].blue * BlueScale > 255){
            pixel[i].blue = 255;
        } 
        else{
            pixel[i].blue = static_cast<unsigned char>(pixel[i].blue * BlueScale);
        }
        if ((short int)pixel[i].green * GreenScale > 255){
            pixel[i].green = 255;
        } 
        else{
            pixel[i].green = static_cast<unsigned char>(pixel[i].green * GreenScale);
        }
    }
}

int calculatePadding(int width) {
    int padding = (4 - (width * 3) % 4) % 4;
    return padding;
}

int main() {
    
    // Open the BMP file
    std::ifstream file("mountain.bmp", std::ios::binary);

    if (!file) {
        printf("Failed to open the file.");
        return 1;
    }

    // Read the BMP file header
    BMPFileHeader fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

    if (file.fail()) {
        printf("Failed to read the BMP file header.");
        return 1;
    }

    if (fileHeader.signature[0] != 'B' || fileHeader.signature[1] != 'M') {
        printf("The file is not a valid BMP image.");
        return 1;
    }

    // Read the BMP info header
    BMPInfoHeader infoHeader;
    
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    if (file.fail()) {
        printf("Failed to read the BMP info header.");
        return 1;
    }

    if (infoHeader.bitsPerPixel != 24) {
        
        printf("Only 24-bit BMP images are supported.");
        return 1;
    }

    // Calculate the size of the image data
    int width = infoHeader.width;
    int height = infoHeader.height;
    int padding = calculatePadding(width);
    int dataSize = (width * 3 + padding) * height;
    int size = width * height;
    // Read the image data
    Pixel* image = new Pixel[dataSize];
    file.read(reinterpret_cast<char*>(image), dataSize);

    if (file.fail()) {
        printf("Failed to read the image data.");
        delete[] image;
        return 1;
    }
    file.close();
    
    // Apply white balance to the image
    whiteBalance(image, size);

    // Write the modified image to a new file
    std::ofstream outputFile("mountain_balanced.bmp", std::ios::binary);

    if (!outputFile) {
        printf("Failed to create the output file.");
        delete[] image;
        return 1;
    }

    // Write the BMP file header and info header to the output file
    outputFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    outputFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    // Write the modified image data to the output file
    outputFile.write(reinterpret_cast<char*>(image), dataSize);
    // Cleanup
    delete[] image;
    if (outputFile.fail()) {
        printf("Failed to write the output file.");
    }
    outputFile.close();
    

    return 0;
}
