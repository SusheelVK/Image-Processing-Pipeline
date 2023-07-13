/*
    Naive unoptimized code for Bilateral Filter of Y values Using Hardcoded Values with Performance Metrics
    Sqrt source: https://stackoverflow.com/questions/19611198/finding-square-root-without-using-sqrt-function
    Exp source: https://stackoverflow.com/questions/70382318/ex-without-math-h?rq=3
*/
#include <stdio.h>
#include <encoding.h>

struct YCoCg{
    unsigned char Y, Co, Cg;
};

const int ROWS = 20;
const int COLS = 20;
const int SIZE = ROWS * COLS;
YCoCg input[SIZE];

const int diameter = 4;
const float sigma_spatial = 5.0f;
const float sigma_range = 5.0f;

inline unsigned char clamp(int a)
{
    if (a < 0)   { return 0; }
    if (a > 255) { return 255; }

    return a;
}

inline double absf(double x){
	return x > 0 ? x : -x;
}
	
inline double fexp(double x){
	double a = 1.0, e = 0;
	int invert = x < 0;
	x = absf(x);
	for (int n = 1; e != e + a; ++n){
		e += a;
		a = a * x / n;
	}
	return invert ? 1/e : e;
}
inline double sqrt(double number)
{
    double error = number * 0.000000001; //define the precision of your result
    double s = number;

    while ((s - number / s) > error) //loop until precision satisfied 
    {
        s = (s + number / s) / 2;
    }
    return s;
}

// Function to compute the Gaussian weight
inline float computeGaussianWeight(float x, float sigma)
{
    return fexp(-0.5f * (x * x) / (sigma * sigma));
}

// Function to apply the bilateral filter to an RGB image
void bilateralFilter()
{
    int kernel_radius = diameter;

    for (int y = 0; y < ROWS; ++y)
    {
        for (int x = 0; x < COLS; ++x)
        {
            float sum_Y = 0.0f;
            float normalization = 0.0f;

            for (int j = -kernel_radius; j <= kernel_radius; ++j)
            {
                for (int i = -kernel_radius; i <= kernel_radius; ++i)
                {
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS)
                    {
                        float spatial_dist = sqrt(i * i + j * j);
                        float spatial_weight = computeGaussianWeight(spatial_dist, sigma_spatial);

                        int idx = ny * COLS + nx;
                        int idx_current = y * COLS + x;

                        float range_diff_Y = input[idx].Y - input[idx_current].Y;
                        float range_weight_Y = computeGaussianWeight(range_diff_Y, sigma_range);

                        float weight = spatial_weight * range_weight_Y;

                        sum_Y += input[idx].Y * weight;
                        normalization += weight;
                    }
                }
            }

            int idx_current = y * COLS + x;
            input[idx_current].Y = clamp(static_cast<unsigned char>(sum_Y / normalization));
        }
    }
}

int main(){

    for (int i = 0; i < SIZE; i++){
        input[i].Y = i % 256;
        input[i].Co = i % 256;
        input[i].Cg = i % 256;
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

    bilateralFilter();

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
}
