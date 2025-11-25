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
    int k0 = source->rows;  // высота исходного изображени€
    int m0 = source->cols;  // ширина исходного изображени€

    Image* result = create_image(k1, m1);

    for (int i = 0; i < k1; i++) {
        // ÷ентр пиксел€ в новом изображении -> координата по высоте в старом
        double src_y = ((i + 0.5) * k0 / (double)k1) - 0.5;
        if (src_y < 0) src_y = 0;
        if (src_y > k0 - 1) src_y = k0 - 1;

        int y0 = (int)src_y;
        int y1 = y0 + 1;
        if (y1 >= k0) y1 = k0 - 1;

        double fy = src_y - y0;  // дробна€ часть по высоте

        for (int j = 0; j < m1; j++) {
            // ÷ентр пиксел€ в новом изображении -> координата по ширине в старом
            double src_x = ((j + 0.5) * m0 / (double)m1) - 0.5;
            if (src_x < 0) src_x = 0;
            if (src_x > m0 - 1) src_x = m0 - 1;

            int x0 = (int)src_x;
            int x1 = x0 + 1;
            if (x1 >= m0) x1 = m0 - 1;

            double fx = src_x - x0;  // дробна€ часть по ширине

            // «начени€ 4 соседей (0 или 1)
            int s00 = source->data[y0][x0];
            int s01 = source->data[y0][x1];
            int s10 = source->data[y1][x0];
            int s11 = source->data[y1][x1];

            // ƒвухлинейна€ интерпол€ци€ в [0,1]
            double v0 = s00 * (1.0 - fx) + s01 * fx;
            double v1 = s10 * (1.0 - fx) + s11 * fx;
            double v = v0 * (1.0 - fy) + v1 * fy;

            // Ѕинаризаци€: 0 или 1
            result->data[i][j] = (v >= 0.5) ? 1 : 0;
        }
    }

    return result;
}
