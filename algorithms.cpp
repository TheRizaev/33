#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>

Image* create_image(int rows, int cols) {
    Image* img = (Image*)malloc(sizeof(Image));
    img->rows = rows;
    img->cols = cols;
    img->data = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        img->data[i] = (int*)malloc(cols * sizeof(int));
    }
    return img;
}

void free_image(Image* img) {
    if (img == NULL) return;
    for (int i = 0; i < img->rows; i++) {
        free(img->data[i]);
    }
    free(img->data);
    free(img);
}

void print_image(Image* img) {
    for (int i = 0; i < img->rows; i++) {
        for (int j = 0; j < img->cols; j++) {
            printf("%d", img->data[i][j]);
            if (j < img->cols - 1) printf(" ");
        }
        printf("\n");
    }
}

Image* copy_image(Image* img) {
    Image* copy = create_image(img->rows, img->cols);
    for (int i = 0; i < img->rows; i++) {
        for (int j = 0; j < img->cols; j++) {
            copy->data[i][j] = img->data[i][j];
        }
    }
    return copy;
}

Image* algorithm1_weighted_averaging(Image* source, int k1, int m1) {
    int k0 = source->rows;
    int m0 = source->cols;

    Image* result = create_image(k1, m1);

    for (int i = 0; i < k1; i++) {
        for (int j = 0; j < m1; j++) {
            double row_start = (double)i * k0 / k1;
            double row_end = (double)(i + 1) * k0 / k1;
            double col_start = (double)j * m0 / m1;
            double col_end = (double)(j + 1) * m0 / m1;

            double black_sum = 0.0;
            double total_area = 0.0;

            int r_start = (int)row_start;
            int r_end = (int)row_end;
            if (row_end > r_end) r_end++;
            if (r_end > k0) r_end = k0;

            int c_start = (int)col_start;
            int c_end = (int)col_end;
            if (col_end > c_end) c_end++;
            if (c_end > m0) c_end = m0;

            for (int r = r_start; r < r_end; r++) {
                for (int c = c_start; c < c_end; c++) {
                    double r_overlap_start = (r > row_start) ? r : row_start;
                    double r_overlap_end = ((r + 1) < row_end) ? (r + 1) : row_end;
                    double c_overlap_start = (c > col_start) ? c : col_start;
                    double c_overlap_end = ((c + 1) < col_end) ? (c + 1) : col_end;

                    double area = (r_overlap_end - r_overlap_start) *
                        (c_overlap_end - c_overlap_start);

                    total_area += area;
                    if (source->data[r][c] == 1) {
                        black_sum += area;
                    }
                }
            }

            result->data[i][j] = (black_sum >= total_area / 2.0) ? 1 : 0;
        }
    }

    return result;
}

Image* algorithm2_simple_averaging(Image* source, int k1, int m1) {
    int k0 = source->rows;
    int m0 = source->cols;

    Image* result = create_image(k1, m1);

    for (int i = 0; i < k1; i++) {
        for (int j = 0; j < m1; j++) {
            int row_start = (i * k0) / k1;
            int row_end = ((i + 1) * k0 + k1 - 1) / k1;
            int col_start = (j * m0) / m1;
            int col_end = ((j + 1) * m0 + m1 - 1) / m1;

            if (row_end > k0) row_end = k0;
            if (col_end > m0) col_end = m0;

            int black_count = 0;
            int total_count = 0;

            for (int r = row_start; r < row_end; r++) {
                for (int c = col_start; c < col_end; c++) {
                    total_count++;
                    if (source->data[r][c] == 1) {
                        black_count++;
                    }
                }
            }

            result->data[i][j] = (black_count * 2 >= total_count) ? 1 : 0;
        }
    }

    return result;
}