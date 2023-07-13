#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <encoding.h>

struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

const int rows = 1920;
const int cols = 1080;

RGB image[rows * cols];

void whiteBalance() {
    int totalRed = 0;
    int totalGreen = 0;
    int totalBlue = 0;
    // Calculate the total of each color channel
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j){
            totalRed += image[i * cols + j].red;
            totalGreen += image[i * cols + j].green;
            totalBlue += image[i * cols + j].blue;
        }
    }
    float scale = (totalRed + totalGreen + totalBlue) / 3.0;

    float RedScale = scale / totalRed;
    float BlueScale = scale / totalBlue;
    float GreenScale = scale / totalGreen;
 
    // Adjust each pixel based on the average color values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j){
            int redVal = image[i * cols + j].red * RedScale;
            int blueVal = image[i * cols + j].blue * BlueScale;
            int greenVal = image[i * cols + j].green * GreenScale;
            if (redVal > 255){
                image[i * cols + j].red = 255;
            } 
            else{
                image[i * cols + j].red = redVal;
            }
            if (blueVal > 255){
                image[i * cols + j].blue = 255;
            } 
            else{
                image[i * cols + j].blue = blueVal;
            }
            if (greenVal > 255){
                image[i * cols + j].green = 255;
            } 
            else{
                image[i * cols + j].green = greenVal;
            }
        }
    }
}
int main() {
    
    for (int i = 0; i < rows; i ++){
        for (int j = 0; j < cols; j ++){
            image[i * cols + j].red = (i * j) % 256;
            image[i * cols + j].blue = (i * j) % 256;
            image[i * cols + j].green = (i * j) % 256;
        }

    }

    write_csr(mhpmevent3,1);//30
    write_csr(mhpmevent4,2);//29
    write_csr(mhpmevent5,4);//27
    write_csr(mhpmevent6,5);//26
    write_csr(mhpmevent7,6);//
    write_csr(mhpmevent8,7);
    write_csr(mhpmevent9,8);
    write_csr(mhpmevent10,9);
    write_csr(mhpmevent11,10);
    write_csr(mhpmevent12,11);
    write_csr(mhpmevent13,12);
    write_csr(mhpmevent14,13);
    write_csr(mhpmevent15,14);
    write_csr(mhpmevent16,15);
    write_csr(mhpmevent17,16);
    write_csr(mhpmevent18,17);
    write_csr(mhpmevent19,18);
    write_csr(mhpmevent20,19);
    write_csr(mhpmevent21,20);
    write_csr(mhpmevent22,21);
    write_csr(mhpmevent23,22);
    write_csr(mhpmevent24,23);
    write_csr(mhpmevent25,24);
    write_csr(mhpmevent26,25);
    write_csr(mhpmevent27,26);
    write_csr(mhpmevent28,27);
    write_csr(mhpmevent29,28);
    write_csr(mhpmevent30,29);
    write_csr(mhpmevent31,30);

    // Apply white balance
    whiteBalance();
    
    write_csr(mhpmevent3,0);
    write_csr(mhpmevent4,0);
    write_csr(mhpmevent5,0);
    write_csr(mhpmevent6,0);
    write_csr(mhpmevent7,0);
    write_csr(mhpmevent8,0);
    write_csr(mhpmevent9,0);
    write_csr(mhpmevent10,0);
    write_csr(mhpmevent11,0);
    write_csr(mhpmevent12,0);
    write_csr(mhpmevent13,0);
    write_csr(mhpmevent14,0);
    write_csr(mhpmevent15,0);
    write_csr(mhpmevent16,0);
    write_csr(mhpmevent17,0);
    write_csr(mhpmevent18,0);
    write_csr(mhpmevent19,0);
    write_csr(mhpmevent20,0);
    write_csr(mhpmevent21,0);
    write_csr(mhpmevent22,0);
    write_csr(mhpmevent23,0);
    write_csr(mhpmevent24,0);
    write_csr(mhpmevent25,0);
    write_csr(mhpmevent26,0);
    write_csr(mhpmevent27,0);
    write_csr(mhpmevent28,0);
    write_csr(mhpmevent29,0);
    write_csr(mhpmevent30,0);
    write_csr(mhpmevent31,0);
    
    printf("mcycle = %d \n",read_csr(mcycle));
    printf("minstret = %d \n",read_csr(minstret));
    printf("misprediction = %d \n",read_csr(mhpmcounter3));
    printf("exceptions = %d \n",read_csr(mhpmcounter4));
    printf("csrops= %d \n",read_csr(mhpmcounter5));
    printf("jumps= %d \n",read_csr(mhpmcounter6));
    printf("branches= %d \n",read_csr(mhpmcounter7));
    printf("floats= %d \n",read_csr(mhpmcounter8));
    printf("muldiv= %d \n",read_csr(mhpmcounter9));
    printf("rawstalls= %d \n",read_csr(mhpmcounter10));
    printf("exetalls= %d \n",read_csr(mhpmcounter11));
    printf("icache_fbhit= %d \n",read_csr(mhpmcounter12));
    printf("icache_fbrelease= %d \n",read_csr(mhpmcounter13));
    printf("icache_miss= %d \n",read_csr(mhpmcounter14));
    printf("icache_ncaccess= %d \n",read_csr(mhpmcounter15));
    printf("icache_access= %d \n",read_csr(mhpmcounter16));
    printf("dcache_read_access= %d \n",read_csr(mhpmcounter17));
    printf("dcache_write_access= %d \n",read_csr(mhpmcounter18));
    printf("dcache_atomic_access= %d \n",read_csr(mhpmcounter19));
    printf("dcache_nc_read_access= %d \n",read_csr(mhpmcounter20));
    printf("dcache_nc_write_access= %d \n",read_csr(mhpmcounter21));
    printf("dcache_read_miss= %d \n",read_csr(mhpmcounter22));
    printf("dcache_write_miss= %d \n",read_csr(mhpmcounter23));
    printf("dcache_atomic_miss= %d \n",read_csr(mhpmcounter24));
    printf("dcache_read_fb_hits= %d \n",read_csr(mhpmcounter25));
    printf("dcache_write_fb_hits= %d \n",read_csr(mhpmcounter26));
    printf("dcache_atomic_fb_hits= %d \n",read_csr(mhpmcounter27));
    printf("dcache_fb_releases= %d \n",read_csr(mhpmcounter28));
    printf("dcache_line_evictions= %d \n",read_csr(mhpmcounter29));
    printf("itlb_misses= %d \n",read_csr(mhpmcounter30));
    printf("dtlb_misses= %d \n",read_csr(mhpmcounter31));

    printf("Run is over. This is just a long string.");
   
   
    return 0;
}
