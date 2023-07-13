#ifndef IMGPROC_H // include guard
#define IMGPROC_H

#include<iostream>

extern int BAYER_ARRAY[];
extern const int ROWS;
extern const int COLS;
extern const int ROWS_DEMOSAIC;
extern const int COLS_DEMOSAIC;
extern const int SIZE;
extern const int SIZE_DEMOSAIC;
const int MAX_Y = 255;
inline unsigned char clamp(int a)
{
    if (a < 0)   { return 0; }
    if (a > 255) { return 255; }

    return a;
}

inline int abs_val(int a){
    return a > 0 ? a : -a;
}

inline unsigned char clampf(float a)
{
    if (a < 0){ return 0; }
    if (a > 255) { return 255; }

    return (unsigned char)a;
}

struct RGB {
    unsigned char red {0};
    unsigned char green {0};
    unsigned char blue {0};
};

struct YCoCg {
    unsigned char Y {0};
    unsigned char Co {0};
    unsigned char Cg {0};
};

const float RGB_to_YCoCg[3][3] = {{0.25, 0.5, 0.25}, {1.0, 0, -1.0},{-0.5, 1, -0.5}};
const float YCoCg_to_RGB[3][3] = {{-0.75, 1.125, 0.875}, {0.75, 0.625, -0.375}, {0.25, 1.125, -0.625}};

void conv_RGB_to_YCoCg(RGB image[], YCoCg new_image[]) {
    int Bcc;
    for (int i = 0; i < ROWS_DEMOSAIC; ++i) {
        for (int j = 0; j < COLS_DEMOSAIC; ++j) {
            Bcc =  (RGB_to_YCoCg[2][0] * image[i * COLS_DEMOSAIC + j].red) + (RGB_to_YCoCg[2][1] * image[i * COLS_DEMOSAIC + j].green) + (RGB_to_YCoCg[2][2] * image[i * COLS_DEMOSAIC + j].blue);
            new_image[i * COLS_DEMOSAIC + j].Co = clampf(RGB_to_YCoCg[0][0] * image[i * COLS_DEMOSAIC + j].red + RGB_to_YCoCg[0][1] * image[i * COLS_DEMOSAIC + j].green + RGB_to_YCoCg[0][2] * (image[i * COLS_DEMOSAIC + j].blue - Bcc));
            new_image[i * COLS_DEMOSAIC + j].Cg = clampf(RGB_to_YCoCg[1][0] * image[i * COLS_DEMOSAIC + j].red + RGB_to_YCoCg[1][1] * image[i * COLS_DEMOSAIC + j].green + RGB_to_YCoCg[1][2] * (image[i * COLS_DEMOSAIC + j].blue - Bcc) + new_image[i * COLS_DEMOSAIC + j].Co / 2);
            new_image[i * COLS_DEMOSAIC + j].Y = clampf(Bcc + new_image[i * COLS_DEMOSAIC + j].Co / 2 + new_image[i * COLS_DEMOSAIC + j].Cg / 2);
        }
    }
}

void conv_YCoCg_to_RGB(RGB old_image[], YCoCg new_image[]) {
    int red, blue, green;
    for (int i = 0; i < ROWS_DEMOSAIC; ++i) {
        for (int j = 0; j < COLS_DEMOSAIC; ++j) {
            blue = YCoCg_to_RGB[2][1] * new_image[i * COLS_DEMOSAIC + j].Co + YCoCg_to_RGB[2][2] * new_image[i * COLS_DEMOSAIC + j].Cg + YCoCg_to_RGB[2][0] * new_image[i * COLS_DEMOSAIC + j].Y;
            green = YCoCg_to_RGB[1][1] * new_image[i * COLS_DEMOSAIC + j].Co + YCoCg_to_RGB[1][2] * new_image[i * COLS_DEMOSAIC + j].Cg + YCoCg_to_RGB[1][0] * new_image[i * COLS_DEMOSAIC + j].Y;
            red =  YCoCg_to_RGB[0][1] * new_image[i * COLS_DEMOSAIC + j].Co + YCoCg_to_RGB[0][2] * new_image[i * COLS_DEMOSAIC + j].Cg + YCoCg_to_RGB[0][0] * new_image[i * COLS_DEMOSAIC + j].Y; 
            old_image[i * COLS_DEMOSAIC + j].green = clamp(green);
            old_image[i * COLS_DEMOSAIC + j].blue = clamp(blue);
            old_image[i * COLS_DEMOSAIC + j].red = clamp(red);
        }
    }
}
void whiteBalance(RGB image[]) {
    int totalRed = 0;
    int totalGreen = 0;
    int totalBlue = 0;
    // Calculate the total of each color channel
    for (int i = 0; i < ROWS_DEMOSAIC; ++i) {
        for (int j = 0; j < COLS_DEMOSAIC; ++j){
            totalRed += image[i * COLS_DEMOSAIC + j].red;
            totalGreen += image[i * COLS_DEMOSAIC + j].green;
            totalBlue += image[i * COLS_DEMOSAIC + j].blue;
        }
    }
    // Calculate scale to multiply with each pixel
    float scale = (totalRed + totalGreen + totalBlue) / 3.0;

    float RedScale = scale / totalRed;
    float BlueScale = scale / totalBlue;
    float GreenScale = scale / totalGreen;
 
    // Adjust each pixel based on the total color values
    for (int i = 0; i < ROWS_DEMOSAIC; ++i) {
        for (int j = 0; j < COLS_DEMOSAIC; ++j){
            int redVal = image[i * COLS_DEMOSAIC + j].red * RedScale;
            int blueVal = image[i * COLS_DEMOSAIC + j].blue * BlueScale;
            int greenVal = image[i * COLS_DEMOSAIC + j].green * GreenScale;
            if (redVal > 255){
                image[i * COLS_DEMOSAIC + j].red = 255;
            } 
            else{
                image[i * COLS_DEMOSAIC + j].red = redVal;
            }
            if (blueVal > 255){
                image[i * COLS_DEMOSAIC + j].blue = 255;
            } 
            else{
                image[i * COLS_DEMOSAIC + j].blue = blueVal;
            }
            if (greenVal > 255){
                image[i * COLS_DEMOSAIC + j].green = 255;
            } 
            else{
                image[i * COLS_DEMOSAIC + j].green = greenVal;
            }
        }
    }
}
/*
    B G
    G R
*/
void ahdDemosaic_BGGR(RGB demosaicArray[]) {
    for (int i = 2; i < ROWS_DEMOSAIC; ++i) {
    
        for (int j = 2; j < COLS_DEMOSAIC; ++j) {
            int x = i;
            int y = j;

            int i_1 = x - 1;
            int i_2 = x + 1;
            int i_3 = x - 2;
            int i_4 = x + 2;

            int j_1 = y - 1;
            int j_2 = y + 1;
            int j_3 = y - 2;
            int j_4 = y + 2;
            int H,V;
            float red, blue, green;
            if (x % 2 == 0 && y % 2 == 1){ // Blue Row and Green Pixel
                red = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y]) / 2;
                blue = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x* COLS + j_2]) / 2;
                green = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 0 && y % 2 == 0){ // Blue Row and Blue Pixel
                V = abs_val((BAYER_ARRAY[i_3 * COLS + y] / 2) + (BAYER_ARRAY[i_4 * COLS + y] / 2) - BAYER_ARRAY[x * COLS + y]);
                H = abs_val((BAYER_ARRAY[x * COLS + j_3] / 2) + (BAYER_ARRAY[x * COLS + j_4] / 2) - BAYER_ARRAY[x * COLS + y]);
                if (H > V){
                    green = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2])/ 2;
                }
                else if(H < V){
                    green = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y])/ 2;
                }
                else{
                    green = -(BAYER_ARRAY[x * COLS + j_3] / 4) + (BAYER_ARRAY[x * COLS + j_1] / 2) + (BAYER_ARRAY[x * COLS + y] / 2) + (BAYER_ARRAY[x * COLS + j_2] / 2) - (BAYER_ARRAY[x * COLS + j_4] / 4); 
                }
                red = (BAYER_ARRAY[i_1 * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + j_2] / 4) + (BAYER_ARRAY[i_2 * COLS + j_1] / 4) + (BAYER_ARRAY[i_2 * COLS + j_2] / 4);
                blue = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 1 && y % 2 == 1){ // Red Row and Red Pixel
                V = abs_val((BAYER_ARRAY[i_3 * COLS + y] / 2) + (BAYER_ARRAY[i_4 * COLS + y] / 2) - BAYER_ARRAY[x * COLS + y]);
                H = abs_val((BAYER_ARRAY[x * COLS + j_3] / 2) + (BAYER_ARRAY[x * COLS + j_4] / 2) - BAYER_ARRAY[x * COLS + y]);
                if (H > V){
                    green = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2])/ 2;
                }
                else if(H < V){
                    green = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y])/ 2;
                }
                else{
                    green = -(BAYER_ARRAY[x * COLS + j_3] / 4) + (BAYER_ARRAY[x * COLS + j_1] / 2) + (BAYER_ARRAY[x * COLS + y] / 2) + (BAYER_ARRAY[x * COLS + j_2] / 2) - (BAYER_ARRAY[x * COLS + j_4] / 4); 
                }
                blue = (BAYER_ARRAY[i_1 * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + j_2] / 4) + (BAYER_ARRAY[i_2 * COLS + j_1] / 4) + (BAYER_ARRAY[i_2 * COLS + j_2] / 4);
                red = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 1 && y % 2 == 0){ // Red Row and Green Pixel
                red = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2]) / 2;
                blue = (BAYER_ARRAY[i_1 *COLS + y] + BAYER_ARRAY[i_2 * COLS + y]) / 2;
                green = BAYER_ARRAY[x * COLS + y];
            }
            
            // Create and return the demosaiced pixel
            RGB pixel;
            pixel.red = clampf(red);
            pixel.green = clampf(green);
            pixel.blue = clampf(blue);
            int temp_x = x - 2;
            int temp_y = y - 2;
            demosaicArray[temp_x * COLS_DEMOSAIC + temp_y] = pixel;
        }
    }
}

/*
    R G
    G B
*/
void ahdDemosaic_RGGB(RGB demosaicArray[]) {
    for (int i = 2; i < ROWS_DEMOSAIC; ++i) {
    
        for (int j = 2; j < COLS_DEMOSAIC; ++j) {
            int x = i;
            int y = j;

            int i_1 = x - 1;
            int i_2 = x + 1;
            int i_3 = x - 2;
            int i_4 = x + 2;

            int j_1 = y - 1;
            int j_2 = y + 1;
            int j_3 = y - 2;
            int j_4 = y + 2;
            int H,V;
            float red, blue, green;
            if (x % 2 == 1 && y % 2 == 0){ // Blue Row and Green Pixel
                red = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y]) / 2;
                blue = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x* COLS + j_2]) / 2;
                green = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 1 && y % 2 == 1){ // Blue Row and Blue Pixel
                V = abs_val((BAYER_ARRAY[i_3 * COLS + y] / 2) + (BAYER_ARRAY[i_4 * COLS + y] / 2) - BAYER_ARRAY[x * COLS + y]);
                H = abs_val((BAYER_ARRAY[x * COLS + j_3] / 2) + (BAYER_ARRAY[x * COLS + j_4] / 2) - BAYER_ARRAY[x * COLS + y]);
                if (H > V){
                    green = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2])/ 2;
                }
                else if(H < V){
                    green = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y])/ 2;
                }
                else{
                    green = (BAYER_ARRAY[x * COLS + j_3] / 4) + (BAYER_ARRAY[x * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + y] / 4) + (BAYER_ARRAY[i_2 * COLS + y] / 4); 
                }
                red = (BAYER_ARRAY[i_1 * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + j_2] / 4) + (BAYER_ARRAY[i_2 * COLS + j_1] / 4) + (BAYER_ARRAY[i_2 * COLS + j_2] / 4);
                blue = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 0 && y % 2 == 0){ // Red Row and Red Pixel
                V = abs_val((BAYER_ARRAY[i_3 * COLS + y] / 2) + (BAYER_ARRAY[i_4 * COLS + y] / 2) - BAYER_ARRAY[x * COLS + y]);
                H = abs_val((BAYER_ARRAY[x * COLS + j_3] / 2) + (BAYER_ARRAY[x * COLS + j_4] / 2) - BAYER_ARRAY[x * COLS + y]);
                if (H > V){
                    green = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2])/ 2;
                }
                else if(H < V){
                    green = (BAYER_ARRAY[i_1 * COLS + y] + BAYER_ARRAY[i_2 * COLS + y])/ 2;
                }
                else{
                    green = (BAYER_ARRAY[x * COLS + j_3] / 4) + (BAYER_ARRAY[x * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + y] / 4) + (BAYER_ARRAY[i_2 * COLS + y] / 4); 
                }
                blue = (BAYER_ARRAY[i_1 * COLS + j_1] / 4) + (BAYER_ARRAY[i_1 * COLS + j_2] / 4) + (BAYER_ARRAY[i_2 * COLS + j_1] / 4) + (BAYER_ARRAY[i_2 * COLS + j_2] / 4);
                red = BAYER_ARRAY[x * COLS + y];
            }
            if (x % 2 == 0 && y % 2 == 1){ // Red Row and Green Pixel
                red = (BAYER_ARRAY[x * COLS + j_1] + BAYER_ARRAY[x * COLS + j_2]) / 2;
                blue = (BAYER_ARRAY[i_1 *COLS + y] + BAYER_ARRAY[i_2 * COLS + y]) / 2;
                green = BAYER_ARRAY[x * COLS + y];
            }
            
            // Create and return the demosaiced pixel
            RGB pixel;
            pixel.red = clampf(red);
            pixel.green = clampf(green);
            pixel.blue = clampf(blue);
            int temp_x = x - 2;
            int temp_y = y - 2;
            demosaicArray[temp_x * COLS_DEMOSAIC + temp_y] = pixel;
        }
    }
}

void conv_RGB_to_YCoCg_Auto(RGB image[], YCoCg new_image[]) {
    int y_max = 0;
    int y_min = 255;
    int Bcc;
    for (int i = 0; i < ROWS_DEMOSAIC; ++i) {
        for (int j = 0; j < COLS_DEMOSAIC; ++j) {
            Bcc =  (RGB_to_YCoCg[2][0] * image[i * COLS_DEMOSAIC + j].red) + (RGB_to_YCoCg[2][1] * image[i * COLS_DEMOSAIC + j].green) + (RGB_to_YCoCg[2][2] * image[i * COLS_DEMOSAIC + j].blue);
            new_image[i * COLS_DEMOSAIC + j].Co = clamp(RGB_to_YCoCg[0][0] * image[i * COLS_DEMOSAIC + j].red + RGB_to_YCoCg[0][1] * image[i * COLS_DEMOSAIC + j].green + RGB_to_YCoCg[0][2] * (image[i * COLS_DEMOSAIC + j].blue - Bcc));
            new_image[i * COLS_DEMOSAIC + j].Cg =  clamp(RGB_to_YCoCg[1][0] * image[i * COLS_DEMOSAIC + j].red + RGB_to_YCoCg[1][1] * image[i * COLS_DEMOSAIC + j].green + RGB_to_YCoCg[1][2] * (image[i * COLS_DEMOSAIC + j].blue - Bcc) + new_image[i * COLS_DEMOSAIC + j].Co / 2);
            new_image[i * COLS_DEMOSAIC + j].Y =  clamp(Bcc + new_image[i * COLS_DEMOSAIC + j].Co / 2 + new_image[i * COLS_DEMOSAIC + j].Cg / 2);
            if(new_image[i * COLS_DEMOSAIC + j].Y > y_max){
                y_max = new_image[i * COLS_DEMOSAIC + j].Y;
            }
            if(new_image[i * COLS_DEMOSAIC + j].Y < y_min){
                y_min = new_image[i * COLS_DEMOSAIC + j].Y;
            }
        }
    }
    int diff = y_max - y_min;
    for (int i = 0; i < ROWS_DEMOSAIC; i ++){
        for (int j = 0; j < COLS_DEMOSAIC; j ++){
            new_image[i * COLS_DEMOSAIC + j].Y = clamp(((new_image[i * COLS_DEMOSAIC + j].Y - y_min) * MAX_Y)/ diff);
        }
    }
}

void autoContrast(YCoCg new_image[]){
    int y_max = 0;
    int y_min = 255;

    for (int i = 0; i < ROWS_DEMOSAIC; i ++){
        for (int j = 0; j < COLS_DEMOSAIC; j ++){
            if(new_image[i * COLS_DEMOSAIC + j].Y > y_max){
                y_max = new_image[i * COLS_DEMOSAIC + j].Y;
            }
            if(new_image[i * COLS_DEMOSAIC + j].Y < y_min){
                y_min = new_image[i * COLS_DEMOSAIC + j].Y;
            }
        }
    } 
    int diff = y_max - y_min;
    for (int i = 0; i < ROWS_DEMOSAIC; i ++){
        for (int j = 0; j < COLS_DEMOSAIC; j ++){
            new_image[i * COLS_DEMOSAIC + j].Y = ((new_image[i * COLS_DEMOSAIC + j].Y - y_min) * 255)/ diff;
        }
    }
}


void normalize(YCoCg image[]){
	for (int i = 0; i < SIZE_DEMOSAIC; i++){
		image[i].Y /= 255;
		
	}
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
// Function to compute the Gaussian weight
inline float computeGaussianWeight(float x, float sigma)
{
    return fexp(-0.5f * (x * x) / (sigma * sigma));
}

// Function to apply the bilateral filter to an RGB image
void bilateralFilterRGB(RGB input[], int diameter, float sigma_spatial, float sigma_range)
{
    int kernel_radius = diameter;

    for (int y = 0; y < ROWS_DEMOSAIC; ++y)
    {
        for (int x = 0; x < COLS_DEMOSAIC; ++x)
        {
            float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;
            float normalization = 0.0f;

            for (int j = -kernel_radius; j <= kernel_radius; ++j)
            {
                for (int i = -kernel_radius; i <= kernel_radius; ++i)
                {
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < COLS_DEMOSAIC && ny >= 0 && ny < ROWS_DEMOSAIC)
                    {
                        float spatial_dist = sqrt(i * i + j * j);
                        float spatial_weight = computeGaussianWeight(spatial_dist, sigma_spatial);

                        int idx = ny * COLS_DEMOSAIC + nx;
                        int idx_current = y * COLS_DEMOSAIC + x;

                        float range_diff_r = input[idx].red - input[idx_current].red;
                        float range_diff_g = input[idx].green - input[idx_current].green;
                        float range_diff_b = input[idx].blue - input[idx_current].blue;

                        float range_weight_r = computeGaussianWeight(range_diff_r, sigma_range);
                        float range_weight_g = computeGaussianWeight(range_diff_g, sigma_range);
                        float range_weight_b = computeGaussianWeight(range_diff_b, sigma_range);

                        float weight = spatial_weight * range_weight_r * range_weight_g * range_weight_b;

                        sum_r += input[idx].red * weight;
                        sum_g += input[idx].green * weight;
                        sum_b += input[idx].blue * weight;

                        normalization += weight;
                    }
                }
            }

            int idx_current = y * COLS_DEMOSAIC + x;
            input[idx_current].red = static_cast<unsigned char>(sum_r / normalization);
            input[idx_current].green = static_cast<unsigned char>(sum_g / normalization);
            input[idx_current].blue = static_cast<unsigned char>(sum_b / normalization);
        }
    }
}


void bilateralFilterYCoCg(YCoCg input[], int diameter, float sigma_spatial, float sigma_range)
{
    int kernel_radius = diameter;

    for (int y = 0; y < ROWS_DEMOSAIC; ++y)
    {
        for (int x = 0; x < COLS_DEMOSAIC; ++x)
        {
            float sum_Y = 0.0f;
            float normalization = 0.0f;

            for (int j = -kernel_radius; j <= kernel_radius; ++j)
            {
                for (int i = -kernel_radius; i <= kernel_radius; ++i)
                {
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < COLS_DEMOSAIC && ny >= 0 && ny < ROWS_DEMOSAIC)
                    {
                        float spatial_dist = sqrt(i * i + j * j);
                        float spatial_weight = computeGaussianWeight(spatial_dist, sigma_spatial);

                        int idx = ny * COLS_DEMOSAIC + nx;
                        int idx_current = y * COLS_DEMOSAIC + x;

                        float range_diff_Y = input[idx].Y - input[idx_current].Y;

                        float range_weight_Y = computeGaussianWeight(range_diff_Y, sigma_range);

                        float weight = spatial_weight * range_weight_Y;

                        sum_Y += input[idx].Y * weight;

                        normalization += weight;
                    }
                }
            }

            int idx_current = y * COLS_DEMOSAIC + x;
            input[idx_current].Y = clamp(static_cast<unsigned char>(sum_Y / normalization));
        }
    }
}

#endif
