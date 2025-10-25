# Image Convolution

A small project to test image convolution with multiple kernels (Gaussian, Sharp, Laplacian, Sobel, ...).

## Description

This project implements image convolution in C to learn more about image processing. Convolution involves sliding a small matrix (called a kernel/filter) over an image to modify its pixel values based on local neighborhoods.
By designing different kernels, we can achieve various effects such as blurring, sharpening, detecting edges, and embossing. Please note that this process is for educational purposes and that the program is likely to contain errors.

## Getting Started

### Executing program

* Compiling
```
make
```
* Running
```
./main INPUT_LOCATION OUTPUT_LOCATION KERNEL_ID
```

## Help

If you encounter issues or unexpected results, check that:
* The image is a correct png file.
* The kernel ID provided is valid (0-15).
* The output directory is writable.

## License

This project is licensed under the MIT License - see the LICENSE.md file for details
I do not own the rights of the example pictures.
