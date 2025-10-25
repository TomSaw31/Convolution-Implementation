#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

typedef struct Matrix * ColorMatrix;

extern float IDENTITY[9];
// DEFAULT BLUR
extern float BLUR_3[9];
extern float BLUR_5[25];
extern float BLUR_7[49];
// GAUSSIAN BLUR
extern float GAUSSIAN_BLUR_3[9];
extern float GAUSSIAN_BLUR_5[25];
extern float GAUSSIAN_BLUR_7[49];
// OTHERS 3x3
extern float SHARP[9];
extern float ULTRA_SHARP[9];
extern float SOBEL_X[9];
extern float SOBEL_Y[9];
extern float LAPLACIAN[9];
extern float EMBOSS[9];
extern float OUTLINE[9];
extern float CHECKER[9];
// 9x9
extern float BIGCHECKER[81];

typedef struct {
    const float *kernel;
    int width;
    int height;
} KernelInfo;

static const KernelInfo kernels[16] = {
    {IDENTITY, 3, 3},
    {BLUR_3, 3, 3},
    {BLUR_5, 5, 5},
    {BLUR_7, 7, 7},
    {GAUSSIAN_BLUR_3, 3, 3},
    {GAUSSIAN_BLUR_5, 5, 5},
    {GAUSSIAN_BLUR_7, 7, 7},
    {SHARP, 3, 3},
    {ULTRA_SHARP, 3, 3},
    {SOBEL_X, 3, 3},
    {SOBEL_Y, 3, 3},
    {LAPLACIAN, 3, 3},
    {EMBOSS, 3, 3},
    {OUTLINE, 3, 3},
    {CHECKER, 3, 3},
    {BIGCHECKER, 9, 9}
};

/**
 * Convert the given PNG file to a matrix containing the pixel data information.
 * @param filename Filename of the PNG file
 * @return the matrix of the image
 */
ColorMatrix read_png_rgba(const char *filename);

/**
 * Free the given matrix
 * @param matrix ColorMatrix to free
 */
void free_matrix(ColorMatrix matrix);

/**
 * Create a new image according to the given matrix
 * @param filename Name of the result file created
 */
void write_png_rgba(const char * filename, ColorMatrix matrix);

/**
 * Realize a convolution on the given image based on the given kernel
 * @param image Matrix containing image info
 * @param kernel Kernel used for convolution
 * @param kernel_width Width of the kernel (must be odd)
 * @param kernel_height Height of the kernel (must be odd)
 * @return Matrix containing the new convoluted image
 */
ColorMatrix convolution(ColorMatrix image, const float * kernel, int kernel_width, int kernel_height);

#endif
