#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>
#include "convolution.h"

float IDENTITY[9] = {
    0.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 0.f
};

float BLUR_3[9] = {
    1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f
};

float BLUR_5[25] = {
    1/25.f, 1/25.f, 1/25.f, 1/25.f, 1/25.f,
    1/25.f, 1/25.f, 1/25.f, 1/25.f, 1/25.f,
    1/25.f, 1/25.f, 1/25.f, 1/25.f, 1/25.f,
    1/25.f, 1/25.f, 1/25.f, 1/25.f, 1/25.f,
    1/25.f, 1/25.f, 1/25.f, 1/25.f, 1/25.f,
};

float BLUR_7[49] = {
    1/49.f, 1/49.f, 1/49.f, 1/49.f, 1/49.f,
    1/49.f, 1/49.f, 1/49.f, 1/49.f, 1/49.f,
    1/49.f, 1/49.f, 1/49.f, 1/49.f, 1/49.f,
    1/49.f, 1/49.f, 1/49.f, 1/49.f, 1/49.f,
    1/49.f, 1/49.f, 1/49.f, 1/49.f, 1/49.f,
};

float GAUSSIAN_BLUR_3[9] = {
    1/16.f, 2/16.f, 1/16.f,
    2/16.f, 4/16.f, 2/16.f,
    1/16.f, 2/16.f, 1/16.f
};

float GAUSSIAN_BLUR_5[25] = {
    1/273.f, 4/273.f, 7/273.f, 4/273.f, 1/273.f,
    4/273.f, 16/273.f, 26/273.f, 16/273.f, 4/273.f,
    7/273.f, 26/273.f, 41/273.f, 26/273.f, 7/273.f,
    4/273.f, 16/273.f, 26/273.f, 16/273.f, 4/273.f,
    1/273.f, 4/273.f, 7/273.f, 4/273.f, 1/273.f
};

float GAUSSIAN_BLUR_7[49] = {
    0.f, 0.f, 1/1003.f, 3/1003.f, 1/1003.f, 0.f, 0.f,
    0.f, 3/1003.f, 13/1003.f, 22/1003.f, 13/1003.f, 3/1003.f, 0.f,
    1/1003.f, 13/1003.f, 59/1003.f, 97/1003.f, 59/1003.f, 13/1003.f, 1/1003.f,
    2/1003.f, 22/1003.f, 97/1003.f, 159/1003.f, 97/1003.f, 22/1003.f, 2/1003.f,
    1/1003.f, 13/1003.f, 59/1003.f, 97/1003.f, 59/1003.f, 13/1003.f, 1/1003.f,
    0.f, 3/1003.f, 13/1003.f, 22/1003.f, 13/1003.f, 3/1003.f, 0.f,
    0.f, 0.f, 1/1003.f, 3/1003.f, 1/1003.f, 0.f, 0.f
};

float SHARP[9] = {
    0.f, 1.f, 0.f,
    1.f, 2.f, 1.f,
    0.f, 1.f, 0.f
};

float ULTRA_SHARP[9] = {
    0.f, 0.f, 0.f,
    0.f, 10.f, 0.f,
    0.f, 0.f, 0.f
};

float SOBEL_X[9] = {
    -1.f, 0.f, 1.f,
    -2.f, 0.f, 2.f,
    -1.f, 0.f, 1.f
};

float SOBEL_Y[9] = {
    -1.f, -2.f, -1.f,
    0.f, 0.f, 0.f,
    1.f, 2.f, 1.f
};

float LAPLACIAN[9] = {
    -1.f, 2.f, -1.f,
    2.f, -4.f, 2.f,
    -1.f, 2.f, -1.f
};

float EMBOSS[9] = {
    -2.f, -1.f, 0.f,
    -1.f, 1.f, 1.f,
    0.f, 1.f, 2.f
};

float OUTLINE[9] = {
    -1.f, -1.f, -1.f,
    -1.f, 8.f, -1.f,
    -1.f, -1.f, -1.f
};

float CHECKER[9] = {
    1.f, -1.f, 1.f,
    -1.f, 1.f, -1.f,
    1.f, -1.f, 1.f
};

float BIGCHECKER[81] = {
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
    -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f,
    -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f,
    -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f,
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
    1/9.f, 1/9.f, 1/9.f, -1/9.f, -1/9.f, -1/9.f, 1/9.f, 1/9.f, 1/9.f,
};

// Pixel data structure
typedef struct {
    uint8_t r, g, b, a;
} Pixel;

// Matrix data structure to store png file info
struct Matrix {
    int width, height;
    Pixel ** data;
};

static struct Matrix * create_matrix(int width, int height) {
    // Allocates memory for the matrix data structure to store image data
    struct Matrix * img = malloc(sizeof(struct Matrix));
    img->width = width;
    img->height = height;
    // Allocate memory for each pixel in the matrix
    img->data = malloc(height * sizeof(Pixel *));
    for (int y = 0; y < height; y++)
        img->data[y] = calloc(width, sizeof(Pixel));
    return img;
}

void free_matrix(ColorMatrix matrix) {
    if (matrix == NULL) {
        return;
    }
    for (int y = 0; y < matrix->height; y++) {
        free(matrix->data[y]);
    }
    free(matrix->data);
    free(matrix);
    matrix = NULL;
}

ColorMatrix read_png_rgba(const char * filename) {
    // Tries to open the file in binary reading mode
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error while loading input file in convolution\n");
        return NULL;
    } 

    // Tests if the file is a png file
    png_byte header[8];
    if (!fread(header, 1, 8, fp)) {
        fclose(fp);
        return NULL;
    }
    // Tests the signature
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return NULL;
    }

    // Create png structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return NULL;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Read data from the given file
    png_init_io(png_ptr, fp);
    // Skips 8 bits
    png_set_sig_bytes(png_ptr, 8);
    // Reads IHDR info from the png
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    // Can be RGB, RGBA, Palette or Grayscale
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Converts to 8 bits format
    if (bit_depth == 16) png_set_strip_16(png_ptr);
    // Convert palette to 8 RGB
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    // Adds alpha to RGB
    if (color_type == PNG_COLOR_TYPE_RGB) png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    // Convert to RGB
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

    // Updates info after modifications
    png_read_update_info(png_ptr, info_ptr);

    // Creates the color matrix
    struct Matrix * img = create_matrix(width, height);
    png_bytep * row_ptrs = malloc(height * sizeof(png_bytep));
    for (int y = 0; y < height; y++)
        row_ptrs[y] = malloc(png_get_rowbytes(png_ptr, info_ptr));

    // Store the colors in the previously created matrix
    png_read_image(png_ptr, row_ptrs);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row_ptrs[y][x * 4]);
            img->data[y][x].r = px[0];
            img->data[y][x].g = px[1];
            img->data[y][x].b = px[2];
            img->data[y][x].a = px[3];
        }

    // Free the used pointers
    for (int y = 0; y < height; y++) {
        free(row_ptrs[y]);
    }
    free(row_ptrs);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return img;
}

void write_png_rgba(const char * filename, ColorMatrix matrix) {
    if (!matrix) {
        fprintf(stderr, "Empty matrix given to write_png_rgba\n");
        return;
    }
    if (!filename) {
        fprintf(stderr, "Empty file name given to write_png_rgba\n");
        return;
    }

    //Open file in writing/binary mode
    FILE * fp = fopen(filename, "wb");
    if (!fp) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Set the IHDR info
    png_set_IHDR(png_ptr, info_ptr, matrix->width, matrix->height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_bytep * row_ptrs = malloc(matrix->height * sizeof(png_bytep));
    if (!row_ptrs) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    for (int y = 0; y < matrix->height; y++) {
        size_t row_size = (size_t)matrix->width * 4;
        row_ptrs[y] = malloc(row_size);
        if (!row_ptrs[y]) {
            for (int k = 0; k < y; k++) {
                free(row_ptrs[k]);
            }
            free(row_ptrs);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(fp);
            return;
        }
        // Get the RGBA Info
        for (int x = 0; x < matrix->width; x++) {
            row_ptrs[y][x * 4 + 0] = matrix->data[y][x].r;
            row_ptrs[y][x * 4 + 1] = matrix->data[y][x].g;
            row_ptrs[y][x * 4 + 2] = matrix->data[y][x].b;
            row_ptrs[y][x * 4 + 3] = matrix->data[y][x].a;
        }
    }

    // Write the info in the new file
    png_write_image(png_ptr, row_ptrs);
    png_write_end(png_ptr, NULL);

    for (int y = 0; y < matrix->height; y++) {
        free(row_ptrs[y]);
    }
    free(row_ptrs);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

ColorMatrix convolution(ColorMatrix image, const float * kernel, int kernel_width, int kernel_height) {
    if (!image) {
        fprintf(stderr, "No image given in convolution\n");
        return NULL;
    }
    if (!kernel) {
        fprintf(stderr, "No kernel given in convolution\n");
        return NULL;
    }
    if (kernel_width % 2 == 0 || kernel_height % 2 == 0) {
        fprintf(stderr, "Kernel's width of height must be an odd integer\n");
        return NULL;
    }

    int img_width = image->width;
    int img_height = image->height;
    
    ColorMatrix output = malloc(sizeof(struct Matrix));
    if(!output) {
        fprintf(stderr, "Error while allocating memory in convolution\n");
    }
    output->width = img_width;
    output->height = img_height;
    output->data = malloc(img_height * sizeof(Pixel*));
    for (int y = 0; y < img_height; y++)
        output->data[y] = calloc(img_width, sizeof(Pixel));
    // Realize the convolution
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x++) {
            float sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0;

            for (int ky = 0; ky < kernel_height; ky++) {
                int yy = y + ky - kernel_height / 2;
                // Make sure the coordinates is in the image
                if (yy < 0 || yy >= img_height) {
                    continue;
                }
                for (int kx = 0; kx < kernel_width; kx++) {
                    int xx = x + kx - kernel_width / 2;
                    // Make sure the coordinates is in the image
                    if (xx < 0 || xx >= img_width) {
                        continue;   
                    }
                    float k = kernel[ky * kernel_width + kx];
                    Pixel p = image->data[yy][xx];
                    sum_r += k * p.r;
                    sum_g += k * p.g;
                    sum_b += k * p.b;
                    sum_a += k * p.a;
                }
            }
            Pixel *output_pixel = &output->data[y][x];
            // Set the info of the colours (between 0 and 255)
            output_pixel->r = (uint8_t)(sum_r < 0 ? 0 : (sum_r > 255 ? 255 : sum_r));
            output_pixel->g = (uint8_t)(sum_g < 0 ? 0 : (sum_g > 255 ? 255 : sum_g));
            output_pixel->b = (uint8_t)(sum_b < 0 ? 0 : (sum_b > 255 ? 255 : sum_b));
            output_pixel->a = (uint8_t)(sum_a < 0 ? 0 : (sum_a > 255 ? 255 : sum_a));
        }
    }

    return output;
}