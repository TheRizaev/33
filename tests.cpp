#include "tests.h"
#include <stdio.h>
#include <time.h>

void run_single_test(TestCase* test) {
    printf("\n========================================\n");
    printf("Test: %s\n", test->name);
    printf("Vhodnoe izobrazhenie (%dx%d) -> Celevoy razmer: %dx%d\n",
        test->input->rows, test->input->cols, test->target_rows, test->target_cols);

    // Показываем входное изображение только если оно небольшое
    if (test->input->rows <= 10 && test->input->cols <= 10) {
        printf("Vhodnoe izobrazhenie:\n");
        print_image(test->input);
    }

    // Алгоритм 1
    clock_t start1 = clock();
    Image* result1 = algorithm1_weighted_averaging(test->input, test->target_rows, test->target_cols);
    clock_t end1 = clock();
    double time1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC * 1000.0;

    printf("\n--- Algoritm 1 (vzveshennoye usredneniye) ---\n");
    printf("Vremya vypolneniya: %.2f ms\n", time1);

    if (result1->rows <= 20 && result1->cols <= 20) {
        printf("Rezultat:\n");
        print_image(result1);
    }

    // Алгоритм 2
    clock_t start2 = clock();
    Image* result2 = algorithm2_simple_averaging(test->input, test->target_rows, test->target_cols);
    clock_t end2 = clock();
    double time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC * 1000.0;

    printf("\n--- Algoritm 2 (prostoye usredneniye) ---\n");
    printf("Vremya vypolneniya: %.2f ms\n", time2);

    if (result2->rows <= 20 && result2->cols <= 20) {
        printf("Rezultat:\n");
        print_image(result2);
    }

    // Сравнение результатов
    int differences = 0;
    for (int i = 0; i < result1->rows; i++) {
        for (int j = 0; j < result1->cols; j++) {
            if (result1->data[i][j] != result2->data[i][j]) {
                differences++;
            }
        }
    }

    printf("\n--- Sravneniye algoritmov ---\n");
    printf("Razlichiy mezhdu rezultatami: %d iz %d pixeley (%.2f%%)\n",
        differences, result1->rows * result1->cols,
        100.0 * differences / (result1->rows * result1->cols));

    if (time1 > 0.01 && time2 > 0.01) {
        printf("Raznica vo vremeni: %.2f ms\n", time1 - time2);
        if (time1 < time2) {
            printf(">>> Algoritm 1 BYSTREE na %.2f%% <<<\n", (time2 - time1) / time2 * 100.0);
        }
        else {
            printf(">>> Algoritm 2 BYSTREE na %.2f%% <<<\n", (time1 - time2) / time1 * 100.0);
        }
    }
    else {
        printf("Test sliskom bystriy dlya tochnogo izmereniya vremeni\n");
    }

    free_image(result1);
    free_image(result2);
}

void run_all_tests() {
    int test_count;
    TestCase** tests = get_all_tests(&test_count);

    printf("==========================================\n");
    printf("   SISTEMA TESTIROVANIYA ALGORITMOV      \n");
    printf("   Masshtabirovaniye rastrovykh          \n");
    printf("   izobrazheniy                          \n");
    printf("==========================================\n");
    printf("\nVsego testov: %d\n", test_count);

    double total_time1 = 0.0;
    double total_time2 = 0.0;
    int total_differences = 0;
    int total_pixels = 0;

    for (int i = 0; i < test_count; i++) {
        clock_t test_start = clock();

        // Алгоритм 1
        clock_t start1 = clock();
        Image* result1 = algorithm1_weighted_averaging(tests[i]->input,
            tests[i]->target_rows,
            tests[i]->target_cols);
        clock_t end1 = clock();
        double time1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC * 1000.0;
        total_time1 += time1;

        // Алгоритм 2
        clock_t start2 = clock();
        Image* result2 = algorithm2_simple_averaging(tests[i]->input,
            tests[i]->target_rows,
            tests[i]->target_cols);
        clock_t end2 = clock();
        double time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC * 1000.0;
        total_time2 += time2;

        // Подсчет различий
        int differences = 0;
        for (int r = 0; r < result1->rows; r++) {
            for (int c = 0; c < result1->cols; c++) {
                if (result1->data[r][c] != result2->data[r][c]) {
                    differences++;
                }
            }
        }
        total_differences += differences;
        total_pixels += result1->rows * result1->cols;

        clock_t test_end = clock();
        double test_time = ((double)(test_end - test_start)) / CLOCKS_PER_SEC * 1000.0;

        printf("\n[%2d/%2d] %s\n", i + 1, test_count, tests[i]->name);
        printf("        Algoritm 1: %.2f ms | Algoritm 2: %.2f ms", time1, time2);
        if (time1 > 0.01 && time2 > 0.01) {
            if (time1 < time2) {
                printf(" | A1 bystree na %.1f%%", (time2 - time1) / time2 * 100.0);
            }
            else {
                printf(" | A2 bystree na %.1f%%", (time1 - time2) / time1 * 100.0);
            }
        }
        printf("\n        Razlichiy: %d iz %d (%.2f%%)\n",
            differences, result1->rows * result1->cols,
            100.0 * differences / (result1->rows * result1->cols));

        free_image(result1);
        free_image(result2);
    }

    printf("\n========================================\n");
    printf("ITOGOVAYA STATISTIKA\n");
    printf("========================================\n");
    printf("Obshcheye vremya Algoritm 1: %.2f ms\n", total_time1);
    printf("Obshcheye vremya Algoritm 2: %.2f ms\n", total_time2);
    printf("Raznica: %.2f ms\n", total_time1 - total_time2);
    if (total_time1 < total_time2) {
        printf(">>> Algoritm 1 v sreднem BYSTREE na %.2f%% <<<\n",
            (total_time2 - total_time1) / total_time2 * 100.0);
    }
    else {
        printf(">>> Algoritm 2 v sreднem BYSTREE na %.2f%% <<<\n",
            (total_time1 - total_time2) / total_time1 * 100.0);
    }
    printf("\nObshcheye razlichiy: %d iz %d pixeley (%.2f%%)\n",
        total_differences, total_pixels, 100.0 * total_differences / total_pixels);
    printf("========================================\n");

    free_all_tests(tests, test_count);
}