#include <stdio.h>
#include <stdlib.h>
#include "convolution.h"


int main(int argc, char * argv[]) {
    if (argc != 4) {
        fprintf(stderr, "No arguments provided\n");
        printf("USE : \n");
        printf("./%s INPUT_FILE, OUTPUT_FILE, KERNEL_ID\n",argv[0]);
        printf("--- KERNEL IDs ---\n");
        printf("-IDENTITY : 0\n");
        printf("-BLUR 3x3 : 1\n");
        printf("-BLUR 5x5 : 2\n");
        printf("-BLUR 7x7 : 3\n");
        printf("-GAUSSIAN BLUR 3x3 : 4\n");
        printf("-GAUSSIAN BLUR 5x5 : 5\n");
        printf("-GAUSSIAN BLUR 7x7 : 6\n");
        printf("-SHARP : 7\n");
        printf("-ULTRA SHARP : 8\n");
        printf("-SOBEL X : 9\n");
        printf("-SOBEL Y : 10\n");
        printf("-LAPLACIAN : 11\n");
        printf("-EMBOSS : 12\n");
        printf("-OUTLINE : 13\n");
        printf("-CHECKER 3x3 : 14\n");
        printf("-CHECKER 9x9 : 15\n");
        return -1;
    }
    ColorMatrix matrix = read_png_rgba(argv[1]);
    int kernel_id = atoi(argv[3]);
    if (kernel_id < 0 || kernel_id > 15) {
        fprintf(stderr, "Incorrect kernel id given\n");
        return -2;
    }
    ColorMatrix conv_matrix = convolution(matrix, kernels[kernel_id].kernel, kernels[kernel_id].width, kernels[kernel_id].height);
    write_png_rgba(argv[2],conv_matrix);
    free_matrix(matrix);
    free_matrix(conv_matrix);
    return 0;
}

