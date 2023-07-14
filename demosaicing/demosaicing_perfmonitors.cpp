/*
    Modified Adaptive-Homogeneity Directed Demosaicing using Hardcoded values with performance metrics
*/
#include <cstdio>
#include <cstdlib>
#include <encoding.h>

struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

const int rows = 1920;
const int cols = 1080;
const int rows_demosaic = rows - 2;
const int cols_demosaic = cols - 2;
const int size = rows * cols;
const int size_demosaic = rows_demosaic * cols_demosaic;

int bayerArray[size];
RGB demosaicArray[size_demosaic];

inline int abs(int a){
    return a > 0 ? a : -a;
}

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
        return (int)a;
    }    
}

/*
    R G
    G B
    
*/
void ahdDemosaic_RGGB(int x, int y) {
    
    int i_1 = x - 1;
    int i_2 = x + 1;
    int i_3 = x - 2;
    int i_4 = x + 2;

    int j_1 = y - 1;
    int j_2 = y + 1;
    int j_3 = y - 2;
    int j_4 = y + 2;
    int H,V;
    int red, blue, green;
    if (x % 2 == 1 && y % 2 == 0){ // Blue Row and Green Pixel
        red = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y]) / 2;
        blue = (bayerArray[x * cols + j_1] + bayerArray[x* cols + j_2]) / 2;
        green = bayerArray[x * cols + y];
    }
    if (x % 2 == 1 && y % 2 == 1){ // Blue Row and Blue Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        red = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        blue = bayerArray[x * cols + y];
    }
    if (x % 2 == 0 && y % 2 == 0){ // Red Row and Red Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        blue = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        red = bayerArray[x * cols + y];

    }
    if (x % 2 == 0 && y % 2 == 1){ // Red Row and Green Pixel
        red = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2]) / 2;
        blue = (bayerArray[i_1 *cols + y] + bayerArray[i_2 * cols + y]) / 2;
        green = bayerArray[x * cols + y];
    }
    
    // Create and return the demosaiced pixel
    RGB pixel;
    pixel.red = clamp(red);
    pixel.green = clamp(green);
    pixel.blue = clamp(blue);
    int temp_x = x - 2;
    int temp_y = y - 2;
    demosaicArray[temp_x * cols_demosaic + temp_y] = pixel;
}

/*
    G R
    B G
    
*/
void ahdDemosaic_GRBG(int x, int y) {
    
    int i_1 = x - 1;
    int i_2 = x + 1;
    int i_3 = x - 2;
    int i_4 = x + 2;

    int j_1 = y - 1;
    int j_2 = y + 1;
    int j_3 = y - 2;
    int j_4 = y + 2;
    int H,V;
    int red, blue, green;
    if (x % 2 == 1 && y % 2 == 1){ // Blue Row and Green Pixel
        red = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y]) / 2;
        blue = (bayerArray[x * cols + j_1] + bayerArray[x* cols + j_2]) / 2;
        green = bayerArray[x * cols + y];
    }
    if (x % 2 == 1 && y % 2 == 0){ // Blue Row and Blue Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        red = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        blue = bayerArray[x * cols + y];
    }
    if (x % 2 == 0 && y % 2 == 1){ // Red Row and Red Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        blue = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        red = bayerArray[x * cols + y];

    }
    if (x % 2 == 0 && y % 2 == 0){ // Red Row and Green Pixel
        red = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2]) / 2;
        blue = (bayerArray[i_1 *cols + y] + bayerArray[i_2 * cols + y]) / 2;
        green = bayerArray[x * cols + y];
    }
    
    // Create and return the demosaiced pixel
    RGB pixel;
    pixel.red = clamp(red);
    pixel.green = clamp(green);
    pixel.blue = clamp(blue);
    int temp_x = x - 2;
    int temp_y = y - 2;
    demosaicArray[temp_x * cols_demosaic + temp_y] = pixel;
}

/*
    B G
    G R
    
*/
void ahdDemosaic_BGGR(int x, int y) {
    
    int i_1 = x - 1;
    int i_2 = x + 1;
    int i_3 = x - 2;
    int i_4 = x + 2;

    int j_1 = y - 1;
    int j_2 = y + 1;
    int j_3 = y - 2;
    int j_4 = y + 2;
    int H,V;
    int red, blue, green;
    if (x % 2 == 0 && y % 2 == 1){ // Blue Row and Green Pixel
        red = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y]) / 2;
        blue = (bayerArray[x * cols + j_1] + bayerArray[x* cols + j_2]) / 2;
        green = bayerArray[x * cols + y];
    }
    if (x % 2 == 0 && y % 2 == 0){ // Blue Row and Blue Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        red = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        blue = bayerArray[x * cols + y];
    }
    if (x % 2 == 1 && y % 2 == 1){ // Red Row and Red Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        blue = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        red = bayerArray[x * cols + y];

    }
    if (x % 2 == 1 && y % 2 == 0){ // Red Row and Green Pixel
        red = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2]) / 2;
        blue = (bayerArray[i_1 *cols + y] + bayerArray[i_2 * cols + y]) / 2;
        green = bayerArray[x * cols + y];
    }
    
    // Create and return the demosaiced pixel
    RGB pixel;
    pixel.red = clamp(red);
    pixel.green = clamp(green);
    pixel.blue = clamp(blue);
    int temp_x = x - 2;
    int temp_y = y - 2;
    demosaicArray[temp_x * cols_demosaic + temp_y] = pixel;
}

/*
    G B
    R G
    
*/
void ahdDemosaic_GBRG(int x, int y) {
    
    int i_1 = x - 1;
    int i_2 = x + 1;
    int i_3 = x - 2;
    int i_4 = x + 2;

    int j_1 = y - 1;
    int j_2 = y + 1;
    int j_3 = y - 2;
    int j_4 = y + 2;
    int H,V;
    int red, blue, green;
    if (x % 2 == 0 && y % 2 == 0){ // Blue Row and Green Pixel
        red = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y]) / 2;
        blue = (bayerArray[x * cols + j_1] + bayerArray[x* cols + j_2]) / 2;
        green = bayerArray[x * cols + y];
    }
    if (x % 2 == 0 && y % 2 == 1){ // Blue Row and Blue Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        red = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        blue = bayerArray[x * cols + y];
    }
    if (x % 2 == 1 && y % 2 == 0){ // Red Row and Red Pixel
        V = abs((bayerArray[i_3 * cols + y] / 2) + (bayerArray[i_4 * cols + y] / 2) - bayerArray[x * cols + y]);
        H = abs((bayerArray[x * cols + j_3] / 2) + (bayerArray[x * cols + j_4] / 2) - bayerArray[x * cols + y]);
        if (H > V){
            green = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2])/ 2;
        }
        else if(H < V){
            green = (bayerArray[i_1 * cols + y] + bayerArray[i_2 * cols + y])/ 2;
        }
        else{
            green = (bayerArray[x * cols + j_3] / 4) + (bayerArray[x * cols + j_1] / 4) + (bayerArray[i_1 * cols + y] / 4) + (bayerArray[i_2 * cols + y] / 4); 
        }
        blue = (bayerArray[i_1 * cols + j_1] / 4) + (bayerArray[i_1 * cols + j_2] / 4) + (bayerArray[i_2 * cols + j_1] / 4) + (bayerArray[i_2 * cols + j_2] / 4);
        red = bayerArray[x * cols + y];

    }
    if (x % 2 == 1 && y % 2 == 1){ // Red Row and Green Pixel
        red = (bayerArray[x * cols + j_1] + bayerArray[x * cols + j_2]) / 2;
        blue = (bayerArray[i_1 *cols + y] + bayerArray[i_2 * cols + y]) / 2;
        green = bayerArray[x * cols + y];
    }
    
    // Create and return the demosaiced pixel
    RGB pixel;
    pixel.red = clamp(red);
    pixel.green = clamp(green);
    pixel.blue = clamp(blue);
    int temp_x = x - 2;
    int temp_y = y - 2;
    demosaicArray[temp_x * cols_demosaic + temp_y] = pixel;
}

int main() {
    
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            bayerArray[i * cols + j] = (i * j) % 256;
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

    // Demosaicing
    for (int i = 2; i < rows_demosaic; ++i) {
        for (int j = 2; j < cols_demosaic; ++j) {
            ahdDemosaic_RGGB(i, j);
        }
    }

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
