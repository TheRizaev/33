#ifndef ALGORITHMS_H
#define ALGORITHMS_H

typedef struct {
    int** data;
    int rows;
    int cols;
} Image;

Image* create_image(int rows, int cols);
void free_image(Image* img);
void print_image(Image* img);
Image* copy_image(Image* img);

Image* algorithm1_weighted_averaging(Image* source, int k1, int m1);
Image* algorithm2_simple_averaging(Image* source, int k1, int m1);

#endif