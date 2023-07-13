/*
    Color Conversion from YCoCg to RGB Using Hardcoded Values with Performance Metrics
*/

#include <cstdio>
#include <cstdlib>
#include <encoding.h>

inline unsigned char clamp(int a)
{
    if (a < 0){
        return 0;
    }
    else if (a > 255)
    {
        return 255;
    }
    else{
        return a;
    }    
}

struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct YCoCg {
    short int Y;
    short int Co;
    short int Cg;
};

const float YCoCg_to_RGB[3][3] = {{-0.75, 1.125, 0.875}, {0.75, 0.625, -0.375}, {0.25, 1.125, -0.625}};

const int rows = 200;
const int cols = 200;

YCoCg image[rows * cols];
RGB new_image[rows * cols];

void conv_YCoCg_to_RGB() {
    int red, blue, green;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            blue = YCoCg_to_RGB[2][1] * image[i * cols + j].Co + YCoCg_to_RGB[2][2] * image[i * cols + j].Cg + YCoCg_to_RGB[2][0] * image[i * cols + j].Y;
            green = YCoCg_to_RGB[1][1] * image[i * cols + j].Co + YCoCg_to_RGB[1][2] * image[i * cols + j].Cg + YCoCg_to_RGB[1][0] * image[i * cols + j].Y;
            red =  YCoCg_to_RGB[0][1] * image[i * cols + j].Co + YCoCg_to_RGB[0][2] * image[i * cols + j].Cg + YCoCg_to_RGB[0][0] * image[i * cols + j].Y; 
            new_image[i * cols + j].green = clamp(green);
            new_image[i * cols + j].blue = clamp(blue);
            new_image[i * cols + j].red = clamp(red);
        }
    }
}

int main() {
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            image[i * cols + j].Y = (i * j) % 256;
            image[i * cols + j].Co = (i * j) % 256;
            image[i * cols + j].Cg = (i * j) % 256;
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

    // Apply color change
    conv_YCoCg_to_RGB();
    
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
    
